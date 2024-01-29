#ifndef FileSytem_
#define FileSytem_
#include "Arduino.h"


#include <SPIFFS.h>            // Built-in
#include <WiFi.h>              // Built-in
#include <ESPmDNS.h>           // Built-in
#include <AsyncTCP.h>          // https://github.com/me-no-dev/AsyncTCP
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include "esp_system.h"        // Built-in
#include "esp_spi_flash.h"     // Built-in 
#include "esp_wifi_types.h"    // Built-in
#include "esp_bt.h"            // Built-in
#define  FS SPIFFS             // In preparation for the introduction of LITTLFS see https://github.com/lorol/LITTLEFS replace SPIFFS with LITTLEFS




typedef struct
{
  String filename;
  String ftype;
  String fsize;
} fileinfo;

class FileSytemFunc
{
public:

    void setup();
    void loop();
    bool read();
//----------------------------------------------------------------

private:

};
extern FileSytemFunc FileSytemFuncs;


#endif //FileSytem_