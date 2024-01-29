#ifndef MeshWifi
#define MeshWifi
// #include "config.h"
#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#include <espnow.h>
#else
#include "esp_wifi.h"
#include <esp_now.h>
#include <WiFi.h>
#endif//#ifdef ARDUINO_ARCH_ESP8266
#ifdef IOTDEVICE_UI
#include "AutoIT_IoT/IoTDevice.h"
#endif//#ifdef IOTDEVICE_UI
#ifdef LOOKLINE_UI
#include  "Lookline/Lookline.h"
#endif//LOOKLINE_UI
//---------------------------------------------- Mesh
// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#ifdef ARDUINO_ARCH_ESP8266

#else

#endif//

#ifdef ARDUINO_ARCH_ESP8266
// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
#else

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
#ifdef IOTDEVICE_UI  
  if(IOT_DEVICE.Debug){
#endif//IOTDEVICE_UI
#ifdef LOOKLINE_UI  
  if(Lookline_PROG.GetDebug()){
#endif//LOOKLINE_UI
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }

// }
#endif//#ifdef ARDUINO_ARCH_ESP8266

class MESH_WIFI{
  public:
    
  
  int check_protocol();
  // void MeshRecive(const uint8_t * mac, const uint8_t *incomingDatas, int len);
  void Mesh_Init();
  void printMAC(const uint8_t * mac_addr);
  bool addPeer(const uint8_t *peer_addr);
  void espnowSent();
  
  private:
};
extern MESH_WIFI mesh_w;


#endif//MeshWifi