#include "Arduino.h"
#include "Telegram.h"
TelegramSmg telegram_;
// #include "main.h"
// main main_tlg;

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
// Initialize Telegram BOT
#define BOTtoken "6804711751:AAG_FRDxCg_oQe5O1NTRZ0k_8rOddijfamI"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "5750546335"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);



void TelegramSmg::setup()
{
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  bot.sendMessage(CHAT_ID, "Sensor statup", "");
}

void TelegramSmg::sendMessage(String message)
{
  bot.sendMessage(CHAT_ID, message, "");
}
