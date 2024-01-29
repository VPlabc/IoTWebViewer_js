#ifndef MeshNet_
#define MeshNet_

#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <EEPROM.h>

// Set your Board and Server ID 
#define BOARD_ID 1
#define MAX_CHANNEL 11  // for North America // 13 in Europe
#define LED 8
#define SAVE_CHANNEL





//Structure to send data
//Must match the receiver structure
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
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
} struct_message;



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
//Create 2 struct_message 



enum PairingStatus {NOT_PAIRED, PAIR_REQUEST, PAIR_REQUESTED, PAIR_PAIRED,};

enum MessageType {PAIRING, DATA, SETTING, COMMAND};
enum CommandType {ONWIFI, ONMESH, RESET, LOADCONFIG};


class MeshNet
{
public:

unsigned long currentMillisPing = millis();
unsigned long previousMillisPing = 0;   // Stores last time temperature was published
int rssi_display;
long interval = 10000;        // Interval at which to publish sensor readings
uint8_t defserverAddress[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


#ifdef SAVE_CHANNEL
  int lastChannel;
#endif  



typedef struct struct_command {  
    uint8_t msgType;
    uint8_t id;
    uint8_t command;
} ;
struct_command structcommand;

typedef struct struct_setting {  
    uint8_t msgType;
    uint8_t id;
    uint8_t targetmoving[9]; //
    uint8_t targetstation[9]; //
} ;
struct_setting structsetting;

typedef struct struct_pairing {       // new structure for pairing
    uint8_t msgType;
    uint8_t id;
    uint8_t macAddr[6];
    uint8_t channel;
};
struct_pairing structpairing;
typedef struct DataTrans {
unsigned long currentMillis = millis();
unsigned long previousMillis = 0; 
// unsigned long currentMillisPing = 0;
// unsigned long previousMillisPing = 0;  
// int rssi_display;
// long interval = 10000;        // Interval at which to publish sensor readings
};
DataTrans dataTrans;
void setup();
void loop();
void addPeer(const uint8_t * mac_addr, uint8_t chan);
PairingStatus autoPairing();
void getParameters();
void SetParameter(byte Status);
void printMAC(const uint8_t * mac_addr);
void setInterval(long time);
private:





};
extern MeshNet meshNet;
#endif//MeshNet_