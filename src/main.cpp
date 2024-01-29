
// SIZE 50 mm,30 mm
// GAP  2 mm, 0 mm
// SPEED 4
// DENSITY 6
// DIRECTION 1
// SHIFT 0
// CLS
// TEXT 24,12,"3",0,1,1,"Sr.No.:S00008"
// TEXT 18,171,"3",0,2,2,"KL:-  3.06 kg"
// TEXT 20,56,"3",0,1,1,"Date:2024/01/23 23:56:41"
// TEXT 20,94,"3",0,1,1,"LINE:������������"
// TEXT 19,133,"3",0,1,1,"NV:"
// PRINT 1

#include <Arduino.h>

#include <HardwareSerial.h>  // if error you can also use SoftwareSerial, check first_view_arduino
#define LOG(string) {Serial.print(string);} 
#define LOGLN(string) {Serial.println(string);}
// You need to use max3485(for 3V3 logic lvl) or similar, if you have printer with rs232.
// If you use esp32 or other MCU with 3v3 logic level you can try with logic level converter.

// Check which pin pairs are responsible for communication on your board!
const byte rxPin = 10;
const byte txPin = 9;
const byte rxPin1 = 21;
const byte txPin1 = 20;
const int printerBaudrate = 9600;  // or 19200 usually

const byte LedPin = 15;  // if used
// const byte LedPin = 4;  // if used


//Define two Serial devices mapped to the two internal UARTs
HardwareSerial MySerial0(0);
HardwareSerial MySerial1(1);

String DataForm = "\
SIZE 50 mm,30 mm\n\
GAP  2 mm, 0 mm\n\
SPEED 4\n\
DENSITY 6\n\
DIRECTION 1\n\
SHIFT 0\n\
CLS\n\
TEXT 24,12,\"3\",0,1,1,\"Sr.No.:S00008\"\n\
TEXT 18,171,\"3\",0,2,2,\"KL:-  3.06 kg\"\n\
TEXT 20,56,\"3\",0,1,1,\"Date:2024/01/23 23:56:41\"\n\
TEXT 20,94,\"3\",0,1,1,\"LINE:01\"\n\
TEXT 19,133,\"3\",0,1,1,\"NV:VinhPhat\"\nPRINT 1\n";

String checkLine(String data, String split);
String dataStr[20];
String dataIn = "";
int strCount = 0;
void setup() {
  Serial.begin(printerBaudrate);                                            // monitor
  delay(2000);
  LOGLN("Start");
    // Configure MySerial0 on pins TX=6 and RX=7 (-1, -1 means use the default)
    MySerial0.begin(9600, SERIAL_8N1, rxPin, txPin);
    MySerial0.println("MySerial0");

    // And configure MySerial1 on pins RX=D9, TX=D10
    MySerial1.begin(printerBaudrate, SERIAL_8N1, rxPin, txPin);
    MySerial1.println("MySerial1");
    // Initialize pin 8 as an output
    pinMode(LedPin, OUTPUT);
  LOGLN("Done");
  for(int j = 0; j < DataForm.length(); j++) {
    // checkLine(DataForm,' ');
    if(DataForm[j] == '\n') {dataStr[strCount++]=dataIn; dataIn ="";}
    else{dataIn += DataForm[j];}
  }
  String keyword[] = {"","","","","","","",".:", "L:" , "e:", "E:", "V:", "T "};
  for(int j = 0; j < strCount; j++) {
    LOGLN(checkLine(dataStr[j],keyword[j]));
  }
  strCount =0;
}
bool New = 0;
    String DataIn = "";
    bool found = false;
void loop() {
  static unsigned long previousMillis = 0;
  const long interval = 1000;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // MySerial0.println("MySerial0");
    // Đảo trạng thái chân số 8
    digitalWrite(LedPin, !digitalRead(LedPin));
  }
  if(MySerial1.available())
    {
        char inchar = (char)MySerial1.read();
        DataIn += inchar;
        if(inchar == '\n') {dataStr[strCount++]=DataIn; DataIn ="";}
        else{DataIn += inchar;}
    }
}
String strs[20];
int StringCount = 0;
String checkLine(String data, String split){
 while (data.length() > 0)
  {
    int index = data.indexOf(split);
    if (index == -1 ) // No space found
    {
      strs[StringCount++] = data;
      break;
    }
    else
    {
      strs[StringCount++] = data.substring(1, index);
      data = data.substring(index+1);
    }
  }
  String StringData ="";
  if(StringCount > 0){StringData = strs[1];
  StringCount = 0;
  int index = data.indexOf("\"");
  StringData = StringData.substring(1, index);}
  return StringData;
}
#ifdef IOTDEVICES/*
#29/11/2023
#update: Auto reconnect mesh, send Radar data, firmware update form host
*/
#include <Arduino.h>
// #define Rad
// #define Sendmessage
// #define BLE
#define Websocket
// #define ESPNOW_Test
// #define Master
// #define Slave
// #define autoPair
#define SAVE_CHANNEL

#ifdef Websocket

#define Telegram
#define MESHWIFI
// #define MASTER

#define MESH_mode 0
#define WIFI_mode 1


#include "main.h"
main mainWS;
#include "Whatsapp.h"
WhatsApp wp;
#include "Telegram.h"
TelegramSmg telegrams;
#ifndef MASTER
#include "FirmwareUpdate.h"
UpdateFW updatefw;
#include "RadarFn.h"
RadarFunc RADAR;
#include "MeshNetwork.h"
MeshNet meshNet;
#include <Wire.h>
#include <INA3221.h>
// Set I2C address to 0x41 (A0 pin -> VCC)
INA3221 ina3221(INA3221_ADDR40_GND);
#define PRINT_DEC_POINTS 3       // decimal points to print

#endif//MASTER
// #include "WebsocketFn.h"
// WebsocketFunc websocketfunc;
// #include "FileSytem.h"
// FileSytemFunc filesytemMain;
#include "html.h"
// #if CONFIG_FREERTOS_UNICORE
// #define ARDUINO_RUNNING_CORE 0
// #else
// #define ARDUINO_RUNNING_CORE 1
// #endif

// define two tasks for Blink & AnalogRead
void TaskMain ( void *pvParameters );
void Task2( void *pvParameters );

#define LOGLN(string){Serial.println(string);}
#define LOG(string){Serial.print(string);}
#define LED 7
#define LED_FULL 1
#define LED_CHARG 2
// #define 2 8
#define             FRMW_VERSION         "1.2236"
#define             PRGM_VERSION         "1.0"

#define             COMMAND_GET_SETTINGS          100
#define             COMMAND_GET_INFO              101
#define             COMMAND_REBOOT                998
#define         NUM_SENSORS 100
// MainRadarData incomingReadings;
uint16_t         incomingDataGroup[sizeof(struct MainRadarDatas)];
MainRadarDatas mainData;
MainGatewayDatas mainGatewayDatas;
MainRadarDatas mainsensors[NUM_SENSORS];
// struct_setting meshNet.structsetting;

bool Mode = MESH_mode;
bool Debug = 1;
bool pairing = false;
unsigned long start;                // used to measure Pairing time
esp_now_peer_info_t slave;
int chan; 
bool            new_sensor_found;
int             sensors_saved;
#ifdef MESHWIFI


#endif//MESHWIFI

int countReadWS = 0;
const int motionSensor = Triger; // PIR Motion Sensor
bool motionDetected = false;
bool Run = false;
// Indicates when motion is detected
void IRAM_ATTR detectsMovement() {
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}

void VoltUpdate();

void main::GetData(uint16_t DistanceAct, uint8_t EnergyAct, uint16_t DistanceMov, uint8_t EnergyMov, uint16_t MaxStation, uint16_t MaxMoving, uint16_t inactivity){
  VoltUpdate();
  mainData.distanceActive = DistanceAct;
  mainData.energyActive = EnergyAct;
  mainData.distanceMoving = DistanceMov;
  mainData.energyMoving = EnergyMov;
  mainData.maxMoving = MaxMoving;
  mainData.maxStation = MaxStation;
  mainData.inactivity = inactivity;
}

WebSocketsServer    webSocket = WebSocketsServer(8000);

extern const char   html_template[];
extern const char   js_main[];
extern const char   css_main[];

WebServer    serverWS(80);
WiFiClient          wifiClient;

void readSensor(int num);
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) ;
void getSensorReadings(uint8_t num);
uint8_t NUM = 0;

String MainURL_fw_Bin = "VPlabc/ESP32IoTdevice/tree/main/.pioenvs/seeed_xiao_esp32c3/firmware.bin";
String MainURL_fw_Version = "VPlabc/ESP32IoTdevice/tree/main/.pioenvs/seeed_xiao_esp32c3/version.txt";





struct settings {
  char pversion[8];
  char ssid[32];
  char password[64];
  unsigned long baud;
  bool Mode;
} gateway_settings = {};

bool setup_mode = false;
int  connected_wifi_clients = 0;
// Timer variables
unsigned long SSlastTime = 3600000;
unsigned long SStimerDelay = 3600000;
unsigned long SSlastTime1 = 0;
unsigned long SStimerDelay1 = 10000;

bool RunMode = false;
bool WSrecive = false;

void WifiSetup();
void sendTarget(uint8_t num);

void startAP(bool new_device) {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Wireless Serial", "12345678");
  setup_mode = true;
}


void sendDeviceMessage(String msg) {

  // Serial.print(msg);
  // Serial.print("\r\n");
}
void sendRawDataOverSocket(const char * msgc, int len) {

  if ( strlen(msgc) == 0 or len == 0  ) return;
  StaticJsonDocument<400> console_data;
  console_data["type"] = "settings";
  console_data["content"] = msgc;
  char   b[400];
  size_t lenn = serializeJson(console_data, b);  // serialize to buffer
  webSocket.broadcastTXT(b, lenn);
}

void getDeviceMessages() {

  char buff[300];
  int state;
  int i;
  while (Serial.available() > 0) {
    i = Serial.readBytesUntil('\n', buff, sizeof(buff) - 1);
    buff[i] = '\0';
    sendRawDataOverSocket(buff, sizeof(buff));
  }
  i = 0;
}


void handleConfig() {

  if (serverWS.method() == HTTP_POST) {
    strncpy(gateway_settings.ssid,              serverWS.arg("ssid").c_str(),             sizeof(gateway_settings.ssid) );
    strncpy(gateway_settings.password,          serverWS.arg("password").c_str(),         sizeof(gateway_settings.password) );
    gateway_settings.ssid[serverWS.arg("ssid").length()]  = gateway_settings.password[serverWS.arg("password").length()]  =  0;  // string terminate
    strncpy(gateway_settings.pversion, PRGM_VERSION , sizeof(PRGM_VERSION) );
    EEPROM.put(0, gateway_settings);
    EEPROM.commit();
    String s = "<!DOCTYPE html><html lang='en'><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>";
    s += "<meta content='text/html;charset=utf-8' http-equiv='Content-Type'>";
    s += "<title>Wireless Serial</title>";
    s += "  <link rel='icon' type='image/png' sizes='16x16' href='data:image/x-icon;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAQAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIdUOc6IVTvpiFU654hVOuaIVTvkiVU74IdUOsqFUDaMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACIVDvli1c9/4pWPf+KVj3/ilY9/4pXPf+LVz3/jVk+/4lWPPaBSS5pAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgk064oVRPf+EUDz/hFA8/4RQPP98Rz3/fUk8/4VRPP+KVzz/jVk+/4ROMXwAAAAA/8s1///LNf//yzX//8s1///KM///yjL//8oy///KMv//yjL//8ky//C4Nf+wfjr/f0o8/4pWPP+LVz3/AAAAAP/FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xjP//8wy/76LOf+DTzz/jFg9/4ZTOLX/xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xzP/f0s8/4pWPP+IVTvj/8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8oy/6JwO/+HUzz/ilY88//FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///KMv+fbTv/h1Q8/4pWPPP/xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///GM///xTP/fUg8/4pWPP+JVjri/8Yz///GM///xjP//8Yz///GM///xjP//8Yz///GM///xjP//8Yz///HM///yzL/sX46/4RRPP+MWD3/h1I4sf/GMvT/xjL0/8Yy9P/FMvP4wDP99740//e+NP/3vjT/9740//O6NP/dpzf/m2k7/4JOPP+KVjz/iVY8/wAAAAAAAAAAAAAAAAAAAAAAAAAAd0A74X5IPf98Rzz/fEc8/3xHPP99SDz/gEw8/4dTPP+LVz3/jFg9/4FKL2sAAAAAAAAAAAAAAAAAAAAAAAAAAIhVOuaMWD7/i1c9/4tXPf+LVz3/i1c9/4xYPf+OWT//iFQ75XxDJ0gAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACHUji2iFQ6zohTOc2HVDnMh1M5yodUOsaGUjerf0YqVAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//8AAPAPAADwAwAA8AEAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAA8AEAAPADAADwDwAA//8AAA=='/>";
    s += "<meta content='utf-8' http-equiv='encoding'>";
    s += "<style>*,::after,::before{box-sizing:border-box}body{margin:0;font-family:system-ui,-apple-system,'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans',sans-serif,'Apple Color Emoji','Segoe UI Emoji','Segoe UI Symbol','Noto Color Emoji';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#fff;-webkit-text-size-adjust:100%;-webkit-tap-highlight-color:transparent}a{color:#0d6efd;text-decoration:underline}a:hover{color:#0a58ca}::-moz-focus-inner{padding:0;border-style:none}.container{width:100%;padding-right:var(--bs-gutter-x,.75rem);padding-left:var(--bs-gutter-x,.75rem);margin-right:auto;margin-left:auto}@media (min-width:576px){.container{max-width:540px}}@media (min-width:768px){.container{max-width:720px}}@media (min-width:992px){.container{max-width:960px}}@media (min-width:1200px){.container{max-width:1140px}}@media (min-width:1400px){.container{max-width:1320px}}.row{--bs-gutter-x:1.5rem;--bs-gutter-y:0;display:flex;flex-wrap:wrap;margin-top:calc(var(--bs-gutter-y) * -1);margin-right:calc(var(--bs-gutter-x)/ -2);margin-left:calc(var(--bs-gutter-x)/ -2)}.row>*{flex-shrink:0;width:100%;max-width:100%;padding-right:calc(var(--bs-gutter-x)/ 2);padding-left:calc(var(--bs-gutter-x)/ 2);margin-top:var(--bs-gutter-y)}@media (min-width:768px){.col-md-12{flex:0 0 auto;width:100%}}.d-flex{display:flex!important}.d-inline-flex{display:inline-flex!important}.border-bottom{border-bottom:1px solid #dee2e6!important}.flex-column{flex-direction:column!important}.justify-content-between{justify-content:space-between!important}.align-items-center{align-items:center!important}.mt-2{margin-top:.5rem!important}.mb-4{margin-bottom:1.5rem!important}.py-3{padding-top:1rem!important;padding-bottom:1rem!important}.pb-3{padding-bottom:1rem!important}.fs-4{font-size:calc(1.275rem + .3vw)!important}.text-decoration-none{text-decoration:none!important}.text-dark{color:#212529!important}@media (min-width:768px){.flex-md-row{flex-direction:row!important}.mt-md-0{margin-top:0!important}.ms-md-auto{margin-left:auto!important}}@media (min-width:1200px){.fs-4{font-size:1.5rem!important}}body{display:flex;flex-wrap:nowrap;height:100vh;height:-webkit-fill-available;overflow-x:auto}body>*{flex-shrink:0;min-height:-webkit-fill-available}a{color:#3d568a!important}.logo{padding-left:40px;background-size:40px 18px;background-image:url(data:image/svg+xml,%3Csvg width='469' height='197' viewBox='0 0 469 197' fill='none' xmlns='http://www.w3.org/2000/svg'%3E%3Crect width='469' height='197' fill='%23F2F2F2'/%3E%3Crect width='469' height='197' fill='white'/%3E%3Cpath d='M127.125 196C158.396 196 184.026 186.794 204.016 168.382C224.005 149.97 234 126.212 234 97.1091C234 68.0061 224.104 44.5455 204.312 26.7273C184.521 8.90909 158.792 0 127.125 0H63V196H127.125Z' fill='%233D568A'/%3E%3Cpath d='M0 156V40H127.837C148.103 40 164.197 45.1122 176.118 55.3365C188.039 65.5609 194 79.5962 194 97.4423C194 115.103 187.94 129.324 175.82 140.106C163.501 150.702 147.507 156 127.837 156H0Z' fill='%2332C5FF'/%3E%3Cpath d='M54.8659 136V96.0774L73.1454 119.714H77.9906L96.2701 96.0774V136H111.136V60H106.291L75.568 100.488L44.8452 60H40V136H54.8659Z' fill='white'/%3E%3Cpath d='M135.241 136V108.011C135.241 103.251 136.554 99.6253 139.181 97.1324C141.808 94.6394 145.416 93.393 150.004 93.393H154V79.9084C152.594 79.4551 150.966 79.2285 149.116 79.2285C142.826 79.2285 137.794 81.7215 134.02 86.7073V79.9084H120.256V136H135.241Z' fill='white'/%3E%3Cpath d='M284 98V1H244V98H284Z' fill='%233D568A'/%3E%3Cpath d='M284 197V98H244V197H284Z' fill='%2332C5FF'/%3E%3Cpath d='M401.669 119.703L469 1H424.805L381.5 78.7506L338.492 1H294L361.627 120L401.669 119.703Z' fill='%2332C5FF'/%3E%3Cpath d='M402 197V119.261L381.852 78L362 119.56V197H402Z' fill='%233D568A'/%3E%3C/svg%3E);background-repeat:no-repeat}</style>";
    s += "</head><body>";
    s += "<div class='container py-3'>";
    s += "<header>";
    s += "  <div class='d-flex flex-column flex-md-row align-items-center pb-3 mb-4 border-bottom' style='border-bottom:1px solid #32C5FF!important;'>";
    s += "    <a href='/' class='d-flex align-items-center text-dark text-decoration-none'><span class='fs-4 logo' style='height:19px;width:66px;'></span></a><span class='fs-4'>Wi-Fi Setup</span>";
    s += "    <nav class='d-inline-flex mt-2 mt-md-0 ms-md-auto'>";
    s += "    </nav>";
    s += "  </div>";
    s += "</header>";
    s += "<body><main>";
    s += "<div class='row justify-content-between'>";
    s += "<div class='col-md-12' style='text-align: center;'>";
    s += "<br /><h4 style='font-size:1.5rem;margin:0;'>Saved!</h4><br /><p>Please restart the device.</p>";
    s += "</div>";
    s += "</div></main></body></html>";
    serverWS.send(200, "text/html", s );

  } else {

    uint32_t realSize = 4000;//ESP.getFlashChipRealSize();
    uint32_t ideSize = 4000;//ESP.getFlashChipSize();
    String s = "<!DOCTYPE html><html lang='en'><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>";
    s += "<meta content='text/html;charset=utf-8' http-equiv='Content-Type'>";
    s += "<title>Wireless Serial</title>";
    s += "  <link rel='icon' type='image/png' sizes='16x16' href='data:image/x-icon;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAQAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIdUOc6IVTvpiFU654hVOuaIVTvkiVU74IdUOsqFUDaMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACIVDvli1c9/4pWPf+KVj3/ilY9/4pXPf+LVz3/jVk+/4lWPPaBSS5pAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgk064oVRPf+EUDz/hFA8/4RQPP98Rz3/fUk8/4VRPP+KVzz/jVk+/4ROMXwAAAAA/8s1///LNf//yzX//8s1///KM///yjL//8oy///KMv//yjL//8ky//C4Nf+wfjr/f0o8/4pWPP+LVz3/AAAAAP/FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xjP//8wy/76LOf+DTzz/jFg9/4ZTOLX/xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xzP/f0s8/4pWPP+IVTvj/8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8oy/6JwO/+HUzz/ilY88//FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///KMv+fbTv/h1Q8/4pWPPP/xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///FM///xTP//8Uz///GM///xTP/fUg8/4pWPP+JVjri/8Yz///GM///xjP//8Yz///GM///xjP//8Yz///GM///xjP//8Yz///HM///yzL/sX46/4RRPP+MWD3/h1I4sf/GMvT/xjL0/8Yy9P/FMvP4wDP99740//e+NP/3vjT/9740//O6NP/dpzf/m2k7/4JOPP+KVjz/iVY8/wAAAAAAAAAAAAAAAAAAAAAAAAAAd0A74X5IPf98Rzz/fEc8/3xHPP99SDz/gEw8/4dTPP+LVz3/jFg9/4FKL2sAAAAAAAAAAAAAAAAAAAAAAAAAAIhVOuaMWD7/i1c9/4tXPf+LVz3/i1c9/4xYPf+OWT//iFQ75XxDJ0gAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACHUji2iFQ6zohTOc2HVDnMh1M5yodUOsaGUjerf0YqVAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA//8AAPAPAADwAwAA8AEAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAA8AEAAPADAADwDwAA//8AAA=='/>";
    s += "<meta content='utf-8' http-equiv='encoding'>";
    s += "<style>*,::after,::before{box-sizing:border-box}body{margin:0;font-family:system-ui,-apple-system,'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans',sans-serif,'Apple Color Emoji','Segoe UI Emoji','Segoe UI Symbol','Noto Color Emoji';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#fff;-webkit-text-size-adjust:100%;-webkit-tap-highlight-color:transparent}a{color:#0d6efd;text-decoration:underline}a:hover{color:#0a58ca}label{display:inline-block}button{border-radius:0}button:focus:not(:focus-visible){outline:0}button,input{margin:0;font-family:inherit;font-size:inherit;line-height:inherit}button{text-transform:none}[type=submit],button{-webkit-appearance:button}::-moz-focus-inner{padding:0;border-style:none}.container{width:100%;padding-right:var(--bs-gutter-x,.75rem);padding-left:var(--bs-gutter-x,.75rem);margin-right:auto;margin-left:auto}@media (min-width:576px){.container{max-width:540px}}@media (min-width:768px){.container{max-width:720px}}@media (min-width:992px){.container{max-width:960px}}@media (min-width:1200px){.container{max-width:1140px}}@media (min-width:1400px){.container{max-width:1320px}}.row{--bs-gutter-x:1.5rem;--bs-gutter-y:0;display:flex;flex-wrap:wrap;margin-top:calc(var(--bs-gutter-y) * -1);margin-right:calc(var(--bs-gutter-x)/ -2);margin-left:calc(var(--bs-gutter-x)/ -2)}.row>*{flex-shrink:0;width:100%;max-width:100%;padding-right:calc(var(--bs-gutter-x)/ 2);padding-left:calc(var(--bs-gutter-x)/ 2);margin-top:var(--bs-gutter-y)}.col-12{flex:0 0 auto;width:100%}@media (min-width:768px){.col-md-12{flex:0 0 auto;width:100%}}.form-label{margin-bottom:.5rem}.form-control{display:block;width:100%;padding:.375rem .75rem;font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#fff;background-clip:padding-box;border:1px solid #ced4da;-webkit-appearance:none;-moz-appearance:none;appearance:none;border-radius:.25rem;transition:border-color .15s ease-in-out,box-shadow .15s ease-in-out}@media (prefers-reduced-motion:reduce){.form-control{transition:none}}.form-control:focus{color:#212529;background-color:#fff;border-color:#86b7fe;outline:0;box-shadow:0 0 0 .25rem rgba(13,110,253,.25)}.form-control::-moz-placeholder{color:#6c757d;opacity:1}.form-control::placeholder{color:#6c757d;opacity:1}.form-control:disabled{background-color:#e9ecef;opacity:1}.form-floating{position:relative}.btn{display:inline-block;font-weight:400;line-height:1.5;color:#212529;text-align:center;text-decoration:none;vertical-align:middle;cursor:pointer;-webkit-user-select:none;-moz-user-select:none;user-select:none;background-color:transparent;border:1px solid transparent;padding:.375rem .75rem;font-size:1rem;border-radius:.25rem;transition:color .15s ease-in-out,background-color .15s ease-in-out,border-color .15s ease-in-out,box-shadow .15s ease-in-out}@media (prefers-reduced-motion:reduce){.btn{transition:none}}.btn:hover{color:#212529}.btn:focus{outline:0;box-shadow:0 0 0 .25rem rgba(13,110,253,.25)}.btn:disabled{pointer-events:none;opacity:.65}.btn-primary{color:#fff;background-color:#0d6efd;border-color:#0d6efd}.btn-primary:hover{color:#fff;background-color:#0b5ed7;border-color:#0a58ca}.btn-primary:focus{color:#fff;background-color:#0b5ed7;border-color:#0a58ca;box-shadow:0 0 0 .25rem rgba(49,132,253,.5)}.btn-primary:active{color:#fff;background-color:#0a58ca;border-color:#0a53be}.btn-primary:active:focus{box-shadow:0 0 0 .25rem rgba(49,132,253,.5)}.btn-primary:disabled{color:#fff;background-color:#0d6efd;border-color:#0d6efd}.btn-lg{padding:.5rem 1rem;font-size:1.25rem;border-radius:.3rem}.d-flex{display:flex!important}.d-inline-flex{display:inline-flex!important}.border-bottom{border-bottom:1px solid #dee2e6!important}.flex-column{flex-direction:column!important}.justify-content-between{justify-content:space-between!important}.align-items-center{align-items:center!important}.mt-2{margin-top:.5rem!important}.mt-3{margin-top:1rem!important}.mb-4{margin-bottom:1.5rem!important}.py-3{padding-top:1rem!important;padding-bottom:1rem!important}.pb-3{padding-bottom:1rem!important}.fs-4{font-size:calc(1.275rem + .3vw)!important}.text-decoration-none{text-decoration:none!important}.text-dark{color:#212529!important}@media (min-width:768px){.flex-md-row{flex-direction:row!important}.mt-md-0{margin-top:0!important}.ms-md-auto{margin-left:auto!important}}@media (min-width:1200px){.fs-4{font-size:1.5rem!important}}body{display:flex;flex-wrap:nowrap;height:100vh;height:-webkit-fill-available;overflow-x:auto}body>*{flex-shrink:0;min-height:-webkit-fill-available}a{color:#3d568a!important}.btn-primary{background-color:#3d568a!important}.logo{padding-left:40px;background-size:40px 18px;background-image:url(\"data:image/svg+xml,%3Csvg width='469' height='197' viewBox='0 0 469 197' fill='none' xmlns='http://www.w3.org/2000/svg'%3E%3Crect width='469' height='197' fill='%23F2F2F2'/%3E%3Crect width='469' height='197' fill='white'/%3E%3Cpath d='M127.125 196C158.396 196 184.026 186.794 204.016 168.382C224.005 149.97 234 126.212 234 97.1091C234 68.0061 224.104 44.5455 204.312 26.7273C184.521 8.90909 158.792 0 127.125 0H63V196H127.125Z' fill='%233D568A'/%3E%3Cpath d='M0 156V40H127.837C148.103 40 164.197 45.1122 176.118 55.3365C188.039 65.5609 194 79.5962 194 97.4423C194 115.103 187.94 129.324 175.82 140.106C163.501 150.702 147.507 156 127.837 156H0Z' fill='%2332C5FF'/%3E%3Cpath d='M54.8659 136V96.0774L73.1454 119.714H77.9906L96.2701 96.0774V136H111.136V60H106.291L75.568 100.488L44.8452 60H40V136H54.8659Z' fill='white'/%3E%3Cpath d='M135.241 136V108.011C135.241 103.251 136.554 99.6253 139.181 97.1324C141.808 94.6394 145.416 93.393 150.004 93.393H154V79.9084C152.594 79.4551 150.966 79.2285 149.116 79.2285C142.826 79.2285 137.794 81.7215 134.02 86.7073V79.9084H120.256V136H135.241Z' fill='white'/%3E%3Cpath d='M284 98V1H244V98H284Z' fill='%233D568A'/%3E%3Cpath d='M284 197V98H244V197H284Z' fill='%2332C5FF'/%3E%3Cpath d='M401.669 119.703L469 1H424.805L381.5 78.7506L338.492 1H294L361.627 120L401.669 119.703Z' fill='%2332C5FF'/%3E%3Cpath d='M402 197V119.261L381.852 78L362 119.56V197H402Z' fill='%233D568A'/%3E%3C/svg%3E\");background-repeat:no-repeat}.alert-danger{position:relative;padding:1rem 1rem;margin-bottom:1rem;border-radius:.25rem;color:#842029;background-color:#f8d7da;border-color:#f5c2c7;}</style>";
    s += "</head><body>";
    s += "<div class='container py-3'>";
    s += "<header>";
    s += "  <div class='d-flex flex-column flex-md-row align-items-center pb-3 mb-4 border-bottom' style='border-bottom:1px solid #32C5FF!important;'>";
    s += "    <a href='/' class='d-flex align-items-center text-dark text-decoration-none'><span class='fs-4 logo' style='height:19px;width:66px;'></span></a><span class='fs-4'>Configuration</span>";
    s += "    <nav class='d-inline-flex mt-2 mt-md-0 ms-md-auto'>";
    s += "    </nav>";
    s += "  </div>";
    s += "</header>";
    s += "<body><main>";
    s += "<div class='row justify-content-between'>";
    if (ideSize != realSize) s += "<div class='alert-danger'>Your flash size (" + String(ideSize) + ") is configured incorrectly. It should be " + String(realSize) + ".</div>";
    s += "<div class='col-md-12'>";
    s += "<form action='/' method='post'>";
    s += "    <div class='col-12 mt-3'><label class='form-label'>Wi-Fi Name</label><input type='text' name='ssid' class='form-control' value='" + String(gateway_settings.ssid) + "'></div>";
    s += "    <div class='col-12 mt-3'><label class='form-label'>Password</label><input type='password' name='password' class='form-control' value='' autocomplete='off'></div>";
    s += "    <div class='form-floating'><br/><button class='btn btn-primary btn-lg' type='submit'>Save</button><br /><br /><br /></div>";
    s += " </form>";
    s += "</div>";
    s += "</div></main></body></html>";
    serverWS.send(200, "text/html", s );
  }
}

void handleMain() {

  if (setup_mode == true) {
    handleConfig();
    return;
  }
  serverWS.sendHeader("Cache-Control", "public, max-age=604800, immutable");
  serverWS.send_P(200, "text/html", html_template );
}

void handleDash() {

  serverWS.sendHeader("Cache-Control", "public, max-age=604800, immutable");
  serverWS.send_P(200, "text/html", index_html );
}

void handleUpdate() {

  long baud = serverWS.arg("baud").toInt();
  gateway_settings.baud = baud;
  EEPROM.put(0, gateway_settings);
  EEPROM.commit();
  serverWS.send(200, "text/html", "1" );
}

void handleMainJS() {
  serverWS.sendHeader("Cache-Control", "public, max-age=604800, immutable");
  serverWS.send_P(200, "text/javascript", js_main);
}

void handleMainCSS() {

  serverWS.sendHeader("Cache-Control", "public, max-age=604800, immutable");
  serverWS.send_P(200, "text/css", css_main);
}

void handleNotFound() {

  serverWS.send(404,   "text/html", "<html><body><p>404 Error</p></body></html>" );
}

void startServer() {

  serverWS.on("/",            handleMain);
  serverWS.on("/main.js",     handleMainJS);
  serverWS.on("/main.css",    handleMainCSS);
  serverWS.on("/update",      handleUpdate);
  serverWS.on("/dashboard",   handleDash);
  serverWS.onNotFound(handleNotFound);
  serverWS.on("/favicon.ico", handleNotFound);
  serverWS.begin();
}


void sendInfo(uint8_t num) {

  byte mac_address[6];
  WiFi.macAddress(mac_address);
  StaticJsonDocument<150> info_data;
  info_data["type"] = "info";
  info_data["version"] = gateway_settings.pversion;
  info_data["wifi"] = String(WiFi.RSSI());
  info_data["ip_address"] = WiFi.localIP().toString();
  info_data["mac_address"] = WiFi.macAddress();
  info_data["version"] = FRMW_VERSION;
  info_data["baud"] = gateway_settings.baud;
  char   b[150];
  size_t len = serializeJson(info_data, b);  // serialize to buffer
  if (num != 255) webSocket.sendTXT(num, b);
  else webSocket.broadcastTXT(b, len);
}

void sendTarget(uint8_t num = 0) {
  String target = "";
if(Mode == WIFI_mode){
  #ifndef MASTER
    target = "{\"mov0\":" + String(RADAR.targetmoving[0]) + ",\"stat0\":\"" + String(RADAR.targetstation[0]) + "\"\n";
    for(byte i = 1; i < 9 ; i++){
    target += ",\"mov"+ String(i) + "\":" + String(RADAR.targetmoving[i]) + ",\"stat"+ String(i) + "\":\"" + String(RADAR.targetstation[i]) + "\"\n";
    }
    target += "}";
    #else
    target = "{\"mov0\":" + String(settinggData.targetmoving[0]) + ",\"stat0\":\"" + String(settinggData.targetstation[0]) + "\"\n";
    for(byte i = 1; i < 9 ; i++){
    target += ",\"mov"+ String(i) + "\":" + String(settinggData.targetmoving[i]) + ",\"stat"+ String(i) + "\":\"" + String(settinggData.targetstation[i]) + "\"\n";
    }
    target += "}";
  #endif//MASTER
    Serial.println(target);
    char   ssb[1024];
    size_t sslen = target.length();
    strcpy(ssb, target.c_str());
    if (num != 255) webSocket.sendTXT(num, ssb);
    else webSocket.broadcastTXT(ssb, sslen);
  }
  if(Mode == MESH_mode){
      target = "{\"mov0\":" + String(RADAR.targetmoving[0]) + ",\"stat0\":\"" + String(RADAR.targetstation[0]) + "\"\n";
      for(byte i = 1; i < 9 ; i++){
      target += ",\"mov"+ String(i) + "\":" + String(RADAR.targetmoving[i]) + ",\"stat"+ String(i) + "\":\"" + String(RADAR.targetstation[i]) + "\"\n";
      }
      target += "}";
      Serial.println("Sen target \n" + target);
      meshNet.structsetting.id = BOARD_ID;
      meshNet.structsetting.msgType = SETTING;
      for(byte i = 0; i < 9 ; i++){meshNet.structsetting.targetstation[i] = RADAR.targetstation[i];}
      for(byte i = 0; i < 9 ; i++){meshNet.structsetting.targetmoving[i] = RADAR.targetmoving[i];}
      esp_err_t result = esp_now_send(meshNet.defserverAddress, (uint8_t *) &meshNet.structsetting, sizeof(meshNet.structsetting));

  }
}

void sendErrorOverSocket(uint8_t num, const char * msg) {

  StaticJsonDocument<100> error_message;
  error_message["type"] = "error";
  error_message["msg"] = msg;
  char   b[100];
  size_t len = serializeJson(error_message, b);  // serialize to buffer
  if (num != 255) webSocket.sendTXT(num, b);
  else webSocket.broadcastTXT(b, len);
}


void reboot() {

  ESP.restart();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  WSrecive = true;
  switch (type) {
    case WStype_TEXT: {
        StaticJsonDocument<300> socket_data;
        DeserializationError error = deserializeJson(socket_data, payload);
        JsonObject obj = socket_data.as<JsonObject>();
        NUM = num;
        // RadarData RDDataMain;
        if (error) {
          StaticJsonDocument<70> error_data;
          error_data["type"] = "error";
          error_data["content"] = "error parsing the last message";
          char   b[70]; // create temp buffer
          size_t len = serializeJson(error_data, b);  // serialize to buffer
          webSocket.sendTXT(num, b);
          break;
        }
        if ( socket_data["action"] == "command") {
          if ( socket_data["content"] == COMMAND_GET_INFO) { 
          sendInfo(num);sendTarget(num);RunMode = true;
#ifndef MASTER    
            RADAR.GetConfig();
#endif//MASTER
          }
          else if ( socket_data["content"] == COMMAND_REBOOT)               reboot();
          else if ( socket_data["content"] == COMMAND_GET_SETTINGS){}
          else {
            StaticJsonDocument<50> error_data;
            error_data["type"] = "error";
            error_data["content"] = "unknown command";
            char   b[50];
            size_t len = serializeJson(error_data, b);  // serialize to buffer
            webSocket.sendTXT(num, b);
          }
        } else if (socket_data["action"] == "console") {
          sendDeviceMessage(socket_data["content"]);
          if(socket_data["content"] == "M27 C"){
            // readSensor(num); 
            // getSensorReadings(num);
          }
        } else if (socket_data["action"] == "settings") {
          if ( socket_data["command"] == "update")  {
            if ( socket_data["key"] == "baud")  {
                long baud = obj[String("value")];
                gateway_settings.baud = baud;
                EEPROM.put(0, gateway_settings);
                EEPROM.commit();
                // Serial.println(obj["value"]);
                Serial.println(baud);
            }

            if ( socket_data["key"] == "sensitivity")  {
              int gates = socket_data["gate"];
              Serial.println("Saving settings: " + String(gates));

#ifndef MASTER
              if(RADAR.setGateSensitivityThresholdRD(socket_data["gate"], socket_data["motionSensitivity"], socket_data["stationarySensitivity"]))
              {
                Serial.println(F("OK, now restart to apply settings"));
              }
              else
              {
                Serial.println(F("failed"));
              }
#endif//MASTER 
            }
          }
          if ( socket_data["command"] == "commit")  {
              Serial.println("Setting Commited");
                StaticJsonDocument<400> request;
                request["type"] = "ack";
                request["content"] = "commited";
                char   commit[400];
                size_t sslen = serializeJson(request, commit);  // serialize to buffer
                if (num != 255) webSocket.sendTXT(num, commit);
                else webSocket.broadcastTXT(commit, sslen);
                if(WSrecive){
                  WSrecive = false;

                  #ifndef MASTER
                  RADAR.GetConfig();
                  #endif// MASTER
                  sendTarget(NUM);
                }
          }

          if ( socket_data["command"] == "done")  {
            // Serial.println("done");
          }
        }
      }
      break;
    default:
      break;
  }
   WSrecive = false;
}
void CheckAndUpdateFW() {
byte ret = updatefw.repeatedCall();
switch (ret) {
  case updatefw.HTTP_UPDATE_FAILED:
      LOGLN("Update Faild!!");
    break;
  case updatefw.HTTP_UPDATE_NO_UPDATES:
      LOGLN("No Update!!");
    break;
  case updatefw.HTTP_UPDATE_OK:
      LOGLN("Update OK!!");
      Mode = MESH_mode;
      EEPROM.put(0, gateway_settings);
      EEPROM.commit();
      delay(3000);ESP.restart();
    break;
  }
}
//setmaxvalues 8 8 3
// Get Sensor Readings and return JSON object
void getSensorReadings(uint8_t num){
  // Serial.println("WSrecive: " + String(WSrecive));
      // mainData.distanceActive = RADAR.distanceActive;
      // mainData.energyActive = RADAR.energyActive;
      // mainData.distanceMoving = RADAR.distanceMoving;
      // mainData.energyMoving = RADAR.energyMoving;
  VoltUpdate();
#ifndef MASTER
  if(WSrecive == false){//WSrecive = true;
  StaticJsonDocument<1024> readings;
  bool state = RADAR.read();
  readings["State"] = String(state);
  readings["MaxStation"] = String(mainData.maxStation);
  readings["MaxMoving"] = String(mainData.maxMoving);
  readings["inactivityTimer"] = String(mainData.inactivity);
  readings["stationaryDistance"] = String(mainData.distanceActive);
  readings["stationaryEnergy"] =  String(mainData.energyActive );
  readings["movingDistance"] = String(mainData.distanceMoving);
  readings["movingEnergy"] = String(mainData.energyMoving );
  readings["vmt"] = String(mainData.VMT );
  readings["amt"] = String(mainData.AMT );
  readings["vmain"] = String(mainData.VMain );
  readings["amain"] = String(mainData.AMain );
  char   ssb[1024];
  size_t sslen = serializeJson(readings, ssb);  // serialize to buffer
  if (num != 255) webSocket.sendTXT(num, ssb);
  else webSocket.broadcastTXT(ssb, sslen);
  }
#endif//MASTER
}
#ifdef MESHWIFI


unsigned int EncodeRespond(byte bytel,byte byteh)
{
    int ret = 0;
    byte byte_one = 0;
    byte byte_two = 0;
    byte_two = bytel;
    byte_one = byteh;
    ret = byte_one;
    ret = ret << 8;
    ret = ret | byte_two;
    return ret;
}
byte EncodeRespondByte(boolean a, boolean b, boolean c, boolean d, boolean e, boolean f, boolean g, boolean h)
    {
        byte byte_1 = 0;
        byte_1 = a;
        byte_1 = byte_1 << 1;
        byte_1 = byte_1| b;
        byte_1 = byte_1 << 1;
        byte_1 = byte_1 | c;
        byte_1 = byte_1 << 1;
        byte_1 = byte_1 | d;
        byte_1 = byte_1 << 1;
        byte_1 = byte_1 | e;
        byte_1 = byte_1 << 1;
        byte_1 = byte_1 | f;
        byte_1 = byte_1 << 1;
        byte_1 = byte_1 | g;
        byte_1 = byte_1 << 1;
        byte_1 = byte_1 | h;
        return byte_1;
    }

#endif//MESHWIFI
unsigned long mainpreviousMillis = 0;
bool onceUpdate = true; 

// long interval = 10000;
byte mainstep = 0;
void Meshloop() {
      meshNet.getParameters();
  if (meshNet.autoPairing() == PAIR_PAIRED) {
    if(onceUpdate){onceUpdate = false;RADAR.GetConfig();sendTarget();Serial.println("Send target");delay(1000);}
    unsigned long maincurrentMillis = millis();
    if (maincurrentMillis - mainpreviousMillis >= meshNet.interval) {
      // Save the last time a new reading was published
      mainpreviousMillis = maincurrentMillis;
      digitalWrite(LED, HIGH);
      // Serial.println("main interval: " + String(meshNet.interval));
    // Serial.println("Paired!!!"  );
      //Set values to send
      
      if(mainstep > 0) {
        if(sensors_saved  > 1) {
             meshNet.interval = 2000;
      //       mainData.rssi = sensors[mainstep-1].rssi;
      //       mainData.msgType = sensors[mainstep-1].msgType;
      //       mainData.id = sensors[mainstep-1].id;
      //       mainData.distanceActive = sensors[mainstep-1].distanceActive;
      //       mainData.energyActive = sensors[mainstep-1].energyActive;
      //       mainData.distanceMoving = sensors[mainstep-1].distanceMoving;
      //       esp_err_t result = esp_now_send(meshNet.dataTrans.defserverAddress, (uint8_t *) &mainData, sizeof(mainData));
      //     mainstep++;if(mainstep == meshNet.dataTrans.sensors_saved){mainstep = -1;}
        }
        else{mainstep = 0;}
      }
      if(mainstep == 0){
        bool state = RADAR.read();if(state == 1){meshNet.setInterval(1000);}else{meshNet.setInterval(10000);}
        bool Full = 0;if(mainData.VMain > 4.2){Full = 1;}else{Full = 0;}
        bool Charg = 0;if((mainData.VMT > 4 && mainData.VMain < 4.2) || mainData.AMT > 0.5){Charg = 1;}else{Charg = 0;}
        byte State = EncodeRespondByte(0, 0, 0, 0, 0, Full, Charg,state); 
        String RadarData = "";
        RadarData += "State " + String(state);
        RadarData += "| Charg " + String(Charg);
        RadarData += "| Full " + String(Full);
        RadarData += "| StateValue " + String(State);
        RadarData += "| MaxStation " + String(mainData.maxStation);
        RadarData += "| MaxMoving " + String(mainData.maxMoving);
        RadarData += "| inactivityTimer " + String(mainData.inactivity);
        RadarData += "| stationaryDistance "+ String(mainData.distanceActive);
        RadarData += "| stationaryEnergy " +  String(mainData.energyActive );
        RadarData += "| movingDistance " + String(mainData.distanceMoving);
        RadarData += "| movingEnergy " + String(mainData.energyMoving );
        RadarData += "| MT volt " + String(mainData.VMT);
        RadarData += "| MT current " + String(mainData.AMT);
        RadarData += "| Main volt " + String(mainData.VMain);
        RadarData += "| Main current " + String(mainData.AMain);
        Serial.println("Radar\n " + RadarData);
        mainData.rssi = meshNet.rssi_display;
        mainData.msgType = DATA; 
        mainData.id = BOARD_ID;
        mainData.state = State;
        // mainData.energyActive = mainData.energyActive;
        // mainData.distanceMoving = mainData.distanceMoving;
        // mainData.energyMoving= mainData.distanceMoving;
        esp_err_t result = esp_now_send(meshNet.defserverAddress, (uint8_t *) &mainData, sizeof(mainData));delay(500);
        //  meshNet.printMAC(meshNet.dataTrans.defserverAddress);
        
        // if(State & 0x01)
        ////////////////////////////////////////////////////////////////////////////////////////////////
        meshNet.currentMillisPing = millis();
        if(meshNet.currentMillisPing - meshNet.previousMillisPing > 15000) {
          // previousMillisPing = currentMillisPing;
          Serial.println("disconnect to gateway");
        }
        mainstep = 1;
      }
      if(mainstep < 0){mainstep = 0;}
      ////////////////////////////////////////////////////////////////////////////////////////////////
    }
  }
}



void saveSensorDatas(byte RSSI,byte ID,byte type,byte maxMov,byte maxStat,byte inacT,byte distanceSta,byte energySta,byte distanceMov,byte energyMov) {//Save Web
    //  bat = EncodeRespond(BatL,BatH);
    //  bat12 = EncodeRespond(Bat12L,Bat12H);
    //  int ID = EncodeRespond(IDl,IDh);
  new_sensor_found = false;
  
  for (int i = 0; i < sensors_saved; i++) {
    if (mainsensors[i].id > 0){
       mainsensors[i].msgType = type;
      mainsensors[i].maxMoving = maxMov;
      mainsensors[i].maxStation = maxStat;
      mainsensors[i].inactivity = inacT;
      mainsensors[i].distanceActive = distanceSta;
      mainsensors[i].energyActive = energySta;
      mainsensors[i].distanceMoving = distanceMov;
      mainsensors[i].energyMoving = energyMov;
      mainsensors[i].rssi = RSSI;
      //mainsensors[i].timestamp = time(nullptr);
      // mainsensors[i].timestamp = timeClient.getEpochTime();
      new_sensor_found = true;
    }
  }

  if ( new_sensor_found == false ) {
    mainsensors[sensors_saved].id = ID;
    mainsensors[sensors_saved].msgType = type;//msg1.category;
    mainsensors[sensors_saved].maxMoving = maxMov;
    mainsensors[sensors_saved].maxStation = maxStat;
    mainsensors[sensors_saved].inactivity = inacT;
    mainsensors[sensors_saved].distanceActive = distanceSta;
    mainsensors[sensors_saved].energyActive = energySta;
    mainsensors[sensors_saved].distanceMoving = distanceMov;
    mainsensors[sensors_saved].energyMoving = energyMov;
    mainsensors[sensors_saved].rssi = RSSI;
    //mainsensors[sensors_saved].timestamp = time(nullptr);
    // mainsensors[sensors_saved].timestamp = timeClient.getEpochTime();
    sensors_saved++;
    Serial.println("New sensor");
  }
}



void OnDataRecvs(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  Serial.print("Packet received from: ");
  meshNet.printMAC(mac_addr);
  Serial.println();
  Serial.print("data size = ");
  Serial.print(sizeof(incomingData));
  Serial.print("| data type = ");
  uint8_t type = incomingData[0];
  switch (type) {
  case DATA :      // we received data from server
    Serial.print("DATA");
    memcpy(&mainGatewayDatas, incomingData, sizeof(mainGatewayDatas));
    
    // Serial.print("| ID  = ");
    // Serial.println(mainGatewayDatas.id);
    // Serial.print("Setpoint distance Station = ");
    // Serial.println(mainGatewayDatas.distanceActive);
    // Serial.print("SetPoint energy Station = ");
    // Serial.println(mainGatewayDatas.energyActive);
    Serial.print(" | reading Id  = ");
    Serial.println(mainGatewayDatas.distanceMoving);
    if(mainData.id == 0){meshNet.previousMillisPing = meshNet.currentMillisPing;mainData.maxStation = mainGatewayDatas.distanceMoving;}
    else{
      saveSensorDatas(mainData.rssi, mainData.id,type,8 ,8 ,8,mainData.distanceActive, mainData.energyActive, mainData.distanceMoving,0);
    }
    // if (mainData.distanceMoving % 2 == 1){
      digitalWrite(LED, LOW);
    // } else { 
    // }
    break;

  case PAIRING:    // we received pairing data from server
    Serial.print("PAIRING");
    memcpy(&meshNet.structpairing, incomingData, sizeof(meshNet.structpairing));
    if (meshNet.structpairing.id == 0) {              // the message comes from server
      meshNet.printMAC(mac_addr);
      Serial.println();
      Serial.print("Pairing done for ");
      meshNet.printMAC(meshNet.structpairing.macAddr);
      Serial.print(" on channel " );
      Serial.print(meshNet.structpairing.channel);    // channel used by the server
      Serial.print(" in ");
      Serial.print(millis()-start);
      Serial.println("ms");
      meshNet.addPeer(meshNet.structpairing.macAddr, meshNet.structpairing.channel); // add the server  to the peer list 
      #ifdef SAVE_CHANNEL
        meshNet.lastChannel = meshNet.structpairing.channel;
        EEPROM.write(0, meshNet.structpairing.channel);
        EEPROM.commit();
      #endif  
      meshNet.SetParameter(PAIR_PAIRED);

    }
    break;
    case SETTING:  
    memcpy(&meshNet.structsetting, incomingData, sizeof(meshNet.structsetting));//messageSetting
      if(meshNet.structsetting.id == BOARD_ID){
        for(byte j = 0; j < 9; j++){
          Serial.println("Saving settings: " + String(j) +"|" + String(meshNet.structsetting.targetmoving[j]) + "|" + meshNet.structsetting.targetstation[j]);
          if(RADAR.setGateSensitivityThresholdRD(j, meshNet.structsetting.targetmoving[j], meshNet.structsetting.targetstation[j]))
          {
            Serial.println(F("OK, now restart to apply settings"));
          }
          else
          {
            Serial.println(F("failed"));
          }
          delay(10);
        }
        ESP.restart();
      }
    break;
    case COMMAND: 
        memcpy(&meshNet.structcommand, incomingData, sizeof(meshNet.structcommand));
        Serial.print("COMMAND");
        Serial.print("| ID  = ");
        Serial.print(meshNet.structcommand.id );
        Serial.print(" | Command:");
        Serial.println(meshNet.structcommand.command);

       if(meshNet.structcommand.id == BOARD_ID){
        if(meshNet.structcommand.command == ONWIFI){WifiSetup();Mode = WIFI_mode;}
        if(meshNet.structcommand.command == ONMESH){meshNet.setup();Mode = MESH_mode;}
        if(meshNet.structcommand.command == LOADCONFIG){RADAR.GetConfig();sendTarget();delay(1000);}
        if(meshNet.structcommand.command == RESET){reboot();}
        if(meshNet.structcommand.command == COMMAND){}
       }
    break;

  }  
}

void main::OnData() {
    // esp_now_register_send_cb(OnDataSents);
    esp_now_register_recv_cb(OnDataRecvs);
}

void VoltUpdate() { 
  // float current[3];
  // float current_compensated[3];
  // float voltage[3];

    mainData.AMT             = ina3221.getCurrent(INA3221_CH1);
    // current_compensated[0] = ina3221.getCurrentCompensated(INA3221_CH1);
    mainData.VMT             = ina3221.getVoltage(INA3221_CH1);

    // current[1]             = ina3221.getCurrent(INA3221_CH2);
    // current_compensated[1] = ina3221.getCurrentCompensated(INA3221_CH2);
    // voltage[1]             = ina3221.getVoltage(INA3221_CH2);

    mainData.AMain             = ina3221.getCurrent(INA3221_CH3);
    // current_compensated[2] = ina3221.getCurrentCompensated(INA3221_CH3);
    mainData.VMain             = ina3221.getVoltage(INA3221_CH3);


}

void WifiSetup(){
   WiFi.mode(WIFI_STA);
    WiFi.begin(gateway_settings.ssid, gateway_settings.password);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    byte tries = 1;
    if ( String(gateway_settings.ssid) != "" && String(gateway_settings.password) != ""  ) {
      while (WiFi.status() != WL_CONNECTED ) {
        if (tries++ > 9 ) {
          startAP(false);
          break;
        }
        delay(1000);
      }
    } else {
      startAP(true);
    }
    Serial.print("Server SOFT AP MAC Address:  ");
  Serial.println(WiFi.softAPmacAddress());

  chan = WiFi.channel();
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

    startServer();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    digitalWrite(LED, HIGH);
    webSocket.begin();
    // webSocket.onEvent(onWebSocketEvent);
    Run = true;
    #ifdef Telegram
    telegrams.setup();
    #endif//Telegram
    #ifdef WhatsApp_// Send Message to WhatsAPP
    // wp.sendMessage(main_.phoneNumber ,main_.apiKey ,"Sensor statup!");
    #endif//Whatsapp
}













void setup() {
  // delay(5000);
  // Start Serial port
  // Serial.begin(115200);
  Debug = true;
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);  // on
  pinMode(LED_FULL, INPUT);
  pinMode(LED_CHARG, INPUT);
  start = millis();

  EEPROM.begin(sizeof(struct settings) );
  delay(2000);
  EEPROM.get( 0, gateway_settings );
  if ( String(gateway_settings.pversion) != PRGM_VERSION )
    memset(&gateway_settings, 0, sizeof(settings));
  unsigned long serial_baud = gateway_settings.baud;
  Serial.println("Serial Baud:" + String(serial_baud));
  if ( !(serial_baud > 0 && serial_baud < 115200) ) serial_baud = 115200;
  Serial.begin( serial_baud );
  delay(500);
  Mode = gateway_settings.Mode;
  if( Mode > 2){Mode = MESH_mode;EEPROM.put(0, gateway_settings);EEPROM.commit();}
#ifndef MASTER
  RADAR.setup();
#endif//MASTER
  updatefw.SetLedPin(LED, LOW);
  updatefw.SetVersion("1.0");
  updatefw.SetUrlFirmware(MainURL_fw_Bin);
  updatefw.SetUrlVersion(MainURL_fw_Version);

if(Mode == WIFI_mode){
    Serial.begin(115200);
    Serial.println("WIFI mode");
    WifiSetup();
  }
  if(Mode == MESH_mode){
    #ifdef MESHWIFI
    Serial.begin(115200);
    Serial.println("MESH mode");
    meshNet.setup();
    // esp_now_register_send_cb(OnDataSents);
    // esp_now_register_recv_cb(OnDataRecvs);

    #endif//MESH_WIFI
  }
Serial.println("main interval: " + String(meshNet.interval));
  // websocketfunc.setup();
  // filesytemMain.setup();
// Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskMain
    ,  "TaskMain"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    Task2
    ,  "Task2"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
    /////////////////////////////////// INA3221 //////////////////////////////////
    
    Wire.begin(8,9);
    ina3221.begin();
    ina3221.reset();

    // Set shunt resistors to 10 mOhm for all channels
    ina3221.setShuntRes(10, 10, 10);

    // Set series filter resistors to 10 Ohm for all channels.
    // Series filter resistors introduce error to the current measurement.
    // The error can be estimated and depends on the resitor values and the bus
    // voltage.
    ina3221.setFilterRes(10, 10, 10);
//////////////////////////////////////////////////////////////////////////////

}
// 















void loop()
{
  if(Mode == WIFI_mode){
    webSocket.loop();
    serverWS.handleClient();
    CheckAndUpdateFW();
    // getDeviceMessages();
    if ( setup_mode && connected_wifi_clients != WiFi.softAPgetStationNum() ) connected_wifi_clients = WiFi.softAPgetStationNum();
    if ((millis() - SSlastTime) > SStimerDelay) {

        #ifndef MASTER
        getSensorReadings(NUM);
        #endif// MASTER
        telegrams.sendMessage("ping!!");
      SSlastTime = millis();
    }
    if ((millis() - SSlastTime1) > SStimerDelay1) {
        // getSensorReadings(NUM);
        CheckAndUpdateFW();
      SSlastTime1 = millis();
    }
    if(motionDetected){
      telegrams.sendMessage("Motion detected!!");
    #ifdef Telegram
    #endif//Telegram
    #ifdef WhatsApp_
      // Send Message to WhatsAPP
      wp.sendMessage(main_.phoneNumber ,main_.apiKey ,"Motion detected!!");
    #endif//Whatsapp
      Serial.println("Motion Detected");
      motionDetected = false;
    }
  }

  if(Mode == MESH_mode){
    #ifdef MESHWIFI
    Meshloop();//meshNet.loop();
    // Serial.println("MESH_loop");
    #endif//MESH_WIFI
  }

#ifndef MASTER
  RADAR.loop();
#endif//MASTER
    // websocketfunc.loop();
    // filesytemMain.loop();


}
/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskMain(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  // initialize digital 2 on pin 13 as an output.
  pinMode(LED, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    if(Mode == WIFI_mode){
      digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
      // refers to how many milliseconds the period between each ticks is, ie. 1ms.
      vTaskDelay(1000 / portTICK_PERIOD_MS );  // vTaskDelay wants ticks, not milliseconds
      digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
     }
    if(pairing == true){
      digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
      // refers to how many milliseconds the period between each ticks is, ie. 1ms.
      vTaskDelay(100 / portTICK_PERIOD_MS );  // vTaskDelay wants ticks, not milliseconds
      digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
     }
      vTaskDelay(100 / portTICK_PERIOD_MS); // 1 second delay
  }
}

void Task2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 100ms delay
  }
}

#endif//Websocket
#endif//
#ifdef autoPair
#ifdef Master
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/?s=esp-now
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based on JC Servaye example: https://github.com/Servayejc/esp_now_web_server/
*/

#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include <ArduinoJson.h>

#define LED 8

// Replace with your network credentials (STATION)
const char* ssid = "Hoang Vuong";
const char* password = "91919191";

esp_now_peer_info_t slave;
int chan; 

enum MessageType {PAIRING, DATA,};
MessageType MSmessageType;

int counter = 0;

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

typedef struct struct_pairing {       // new structure for pairing
    uint8_t msgType;
    uint8_t id;
    uint8_t macAddr[6];
    uint8_t channel;
} struct_pairing;

struct_message incomingReadings;
struct_message outgoingSetpoints;
struct_pairing MSpairingData;

AsyncWebServer server(80);
AsyncEventSource events("/events");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.temperature { color: #fd7e14; }
    .card.humidity { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #1</h4>
        <p><span class="reading"><span id="ds1"></span></span></p>
        <p class="packet">energy: <span id="es1"></span></p>
        <p><span class="reading"><span id="dm1"></span></span></p>        
        <p class="packet">energy: <span id="em1"></span></p>
        <p class="packet">State: <span id="state1"></span></p>
        <p class="packet">RSSI: <span id="rssi1"></span></p>
        <p class="packet">Reading ID: <span id="rh1"></span></p>
      </div>
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #2</h4>
        <p><span class="reading"><span id="ds2"></span></span></p>
        <p class="packet">energy: <span id="es2"></span></p>
        <p><span class="reading"><span id="dm2"></span></span></p>        
        <p class="packet">energy: <span id="em2"></span></p>
        <p class="packet">State: <span id="state2"></span></p>
        <p class="packet">RSSI: <span id="rssi2"></span></p>
        <p class="packet">Reading ID: <span id="rh2"></span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  var states = "Idle | None";
  if(obj.state == 0){states = "Idle | None";}
  if(obj.state == 1){states = "Active | None";}
  if(obj.state == 2){states = "Idle | Charge";}
  if(obj.state == 3){states = "Active | Charge";}
  if(obj.state == 4){states = "Idle | Full";}
  if(obj.state == 5){states = "Active | Full";}
  document.getElementById("ds"+obj.id).innerHTML = obj.distanceStat;
  document.getElementById("es"+obj.id).innerHTML = obj.energyStat;
  document.getElementById("dm"+obj.id).innerHTML = obj.distanceMov
  document.getElementById("em"+obj.id).innerHTML = obj.energyMov;
  document.getElementById("state"+obj.id).innerHTML = states;
  document.getElementById("rssi"+obj.id).innerHTML = obj.rssi;
  document.getElementById("rh"+obj.id).innerHTML = obj.readingId;
 }, false);
}
</script>
</body>
</html>)rawliteral";

void readDataToSend() {
  outgoingSetpoints.msgType = DATA;
  outgoingSetpoints.id = 0;
  outgoingSetpoints.distanceActive = random(0, 40);
  outgoingSetpoints.energyActive = random(0, 100);
  outgoingSetpoints.distanceMoving = counter++;
}


// ---------------------------- esp_ now -------------------------
void printMAC(const uint8_t * mac_addr){
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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success to " : "Delivery Fail to ");
  printMAC(mac_addr);
  Serial.println();
}

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  Serial.print(len);
  Serial.print(" bytes of data received from : ");
  printMAC(mac_addr);
  Serial.println();
  StaticJsonDocument<1000> root;
  String payload;
  uint8_t type = incomingData[0];       // first message byte is the type of message 
  switch (type) {
  case DATA :                           // the message is data type
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
    // create a JSON document with received data and send it by event to the web page
    root["id"] = incomingReadings.id;
    root["distanceStat"] = incomingReadings.distanceActive;
    root["energyStat"] = incomingReadings.energyActive;
    root["distanceMov"] = String(incomingReadings.distanceMoving);
    root["energyMov"] = String(incomingReadings.energyMoving);
    root["state"] = String(incomingReadings.state);
    root["rssi"] = String(incomingReadings.rssi);
    serializeJson(root, payload);
    Serial.print("event send :");
    serializeJson(root, Serial);
    events.send(payload.c_str(), "new_readings", millis());
    Serial.println();
    break;
  
  case PAIRING:                            // the message is a pairing request 
    memcpy(&MSpairingData, incomingData, sizeof(MSpairingData));
    Serial.println(MSpairingData.msgType);
    Serial.println(MSpairingData.id);
    Serial.print("Pairing request from: ");
    printMAC(mac_addr);
    Serial.println();
    Serial.println(MSpairingData.channel);
    if (MSpairingData.id > 0) {     // do not replay to server itself
      if (MSpairingData.msgType == PAIRING) { 
        MSpairingData.id = 0;       // 0 is server
        // Server is in AP_STA mode: peers need to send data to server soft AP MAC address 
        WiFi.softAPmacAddress(MSpairingData.macAddr);   
        MSpairingData.channel = chan;
        Serial.println("send response");
        esp_err_t result = esp_now_send(mac_addr, (uint8_t *) &MSpairingData, sizeof(MSpairingData));
        addPeer(mac_addr);
      }  
    }  
    break; 
  }
}

void initESP_NOW(){
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
} 

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  Serial.println();
  Serial.print("Server MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }

  Serial.print("Server SOFT AP MAC Address:  ");
  Serial.println(WiFi.softAPmacAddress());

  chan = WiFi.channel();
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  initESP_NOW();
  
  // Start Web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  

  // Events 
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  
  // start server
  server.begin();

}

void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    Serial.println("Event interval");
    events.send("ping",NULL,millis());
    lastEventTime = millis();
    readDataToSend();
    esp_now_send(NULL, (uint8_t *) &outgoingSetpoints, sizeof(outgoingSetpoints));
  }
}
#endif//Master
#ifdef Slave

#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <EEPROM.h>

// Set your Board and Server ID 
#define BOARD_ID 2
#define MAX_CHANNEL 11  // for North America // 13 in Europe
#define LED 8

uint8_t serverAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//Structure to send data
//Must match the receiver structure
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    uint8_t msgType;
    uint8_t rssi; 
    uint8_t id;
    uint16_t distanceMoving;
    uint8_t energyMoving;
    uint16_t  distanceActive;
    uint8_t energyActive;
    uint16_t  maxStation;
    uint16_t  maxMoving;
    uint16_t  inactivity;
} struct_message;

typedef struct struct_pairing {       // new structure for pairing
    uint8_t msgType;
    uint8_t id;
    uint8_t macAddr[6];
    uint8_t channel;
} struct_pairing;

//Create 2 struct_message 
struct_message mainData;  // data to send
struct_message inData;  // data received
struct_pairing pairingData;

#define         NUM_SENSORS 20
struct_message sensors[NUM_SENSORS];

enum PairingStatus {NOT_PAIRED, PAIR_REQUEST, PAIR_REQUESTED, PAIR_PAIRED,};
PairingStatus pairingStatus = NOT_PAIRED;

enum MessageType {PAIRING, DATA,};
MessageType messageType;

#ifdef SAVE_CHANNEL
  int lastChannel;
#endif  
int channel = 1;
 
// simulate temperature and humidity data
float t = 0;
float h = 0;

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;   // Stores last time temperature was published
unsigned long currentMillisPing = millis();
unsigned long previousMillisPing = 0;   // Stores last time temperature was published
long interval = 10000;        // Interval at which to publish sensor readings
unsigned long start;                // used to measure Pairing time
unsigned int readingId = 0;   

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

void saveSensorData(byte RSSI,byte ID,byte type,byte maxMov,byte maxStat,byte inacT,byte distanceSta,byte energySta,byte distanceMov,byte energyMov);
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

void addPeer(const uint8_t * mac_addr, uint8_t chan){
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
  memcpy(serverAddress, mac_addr, sizeof(uint8_t[6]));
}

void printMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
byte resent = 0;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status == ESP_NOW_SEND_FAIL){
    interval = 1000;
    resent++;if(resent > 10){
    resent = 0;Serial.println("fail 10 times");
    pairingStatus = PAIR_REQUEST; 
    }
  }else{interval = 10000;}
}

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  Serial.print("Packet received from: ");
  printMAC(mac_addr);
  Serial.println();
  Serial.print("data size = ");
  Serial.print(sizeof(incomingData));
  Serial.print("| data type = ");
  uint8_t type = incomingData[0];
  switch (type) {
  case DATA :      // we received data from server
    Serial.print("DATA");
    memcpy(&inData, incomingData, sizeof(inData));
    
    Serial.print("| ID  = ");
    Serial.println(inData.id);
    Serial.print("Setpoint temp = ");
    Serial.println(inData.distanceActive);
    Serial.print("SetPoint humidity = ");
    Serial.println(inData.energyActive);
    Serial.print("reading Id  = ");
    Serial.println(inData.distanceMoving);
    if(inData.id == 0){previousMillisPing = currentMillisPing;}
    else{
      saveSensorData(inData.rssi, inData.id,type,8 ,8 ,8,inData.distanceActive, inData.energyActive, inData.distanceMoving,0);
    }
    // if (inData.distanceMoving % 2 == 1){
      digitalWrite(LED, LOW);
    // } else { 
    // }
    break;

  case PAIRING:    // we received pairing data from server
    Serial.print("PAIRING");
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    if (pairingData.id == 0) {              // the message comes from server
      printMAC(mac_addr);
      Serial.println();
      Serial.print("Pairing done for ");
      printMAC(pairingData.macAddr);
      Serial.print(" on channel " );
      Serial.print(pairingData.channel);    // channel used by the server
      Serial.print(" in ");
      Serial.print(millis()-start);
      Serial.println("ms");
      addPeer(pairingData.macAddr, pairingData.channel); // add the server  to the peer list 
      #ifdef SAVE_CHANNEL
        lastChannel = pairingData.channel;
        EEPROM.write(0, pairingData.channel);
        EEPROM.commit();
      #endif  
      pairingStatus = PAIR_PAIRED;             // set the pairing status
    }
    break;
  }  
}
byte repair = 0;

PairingStatus autoPairing(){
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
    esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);
    // set callback routines
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
  
    // set pairing data to send to the server
    pairingData.msgType = PAIRING;
    pairingData.id = BOARD_ID;     
    pairingData.channel = channel;

    // add peer and send request
    addPeer(serverAddress, channel);
    esp_now_send(serverAddress, (uint8_t *) &pairingData, sizeof(pairingData));
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
bool            new_sensor_found;
int             sensors_saved;

void saveSensorData(byte RSSI,byte ID,byte type,byte maxMov,byte maxStat,byte inacT,byte distanceSta,byte energySta,byte distanceMov,byte energyMov) {//Save Web
    //  bat = EncodeRespond(BatL,BatH);
    //  bat12 = EncodeRespond(Bat12L,Bat12H);
    //  int ID = EncodeRespond(IDl,IDh);
  new_sensor_found = false;
  
  for (int i = 0; i < sensors_saved; i++) {
    if (sensors[i].id > 0){
       sensors[i].msgType = type;
      sensors[i].maxMoving = maxMov;
      sensors[i].maxStation = maxStat;
      sensors[i].inactivity = inacT;
      sensors[i].distanceActive = distanceSta;
      sensors[i].energyActive = energySta;
      sensors[i].distanceMoving = distanceMov;
      sensors[i].energyMoving = energyMov;
      sensors[i].rssi = RSSI;
      //sensors[i].timestamp = time(nullptr);
      // sensors[i].timestamp = timeClient.getEpochTime();
      new_sensor_found = true;
    }
  }

  if ( new_sensor_found == false ) {
    sensors[sensors_saved].id = ID;
    sensors[sensors_saved].msgType = type;//msg1.category;
    sensors[sensors_saved].maxMoving = maxMov;
    sensors[sensors_saved].maxStation = maxStat;
    sensors[sensors_saved].inactivity = inacT;
    sensors[sensors_saved].distanceActive = distanceSta;
    sensors[sensors_saved].energyActive = energySta;
    sensors[sensors_saved].distanceMoving = distanceMov;
    sensors[sensors_saved].energyMoving = energyMov;
    sensors[sensors_saved].rssi = RSSI;
    //sensors[sensors_saved].timestamp = time(nullptr);
    // sensors[sensors_saved].timestamp = timeClient.getEpochTime();
    sensors_saved++;
    Serial.println("New sensor");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(LED, OUTPUT);
  Serial.print("Client Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  start = millis();

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
void loop() {
  if (autoPairing() == PAIR_PAIRED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // Save the last time a new reading was published
      previousMillis = currentMillis;
      digitalWrite(LED, HIGH);
    // Serial.println("Paired!!!"  );
      //Set values to send
      if(step > 0) {
        if(sensors_saved  > 1) {
            interval = 1000;
            mainData.rssi = sensors[step-1].rssi;
            mainData.msgType = sensors[step-1].msgType;
            mainData.id = sensors[step-1].id;
            mainData.distanceActive = sensors[step-1].distanceActive;
            mainData.energyActive = sensors[step-1].energyActive;
            mainData.distanceMoving = sensors[step-1].distanceMoving;
            esp_err_t result = esp_now_send(serverAddress, (uint8_t *) &mainData, sizeof(mainData));
          step++;if(step == sensors_saved){step = -1;}
        }
        else{step = 0;}
      }
      if(step == 0){
        mainData.rssi = rssi_display;
        mainData.msgType = DATA;
        mainData.id = BOARD_ID;
        mainData.distanceActive = readDHTTemperature();
        mainData.energyActive = readDHTHumidity();
        mainData.distanceMoving = readingId++;
        esp_err_t result = esp_now_send(serverAddress, (uint8_t *) &mainData, sizeof(mainData));
        ////////////////////////////////////////////////////////////////////////////////////////////////
        currentMillisPing = millis();
        if(currentMillisPing - previousMillisPing > 15000) {
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
#endif//Slave
#endif//autoPair