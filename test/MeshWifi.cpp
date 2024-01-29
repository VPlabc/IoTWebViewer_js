#include "MeshWifi.h"

#include <ArduinoJson.h>

MESH_WIFI mesh_wifi;

//Structure example to send data
    //Must match the receiver structure
    typedef struct struct_message {
        byte networkID;       //1
        byte nodeID;          //1
        byte category;        //1
        byte status;          //1
        float temperature;    //4
        float humidity;       //4
        float mbattery;       //4
        float battery;        //4
        int RSSI;             //4
    } struct_message;
    // Create a struct_message called BME280Readings to hold sensor readings
    struct_message DataSenddings;

    typedef struct struct_pairing {       // new structure for pairing
        uint8_t msgType;
        uint8_t id;
        uint8_t macAddr[6];
        uint8_t channel;
    } struct_pairing;

struct_message incomingReadings_main;
struct_message outgoingSetpoints;
struct_pairing pairingData;

#define LOGLN(string){Serial.println(string);}
#define LOG(string){Serial.print(string);}

bool Debug;
esp_now_peer_info_t slave;
int chan; 
uint8_t current_protocol;
esp_now_peer_info_t peerInfo;
esp_interface_t current_esp_interface;
wifi_interface_t current_wifi_interface;
int rssi_display;
// Estructuras para calcular los paquetes, el RSSI, etc
typedef struct {
  unsigned frame_ctrl: 16;
  unsigned duration_id: 16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl: 16;
  uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;
//La callback que hace la magia
void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
  // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
  if (type != WIFI_PKT_MGMT)
    return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

  int rssi = ppkt->rx_ctrl.rssi;
  rssi_display = 120 - rssi;
}

int MESH_WIFI::check_protocol()
{
    char error_buf1[100];
  if(Debug){
    LOGLN();
    LOGLN("___________________________________");
    LOGLN();
  }
     esp_err_t error_code = esp_wifi_get_protocol(current_wifi_interface, &current_protocol);
     esp_err_to_name_r(error_code,error_buf1,100);
  if(Debug){
     LOG("esp_wifi_get_protocol error code: ");
     LOGLN(error_buf1);
    LOGLN("Code: " + String(current_protocol));
    if ((current_protocol&WIFI_PROTOCOL_11B) == WIFI_PROTOCOL_11B)
      LOGLN("Protocol is WIFI_PROTOCOL_11B");
    if ((current_protocol&WIFI_PROTOCOL_11G) == WIFI_PROTOCOL_11G)
      LOGLN("Protocol is WIFI_PROTOCOL_11G");
    if ((current_protocol&WIFI_PROTOCOL_11N) == WIFI_PROTOCOL_11N)
      LOGLN("Protocol is WIFI_PROTOCOL_11N");
    if ((current_protocol&WIFI_PROTOCOL_LR) == WIFI_PROTOCOL_LR)
      LOGLN("Protocol is WIFI_PROTOCOL_LR");
    LOGLN("___________________________________");
    LOGLN();
    LOGLN();
  }
    return current_protocol;
}

void MeshRecive(const uint8_t * mac, const uint8_t *incomingDatas, int len);
void MESH_WIFI::Mesh_Init(){
  #ifndef ARDUINO_ARCH_ESP8266
  String dataDisp = "";
  // if (!CONFIG::is_locked(FLAG_BLOCK_OLED)) {
  //  IOT_DEVICE.OLED_Display("Mesh Init",2);}
  //   else{ OLED_DISPLAY::display_text("Mesh Init", 0, 0, 85);}
  // Init ESP-NOW
  WiFi.mode(WIFI_STA);
  #ifdef ARDUINO_ARCH_ESP8266
  #else
  if(check_protocol() != 8){
  esp_wifi_set_protocol(current_wifi_interface, WIFI_PROTOCOL_LR);
  check_protocol();
  }
  #endif
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);
      // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    esp_now_register_send_cb(OnDataSent);
  // // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(MeshRecive);
  #endif//
}

// ---------------------------- esp_ now -------------------------
void printMAC_t(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

bool addPeer(const uint8_t *peer_addr) {      // add pairing
  memset(&slave, 0, sizeof(slave));
  const esp_now_peer_info_t *peer = &slave;
  memcpy(slave.peer_addr, peer_addr, 6);
  
  slave.channel = chan; // pick a channel
  slave.encrypt = 0; // no encryption
  // check if the peer exists
  bool exists = esp_now_is_peer_exist(slave.peer_addr);
  if (exists) {
    // Slave already paired.
    Serial.println("Already Paired");
    return true;
  }
  else {
    esp_err_t addStatus = esp_now_add_peer(peer);
    if (addStatus == ESP_OK) {
      // Pair success
      Serial.println("Pair success");
      return true;
    }
    else 
    {
      Serial.println("Pair failed");
      return false;
    }
  }
}

void espnowSent() {
    #ifdef ESP32
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &DataSenddings, sizeof(DataSenddings));
    if (result == ESP_OK) {
      if(Debug)LOGLN("Sent with success");
    }
    else {
      if(Debug)LOGLN("Error sending the data");
      //  IOT_DEVICE.MeshBegin();
    }
    #endif//def ESP32
}


//----------------------------------------------------------------
//------------ Mesh Reciver Function   ---------------------------
//----------------------------------------------------------------
// Callback when data is received Mesh
struct_message incomingReadings;
void MeshRecive(const uint8_t * mac, const uint8_t *incomingDatas, int len) {

Serial.print(len);
  Serial.print(" bytes of data received from : ");
  printMAC_t(mac_addr);
  Serial.println();
  StaticJsonDocument<1000> root;
  String payload;
  uint8_t type = incomingDatas[0];       // first message byte is the type of message 
  switch (type) {
  case DATA :                           // the message is data type
    memcpy(&incomingReadings_main, incomingDatas, sizeof(incomingReadings_main));
    // create a JSON document with received data and send it by event to the web page
    root["id"] = incomingReadings_main.nodeID;
    root["temperature"] = incomingReadings_main.temperature;
    root["humidity"] = incomingReadings_main.humidity;
    root["readingId"] = String(incomingReadings_main.readingId);
    serializeJson(root, payload);
    Serial.print("event send :");
    serializeJson(root, Serial);
    events.send(payload.c_str(), "new_readings", millis());
    Serial.println();
    break;
  
  case PAIRING:                            // the message is a pairing request 
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    Serial.println(pairingData.msgType);
    Serial.println(pairingData.id);
    Serial.print("Pairing request from: ");
    printMAC_t(mac_addr);
    Serial.println();
    Serial.println(pairingData.channel);
    if (pairingData.id > 0) {     // do not replay to server itself
      if (pairingData.msgType == PAIRING) { 
        pairingData.id = 0;       // 0 is server
        // Server is in AP_STA mode: peers need to send data to server soft AP MAC address 
        WiFi.softAPmacAddress(pairingData.macAddr);   
        pairingData.channel = chan_t;
        Serial.println("send response");
        esp_err_t result = esp_now_send(mac_addr, (uint8_t *) &pairingData, sizeof(pairingData));
        addPeer_t(mac_addr);
      }  
    }  
    break; 
  }

    if(len == sizeof(incomingReadings)) {
    memcpy(&incomingReadings, incomingDatas, sizeof(incomingReadings));
    // LOG("Data  incomingReadings");
    // LOGLN("RSSI:" + String(rssi_display));
    // // LOGLN(len);
    // incomingnetworkID = incomingReadings.networkID;
    // incomingnodeID = incomingReadings.nodeID;
    // incomingstatus = incomingReadings.status;
    // incomingCatagory = incomingReadings.category;
    // incominghumidity = incomingReadings.humidity;
    // incomingmbattery = incomingReadings.mbattery;
    // incomingbattery = incomingReadings.battery;
    // incomingReadings.RSSI = rssi_display;
    // IOT_DEVICE.updateDisplay();
    // }
    // if(len == sizeof(DataCommand)){// Control by Mesh
    //   memcpy(&DataCommand, incomingDatas, sizeof(DataCommand));
    //   if(DataCommand.Command == 1 && DataCommand.networkID == networkID && DataCommand.nodeID == nodeID){
    //     if(Debug){LOGLN("Data Controller");}

    // }
    // if(len == sizeof(pairingData)) {
    //   memcpy(&pairingData, incomingDatas, sizeof(pairingData));
    //   Serial.println("Net ID: " + String(pairingData.networkID));
    //   Serial.println("Node ID: " + String(pairingData.boardid));
    //   Serial.print("Pairing request from: ");
    //   printMAC(mac);
    //   Serial.println();
    //   Serial.println("chanel: " + String(pairingData.channel));
    //   if (pairingData.boardid > 0) {     // do not replay to server itself
    //     // if (pairingData.networkID == networkID) { 
    //       pairingData.boardid = 0;       // 0 is server
    //       // Server is in AP_STA mode: peers need to send data to server soft AP MAC address 
    //       WiFi.softAPmacAddress(pairingData.macAddr);   
    //       pairingData.channel = chan;
    //       Serial.println("send response");
    //       esp_err_t result = esp_now_send(broadcastAddress ,(uint8_t *) &pairingData, sizeof(pairingData));
    //       addPeer(mac);
    //     // }  
    //   }  
    // }
    if(len == sizeof(incomingReadings)) {
    memcpy(&incomingReadings, incomingDatas, sizeof(incomingReadings));
    LOG("Data  incomingReadings");
    LOGLN(" | RSSI:" + String(rssi_display));
    // LOGLN(len);
    }
    // Serial.println("DataIn");

    // RF_Serial.write((const uint8_t*)&incomingReadings,  sizeof(incomingReadings));
    // RF_Serial.write('\n');
    //Serial.println("update status");
    }
}