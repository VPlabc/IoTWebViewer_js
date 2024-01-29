#ifndef Main_
#define Main_
#include "Arduino.h"

#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>

#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include <esp_now.h>
#include <esp_wifi.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 8
#define Triger 9
#endif

// #define 2 8
#define             FRMW_VERSION         "1.2236"
#define             PRGM_VERSION         "1.0"

#define             COMMAND_GET_SETTINGS          100
#define             COMMAND_GET_INFO              101
#define             COMMAND_REBOOT                998



typedef struct MainRadarDatas
{
    uint8_t msgType;
    uint8_t rssi;
    uint8_t id;
    uint8_t state;
    uint16_t distanceMoving;
    uint8_t energyMoving;
    uint16_t  distanceActive;
    uint8_t energyActive;
    uint16_t  maxStation;
    uint16_t  maxMoving;
    uint16_t  inactivity;
    float  VMT;
    float  AMT;
    float  VMain;
    float  AMain;
} MainRadarDatas;
typedef struct MainGatewayDatas
{
    uint8_t msgType;
    uint8_t rssi;
    uint8_t id;
    uint8_t state;
    uint16_t distanceMoving;
    uint8_t energyMoving;
    uint16_t  distanceActive;
    uint8_t energyActive;
} MainGatewayDatas;
// typedef struct struct_pairing {       // new structure for pairing
//     uint8_t msgType;
//     uint8_t id;
//     uint8_t macAddr[6];
//     uint8_t channel;
// } struct_pairing;

// typedef struct struct_setting {  
//     uint8_t msgType;
//     uint8_t id;
//     uint8_t targetmoving[9]; //
//     uint8_t targetstation[9]; //
// } struct_setting;

class main
{
public:


const String FirmwareVer="0.0";
String phoneNumber = "+84909961034";
String apiKey = "8322924";

const char* ssid = "Hoang Vuong";
const char* password = "91919191";
void GetData(uint16_t DistanceAct, uint8_t EnergyAct, uint16_t DistanceMov, uint8_t EnergyMov, uint16_t MaxStation, uint16_t MaxMoving, uint16_t inactivity);
void OnData();
private:
// void OnDataSents(const uint8_t *mac_addr, esp_now_send_status_t status) ;
// void OnDataRecvs(const uint8_t * mac_addr, const uint8_t *incomingData, int len) ;

void handleConfig();
void sendRawDataOverSocket(const char * msgc, int len);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void sendInfo(uint8_t num);
void sendErrorOverSocket(uint8_t num, const char * msg);
void reboot();
void handleMain();
void handleUpdate();
void handleMainJS();
void handleMainCSS();
void handleNotFound();

};
extern main MAIN;

#endif //Main_