#ifndef Whatsapp_
#define Whatsapp_
#include "Arduino.h"

class WhatsApp
{
public:
void sendMessage(String phoneNumber,String apiKey,String message);

//----------------------------------------------------------------

private:

};
extern WhatsApp WHATSAPP;


#endif //WhatsApp_