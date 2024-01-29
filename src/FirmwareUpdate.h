#ifndef UpdateFW_
#define UpdateFW_
#include <Arduino.h>

#define ServerUpdateFW
class UpdateFW
{
public:

enum HTTPUpdateResult {
    HTTP_UPDATE_FAILED,
    HTTP_UPDATE_NO_UPDATES,
    HTTP_UPDATE_OK
};
typedef HTTPUpdateResult httpUpdate_return; // backward compatibility


    String versionDetec ="";
    void SetUrlVersion(String input);
    void SetUrlFirmware(String input);
    void SetVersion(String input);
    void SetLedPin(byte Pin ,byte stateOn);
    byte repeatedCall();
    void ShowMess(String txt);
    byte FirmwareUpdate();
    byte FirmwareVersionCheck(void);
};
extern UpdateFW UPDATEFW;
#endif//UpdateFW_


// API code
/*

void setup()
{
    SetUrlVersion(String);
    SetUrlFirmware(String);
    SetLedPin(byte Pin ,byte stateOn);
    // featback String versionDetec
}

    // HTTP_UPDATE_FAILED,
    // HTTP_UPDATE_NO_UPDATES,
    // HTTP_UPDATE_OK
typedef HTTPUpdateResult t_httpUpdate_return; // backward compatibility
t_httpUpdate_return ret = FirmwareUpdate();
switch (ret) {
  case HTTP_UPDATE_FAILED:
    #ifdef IOTDEVICE_UI
      if(UFWDebug)Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    #endif//#ifdef IOTDEVICE_UI
    
    #ifdef LOOKLINE_UI
      LookLine_prog.DebugOut("Update Faild!!\n", OUPUT);
    #endif// LOOKLINE_UI
      #ifdef ESP_OLED_FEATURE
      FWUPD.ShowMess("Update Faild!!");
      #endif//ESP_OLED_FEATURE
    break;

  case HTTP_UPDATE_NO_UPDATES:
    #ifdef IOTDEVICE_UI
      if(UFWDebug)Serial.println("HTTP_UPDATE_NO_UPDATES");
    #endif//#ifdef IOTDEVICE_UI
    #ifdef LOOKLINE_UI
      LookLine_prog.DebugOut("No Update!!\n", OUPUT);
    #endif// LOOKLINE_UI
      #ifdef ESP_OLED_FEATURE
      FWUPD.ShowMess("No Update!!");
      #endif//ESP_OLED_FEATURE
    break;

  case HTTP_UPDATE_OK:
    #ifdef IOTDEVICE_UI
    if(UFWDebug)Serial.println("HTTP_UPDATE_OK");
    #endif//#ifdef IOTDEVICE_UI
    #ifdef LOOKLINE_UI
      LookLine_prog.DebugOut("Update OK!!\n", OUPUT);
    #endif// LOOKLINE_UI
      #ifdef ESP_OLED_FEATURE
      FWUPD.ShowMess("Update OK!!");
      #endif//ESP_OLED_FEATURE
      #ifdef IOTDEVICE_UI
        if(IOT_DEVICE_FW.ROLE == Node){
          CONFIG::write_byte (EP_EEPROM_WIFI_MODE, MESHMODE);
        }
        if(IOT_DEVICE_FW.ROLE == NodeGateway){
          CONFIG::write_byte (EP_EEPROM_WIFI_MODE, MESHSLAVE);
        }
      #endif//IOTDEVICE_UI
      delay(3000);ESP.restart();
    break;
  }
*/

