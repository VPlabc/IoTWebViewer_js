
#ifndef Telegram_
#define Telegram_
#include "Arduino.h"

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>


class TelegramSmg
{
public:

void setup();
void loop();
void sendMessage(String message);
//----------------------------------------------------------------

private:

};
extern TelegramSmg TELEGRAM;


#endif //Telegram_