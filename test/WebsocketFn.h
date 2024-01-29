#ifndef WebsocketFn_
#define WebsocketFn_

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>


class WebsocketFunc
{
public:
    void setup();
    void loop();
    bool read();
//----------------------------------------------------------------

private:

};
extern WebsocketFunc WSFunc;
#endif//WebsocketFn_