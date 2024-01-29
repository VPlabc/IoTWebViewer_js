#define ESPNOW_Test
#define Master
#define Slave


#ifdef BLE
#include <BleKeyboard.h> 
//Se the name of the bluetooth keyboard (that shows up in the bluetooth menu of your device)
BleKeyboard bleKeyboard("ESP32C3 Phone");
#define ESP32C3_board

#ifdef ESP32C3_board
const int buttonPin = 9;
const int LEDPin = 8;
#endif// seeed_xiao_esp


#ifdef ESP32DEV
const int buttonPin = 0;
const int LEDPin = 2;
#endif// esp32dev
//Set the old button state to be LOW/false; which means not pressed
boolean oldPinState = LOW;

void setup() {
  //Start the Serial communication (with the computer at 115200 bits per second)
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
  //Send this message to the computer
  Serial.println("Starting BLE work!");
  //Begin the BLE keyboard/start advertising the keyboard (so phones can find it)
  bleKeyboard.begin();
  //Make the button pin an INPUT_PULLDOWN pin, which means that it is normally LOW, untill it is pressed/ connected to the 3.3V
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(LEDPin, HIGH);
}
int count = 0;
bool once = true;
void loop() {

  if (bleKeyboard.isConnected()) {
    // if (once) {once = false;bleKeyboard.print("OK Run!!\n");}
    digitalWrite(LEDPin, LOW);
    //if the keyboard is connected to a device
    if (digitalRead(buttonPin) == LOW) {
      //If the button pin is pressed (since it is pulled down, it is pressed when it is high
      if (oldPinState == LOW) {
        //if the old Pin state was LOW and the button pin is HIGH than...
        //send the spacebar
        // bleKeyboard.print(" ");
        //or you can comment this one out, and it will send the newline character
        bleKeyboard.write(KEY_NUM_0);
        Serial.println("push");
        once = true;
      }
      oldPinState = HIGH;
    } else {
      oldPinState = LOW;
    }
  }
  else{
    if (once) {once = false;bleKeyboard.begin();Serial.println("Disconnected");}
  }
  delay(10);
  digitalWrite(LEDPin, HIGH);
  count++;if(count >= 100){count = 0;
  // bleKeyboard.print("OK Run!!\n");
  }
}
//
#endif//BLE

#ifdef Rada

/*
 * Example sketch to show using configuration commands on the LD2410.
 * 
 * This has been tested on the following platforms...
 * 
 * On ESP32, connect the LD2410 to GPIO pins 32&33
 * On ESP32S2, connect the LD2410 to GPIO pins 8&9
 * On ESP32C3, connect the LD2410 to GPIO pins 4&5
 * On Arduino Leonardo or other ATmega32u4 board connect the LD2410 to GPIO pins TX & RX hardware serial
 * 
 * The serial configuration for other boards will vary and you'll need to assign them yourself
 * 
 * There is no example for ESP8266 as it only has one usable UART and will not boot if the alternate UART pins are used for the radar.
 * 
 * For this sketch and other examples to be useful the board needs to have two usable UARTs.
 * 
 */


#include "RadarFn.h"
RadarFunc RADARFunct;

      #define RADAR_RX_PIN 4
      #define RADAR_TX_PIN 5
      
#include <ld2410.h>

ld2410 radar;
bool engineeringMode = false;
String command;
int count = 0;
void setup(void)
{
  delay(5000);
  Serial.begin(115200); //Feedback over Serial Monitor
  delay(500); //Give a while for Serial Monitor to wake up
  //radar.debug(Serial); //Uncomment to show debug information from the library on the Serial Monitor. By default this does not show sensor reads as they are very frequent.

    Serial1.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN); //UART for monitoring the radar

  delay(500);
  Serial.print(F("\nConnect LD2410 radar TX to GPIO:"));
  Serial.println(RADAR_RX_PIN);
  Serial.print(F("Connect LD2410 radar RX to GPIO:"));
  Serial.println(RADAR_TX_PIN);
  Serial.print(F("LD2410 radar sensor initialising: "));
  if(radar.begin(Serial1))
  {
    Serial.println(F("OK"));
    Serial.print(F("LD2410 firmware version: "));
    Serial.print(radar.firmware_major_version);
    Serial.print('.');
    Serial.print(radar.firmware_minor_version);
    Serial.print('.');
    Serial.println(radar.firmware_bugfix_version, HEX);
  }
  else
  {
    Serial.println(F("not connected"));
  }
  Serial.println(F("Supported commands\nread: read current values from the sensor\nreadconfig: read the configuration from the sensor\nsetmaxvalues <motion gate> <stationary gate> <inactivitytimer>\nsetsensitivity <gate> <motionsensitivity> <stationarysensitivity>\nenableengineeringmode: enable engineering mode\ndisableengineeringmode: disable engineering mode\nrestart: restart the sensor\nreadversion: read firmware version\nfactoryreset: factory reset the sensor\n"));
}

void loop()
{
  RADARFunct.RadarFunct();
}
#endif//Rada

#ifdef Sendmessage
/* 
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-send-messages-whatsapp/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#define Telegram
#define WhatsApp_

#include "main.h"
main main_;
#include "Whatsapp.h"
WhatsApp wp;
#include "Telegram.h"
TelegramSmg telegrams;
#include "RadarFn.h"
RadarFunc RADAR;
#include "WebsocketFn.h"
WebsocketFunc websocketfunc;
#include "FileSytem.h"
FileSytemFunc filesytemMain;
// #if CONFIG_FREERTOS_UNICORE
// #define ARDUINO_RUNNING_CORE 0
// #else
// #define ARDUINO_RUNNING_CORE 1
// #endif

// define two tasks for Blink & AnalogRead
void TaskMain ( void *pvParameters );
void Task2( void *pvParameters );


// MainRadarData incomingReadings;
uint16_t         incomingDataGroup[sizeof(struct MainRadarData)];
MainRadarData mainData;


int countRead = 0;
const int motionSensor = Triger; // PIR Motion Sensor
bool motionDetected = false;
bool Run = false;
// Indicates when motion is detected
void IRAM_ATTR detectsMovement() {
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}

void main::GetData(uint16_t DistanceAct, uint8_t EnergyAct, uint16_t DistanceMov, uint8_t EnergyMov){
  
  mainData.distanceActive = DistanceAct;
  mainData.energyActive = EnergyAct;
  mainData.distanceMoving = DistanceMov;
  mainData.energyMoving = EnergyMov;
}

void setup() {
  Serial.begin(9600);

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


  WiFi.begin(main_.ssid, main_.password);

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);



  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Run = true;
  #ifdef Telegram
  // telegrams.setup();
  #endif//Telegram
  #ifdef WhatsApp_// Send Message to WhatsAPP
  // wp.sendMessage(main_.phoneNumber ,main_.apiKey ,"Sensor statup!");
  #endif//Whatsapp
  RADAR.setup();
  // websocketfunc.setup();
  // filesytemMain.setup();
}

void loop() {
    RADAR.loop();
    // websocketfunc.loop();
    // filesytemMain.loop();
    #ifdef Telegram
    if(motionDetected){
      telegrams.sendMessage("Motion detected!!");
    #endif//Telegram
    #ifdef WhatsApp_
      // Send Message to WhatsAPP
      wp.sendMessage(main_.phoneNumber ,main_.apiKey ,"Motion detected!!");
    #endif//Whatsapp
      Serial.println("Motion Detected");
      motionDetected = false;
    }
    countRead++;
    if(countRead > 1000000){
      countRead = 0;
      if(Run){
      bool state = RADAR.read();
      mainData.distanceActive = RADAR.distanceActive;
      mainData.energyActive = RADAR.energyActive;
      mainData.distanceMoving = RADAR.distanceMoving;
      mainData.energyMoving = RADAR.energyMoving;
      String msg = "State: " + String(state) + " \n distAct:" + String(mainData.distanceActive) + " | energy:" + String(mainData.energyActive) + "\n";
            msg += "distMoving: " + String(mainData.distanceMoving) + " | energy:" + String(mainData.energyMoving) + "\n";
      Serial.println(msg);
      }
    }
}


/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskMain(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  // initialize digital 2 on pin 13 as an output.
  pinMode(2, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
    // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
    // refers to how many milliseconds the period between each ticks is, ie. 1ms.
    vTaskDelay(1000 / portTICK_PERIOD_MS );  // vTaskDelay wants ticks, not milliseconds
    digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
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

#endif//Sendmessage





#ifdef ESPNOW_Test
#ifdef Master

#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include <ArduinoJson.h>

// Replace with your network credentials (STATION)
const char* ssid = "Hoang Vuong";
const char* password = "91919191";

esp_now_peer_info_t slave_t;
int chan_t; 

enum MessageType {PAIRING, DATA,};
MessageType messageType;

int counter = 0;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  uint8_t msgType;
  uint8_t id;
  float temp;
  float hum;
  unsigned int readingId;
} struct_message;

typedef struct struct_pairing {       // new structure for pairing
    uint8_t msgType;
    uint8_t id;
    uint8_t macAddr[6];
    uint8_t channel;
} struct_pairing;

struct_message incomingReadings_main;
struct_message outgoingSetpoints;
struct_pairing pairingData;

AsyncWebServer server_main(80);
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
  <div class="row  p-4">
            <div class="col-sm-6 col-md-6 mb-5">
                <div class="card">
                    <h1 class="text-primary stationaryDistance_group" style="align-self: center"> Stationary</h1>  
                  <div class=" d-flex"><span class="text-muted">distance:</span><h2 style="align-self: center" class="reading"><span id="StationaryDistance"></span> cm</h2></div>   
                  <div class=" d-flex"><span class="text-muted">energy:</span><p  style="align-self: center" class="reading"><span id="StationaryEnergy"></span></p></div>
                 
                </div>
            </div>

            <div class="col-sm-6 col-md-6 mb-5">
                <div class="card">
                    <h1 class="text-primary stationaryDistance_group" style="align-self: center"> Moving</h1>
                    <div class=" d-flex"><span class="text-muted">distance:</span><h2 class="reading"><span id="MovingDistance"></span> cm</h2></div>   
                    <div class=" d-flex"><span class="text-muted">energy:</span><p class="reading"><span id="MovingEnergy"></span></p></div>
                   
                </div>
            </div>   
         </div>  
    </div>
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #1 - TEMPERATURE</h4><p><span class="reading"><span id="t1"></span> &deg;C</span></p><p class="packet">Reading ID: <span id="rt1"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> BOARD #1 - HUMIDITY</h4><p><span class="reading"><span id="h1"></span> &percnt;</span></p><p class="packet">Reading ID: <span id="rh1"></span></p>
      </div>
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> BOARD #2 - TEMPERATURE</h4><p><span class="reading"><span id="t2"></span> &deg;C</span></p><p class="packet">Reading ID: <span id="rt2"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> BOARD #2 - HUMIDITY</h4><p><span class="reading"><span id="h2"></span> &percnt;</span></p><p class="packet">Reading ID: <span id="rh2"></span></p>
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
  document.getElementById("t"+obj.id).innerHTML = obj.temperature.toFixed(2);
  document.getElementById("h"+obj.id).innerHTML = obj.humidity.toFixed(2);
  document.getElementById("rt"+obj.id).innerHTML = obj.readingId;
  document.getElementById("rh"+obj.id).innerHTML = obj.readingId;
 }, false);
}
</script>
</body>
</html>)rawliteral";

void readDataToSend() {
  outgoingSetpoints.msgType = DATA;
  outgoingSetpoints.id = 0;
  outgoingSetpoints.temp = random(0, 40);
  outgoingSetpoints.hum = random(0, 100);
  outgoingSetpoints.readingId = counter++;
}


// ---------------------------- esp_ now -------------------------
void printMAC_t(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

bool addPeer_t(const uint8_t *peer_addr) {      // add pairing
  memset(&slave_t, 0, sizeof(slave_t));
  const esp_now_peer_info_t *peer = &slave_t;
  memcpy(slave_t.peer_addr, peer_addr, 6);
  
  slave_t.channel = chan_t; // pick a chan_tnel
  slave_t.encrypt = 0; // no encryption
  // check if the peer exists
  bool exists = esp_now_is_peer_exist(slave_t.peer_addr);
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
void OnDataSent_t(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success to " : "Delivery Fail to ");
  printMAC_t(mac_addr);
  Serial.println();
}

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  Serial.print(len);
  Serial.print(" bytes of data received from : ");
  printMAC_t(mac_addr);
  Serial.println();
  StaticJsonDocument<1000> root;
  String payload;
  uint8_t type = incomingData[0];       // first message byte is the type of message 
  switch (type) {
  case DATA :                           // the message is data type
    memcpy(&incomingReadings_main, incomingData, sizeof(incomingReadings_main));
    // create a JSON document with received data and send it by event to the web page
    root["id"] = incomingReadings_main.id;
    root["temperature"] = incomingReadings_main.temp;
    root["humidity"] = incomingReadings_main.hum;
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
}

void initESP_NOW(){
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    esp_now_register_send_cb(OnDataSent_t);
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

  chan_t = WiFi.channel();
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  initESP_NOW();
  
  // Start Web server
  server_main.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
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
  server_main.addHandler(&events);
  
  // start server
  server_main.begin();

}

void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping",NULL,millis());
    lastEventTime = millis();
    readDataToSend();
    esp_now_send(NULL, (uint8_t *) &outgoingSetpoints, sizeof(outgoingSetpoints));
  }
}
#endif//Master

#ifdef Slave
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/?s=esp-now
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based on JC Servaye example: https://github.com/Servayejc/esp_now_sender/
*/

#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <EEPROM.h>

// Set your Board and Server ID 
#define BOARD_ID 1
#define MAX_CHANNEL 11  // for North America // 13 in Europe

uint8_t serverAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//Structure to send data
//Must match the receiver structure
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  uint8_t msgType;
  uint8_t id;
  float temp;
  float hum;
  unsigned int readingId;
} struct_message;

typedef struct struct_pairing {       // new structure for pairing
    uint8_t msgType;
    uint8_t id;
    uint8_t macAddr[6];
    uint8_t chan_tnel;
} struct_pairing;

//Create 2 struct_message 
struct_message myData;  // data to send
struct_message inData;  // data received
struct_pairing pairingData;

enum PairingStatus {NOT_PAIRED, PAIR_REQUEST, PAIR_REQUESTED, PAIR_PAIRED,};
PairingStatus pairingStatus = NOT_PAIRED;

enum MessageType {PAIRING, DATA,};
MessageType messageType;

#ifdef SAVE_CHANNEL
  int lastChannel;
#endif  
int chan_tnel = 1;
 
// simulate temperature and humidity data
float t = 0;
float h = 0;

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings
unsigned long start_main;                // used to measure Pairing time
unsigned int readingId = 0;   

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

void addPeer_t(const uint8_t * mac_addr, uint8_t chan_t){
  esp_now_peer_info_t peer;
  ESP_ERROR_CHECK(esp_wifi_set_channel(chan_t ,WIFI_SECOND_CHAN_NONE));
  esp_now_del_peer(mac_addr);
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  peer.channel = chan_t;
  peer.encrypt = false;
  memcpy(peer.peer_addr, mac_addr, sizeof(uint8_t[6]));
  if (esp_now_add_peer(&peer) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(serverAddress, mac_addr, sizeof(uint8_t[6]));
}

void printMAC_t(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

void OnDataSent_t(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  Serial.print("Packet received from: ");
  printMAC_t(mac_addr);
  Serial.println();
  Serial.print("data size = ");
  Serial.println(sizeof(incomingData));
  uint8_t type = incomingData[0];
  switch (type) {
  case DATA :      // we received data from server
    memcpy(&inData, incomingData, sizeof(inData));
    Serial.print("ID  = ");
    Serial.println(inData.id);
    Serial.print("Setpoint temp = ");
    Serial.println(inData.temp);
    Serial.print("SetPoint humidity = ");
    Serial.println(inData.hum);
    Serial.print("reading Id  = ");
    Serial.println(inData.readingId);

    if (inData.readingId % 2 == 1){
      digitalWrite(2, LOW);
    } else { 
      digitalWrite(2, HIGH);
    }
    break;

  case PAIRING:    // we received pairing data from server
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    if (pairingData.id == 0) {              // the message comes from server
      printMAC_t(mac_addr);
      Serial.print("Pairing done for ");
      printMAC_t(pairingData.macAddr);
      Serial.print(" on chan_tnel " );
      Serial.print(pairingData.chan_tnel);    // chan_tnel used by the server
      Serial.print(" in ");
      Serial.print(millis()-start_main);
      Serial.println("ms");
      addPeer_t(pairingData.macAddr, pairingData.chan_tnel); // add the server  to the peer list 
      #ifdef SAVE_CHANNEL
        lastChannel = pairingData.chan_tnel;
        EEPROM.write(0, pairingData.chan_tnel);
        EEPROM.commit();
      #endif  
      pairingStatus = PAIR_PAIRED;             // set the pairing status
    }
    break;
  }  
}

PairingStatus autoPairing(){
  switch(pairingStatus) {
    case PAIR_REQUEST:
    Serial.print("Pairing request on chan_tnel "  );
    Serial.println(chan_tnel);

    // set WiFi chan_tnel   
    ESP_ERROR_CHECK(esp_wifi_set_channel(chan_tnel,  WIFI_SECOND_CHAN_NONE));
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
    }

    // set callback routines
    esp_now_register_send_cb(OnDataSent_t);
    esp_now_register_recv_cb(OnDataRecv);
  
    // set pairing data to send to the server
    pairingData.msgType = PAIRING;
    pairingData.id = BOARD_ID;     
    pairingData.chan_tnel = chan_tnel;

    // add peer and send request
    addPeer_t(serverAddress, chan_tnel);
    esp_now_send(serverAddress, (uint8_t *) &pairingData, sizeof(pairingData));
    previousMillis = millis();
    pairingStatus = PAIR_REQUESTED;
    break;

    case PAIR_REQUESTED:
    // time out to allow receiving response from server
    currentMillis = millis();
    if(currentMillis - previousMillis > 250) {
      previousMillis = currentMillis;
      // time out expired,  try next chan_tnel
      chan_tnel ++;
      if (chan_tnel > MAX_CHANNEL){
         chan_tnel = 1;
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

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(2, OUTPUT);
  Serial.print("Client Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  start_main = millis();

  #ifdef SAVE_CHANNEL 
    EEPROM.begin(10);
    lastChannel = EEPROM.read(0);
    Serial.println(lastChannel);
    if (lastChannel >= 1 && lastChannel <= MAX_CHANNEL) {
      chan_tnel = lastChannel; 
    }
    Serial.println(chan_tnel);
  #endif  
  pairingStatus = PAIR_REQUEST;
}  

void loop() {
  if (autoPairing() == PAIR_PAIRED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // Save the last time a new reading was published
      previousMillis = currentMillis;
      //Set values to send
      myData.msgType = DATA;
      myData.id = BOARD_ID;
      myData.temp = readDHTTemperature();
      myData.hum = readDHTHumidity();
      myData.readingId = readingId++;
      esp_err_t result = esp_now_send(serverAddress, (uint8_t *) &myData, sizeof(myData));
    }
  }
}

#endif//Slave
#endif//ESPNOW_Test