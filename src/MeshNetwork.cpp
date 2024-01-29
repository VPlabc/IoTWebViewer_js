#include "MeshNetwork.h"
MeshNet meshNetwork;
#include "main.h"
main MeshMain;

int channel = 1;
 
// simulate temperature and humidity data
float t = 0;
float h = 0;

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;   // Stores last time temperature was published

unsigned int readingId = 0;   


 struct_message myData;  // data to send
 struct_message inData;  // data received
//  structpairing pairingData;
 MessageType messageType;
//  struct_setting messageSetting;

uint8_t defserverAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
PairingStatus pairingStatus = NOT_PAIRED;
void MeshNet::SetParameter(byte Status){
if(Status == PAIR_REQUEST){pairingStatus = PAIR_REQUEST;}
if(Status == PAIR_PAIRED){pairingStatus = PAIR_PAIRED;}
}


void MeshNet::setInterval(long time){
  interval = time;
}
void MeshNet::getParameters(){
DataTrans datTrans;
dataTrans.currentMillis = currentMillis; 
dataTrans.previousMillis = previousMillis; 
//  datTrans.currentMillisPing = currentMillisPing;
//  datTrans.previousMillisPing = previousMillisPing;
//  datTrans.rssi_display = rssi_display;
//  meshNet.interval = interval;
//  dataTrans.sensors_saved = sensors_saved;
//  for(int i = 0; i < 6 ; i++){dataTrans.defserverAddress[i] = defserverAddress[i];}
}
// PairingStatus autoPairing();

void promiscuous_rx_cbs(void *buf, wifi_promiscuous_pkt_type_t type) {
  // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
  if (type != WIFI_PKT_MGMT)
    return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

  int rssi = ppkt->rx_ctrl.rssi;
  meshNet.rssi_display = rssi;
}

// void saveSensorDatas(byte RSSI,byte ID,byte type,byte maxMov,byte maxStat,byte inacT,byte distanceSta,byte energySta,byte distanceMov,byte energyMov);
// simulate temperature reading
float readDHTTemperature() {
  t = random(0,40);
  return t;
}

// simulate humidity reading
float readDHTHumidity() {
  h = random(0,100);
  return h;
}

void MeshNet::addPeer(const uint8_t * mac_addr, uint8_t chan){
  esp_now_peer_info_t peer;
  ESP_ERROR_CHECK(esp_wifi_set_channel(chan ,WIFI_SECOND_CHAN_NONE));
  esp_now_del_peer(mac_addr);
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  peer.channel = chan;
  peer.encrypt = false;
  memcpy(peer.peer_addr, mac_addr, sizeof(uint8_t[6]));
  if (esp_now_add_peer(&peer) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(defserverAddress, mac_addr, sizeof(uint8_t[6]));
}

void MeshNet::printMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

byte resent = 0;bool onceOnsent = true;
void OnDataSents(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status == ESP_NOW_SEND_FAIL){
    meshNet.interval = 1000;
    resent++;if(resent > 5){onceOnsent = true;
    resent = 0;Serial.println("fail 5 times");
    meshNet.SetParameter(PAIR_REQUEST);
    }
  }else{resent = 0;if(onceOnsent){meshNet.interval = 10000;onceOnsent = false;}}
}

byte repair = 0;
PairingStatus MeshNet::autoPairing(){
  switch(pairingStatus) {
    case PAIR_REQUEST:
    Serial.print("Pairing request on channel "  );
    Serial.println(channel);

    // set WiFi channel   
    ESP_ERROR_CHECK(esp_wifi_set_channel(channel,  WIFI_SECOND_CHAN_NONE));
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
    }
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cbs);
    // set callback routines
    esp_now_register_send_cb(OnDataSents);
    // esp_now_register_recv_cb(MeshMain.OnDataRecvs());
    MeshMain.OnData();
    // set pairing data to send to the server
    structpairing.msgType = PAIRING;
    structpairing.id = BOARD_ID;     
    structpairing.channel = channel;

    // add peer and send request
    addPeer(defserverAddress, channel);
    esp_now_send(defserverAddress, (uint8_t *) &structpairing, sizeof(structpairing));
    previousMillis = millis();
    pairingStatus = PAIR_REQUESTED;
    break;

    case PAIR_REQUESTED:
    // time out to allow receiving response from server
    currentMillis = millis();
    if(currentMillis - previousMillis > 250) {
      previousMillis = currentMillis;
      // time out expired,  try next channel
      channel ++;digitalWrite(LED, !(digitalRead(LED)));
      if (channel > MAX_CHANNEL){
         channel = 1;
         repair++;if(repair > 10){ESP.restart();}
      }   
      pairingStatus = PAIR_REQUEST;
    }
    break;

    case PAIR_PAIRED:
      // nothing to do here 
    break;
  }
  return pairingStatus;
}  


void MeshNet::setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(LED, OUTPUT);
  Serial.print("Client Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();


  #ifdef SAVE_CHANNEL 
    EEPROM.begin(10);
    lastChannel = EEPROM.read(0);
    Serial.println(lastChannel);
    if (lastChannel >= 1 && lastChannel <= MAX_CHANNEL) {
      channel = lastChannel; 
    }
    Serial.println(channel);
  #endif  
  pairingStatus = PAIR_REQUEST;
}  
byte step = 0;
void MeshNet::loop() {
  if (autoPairing() == PAIR_PAIRED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // Save the last time a new reading was published
      previousMillis = currentMillis;
      digitalWrite(LED, HIGH);
    // Serial.println("Paired!!!"  );
      //Set values to send
      if(step > 0) {
        // if(sensors_saved  > 1) {
        //     interval = 1000;
        //     myData.rssi = sensors[step-1].rssi;
        //     myData.msgType = sensors[step-1].msgType;
        //     myData.id = sensors[step-1].id;
        //     myData.distanceActive = sensors[step-1].distanceActive;
        //     myData.energyActive = sensors[step-1].energyActive;
        //     myData.distanceMoving = sensors[step-1].distanceMoving;
        //     esp_err_t result = esp_now_send(defserverAddress, (uint8_t *) &myData, sizeof(myData));
        //   step++;if(step == sensors_saved){step = -1;}
        // }
        // else{
            step = 0;
            // }
      }
      if(step == 0){
        myData.rssi = rssi_display;
        myData.msgType = DATA;
        myData.id = BOARD_ID;
        myData.distanceActive = readDHTTemperature();
        myData.energyActive = readDHTHumidity();
        myData.distanceMoving = readingId++;
        esp_err_t result = esp_now_send(defserverAddress, (uint8_t *) &myData, sizeof(myData));
        ////////////////////////////////////////////////////////////////////////////////////////////////
        meshNet.currentMillisPing = millis();
        if(meshNet.currentMillisPing - meshNet.previousMillisPing > 15000) {
          // previousMillisPing = currentMillisPing;
          Serial.println("disconnect to gateway");
        }
        step = 1;
      }
      if(step < 0){step = 0;}
      ////////////////////////////////////////////////////////////////////////////////////////////////
    }
  }
}