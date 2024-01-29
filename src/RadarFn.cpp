#include "Arduino.h"
#include "RadarFn.h"
RadarFunc Data;
#include "main.h"
main main_tlg;
#include <ld2410.h>

ld2410 radar;

#define RADAR_RX_PIN 4
#define RADAR_TX_PIN 5


bool RadarFunc::setGateSensitivityThresholdRD(uint8_t gate,uint8_t motionSensitivity,uint8_t stationarySensitivity){
  // Serial.print("gate: " + String(gate) + " | motionSensitivity: " + String(motionSensitivity) + " | motionSensitivity: " + String(motionSensitivity));
  // return true;
  return radar.setGateSensitivityThreshold( gate, motionSensitivity, stationarySensitivity);
}

void RadarFunc::GetConfig()
{
  Serial1.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN); //UART for monitoring the radar

  delay(500);
  if(radar.begin(Serial1))
  { Serial.println("radar reload");}
  if(radar.requestRestart()){ Serial.println("restart done");}
  else{ Serial.println("restart failed");}
  if(radar.requestCurrentConfiguration()){Serial.println(F("OK"));}
    for(uint8_t gate = 0; gate <= radar.max_gate; gate++)
    {
      Serial.print(F("Gate "));
      Serial.print(gate);
      Serial.print(F(" moving targets: "));
      Serial.print(radar.motion_sensitivity[gate]);
      Serial.print(F(" stationary targets: "));
      Serial.println(radar.stationary_sensitivity[gate]);
      targetmoving[gate] = radar.motion_sensitivity[gate];
      targetstation[gate] = radar.stationary_sensitivity[gate];
    }
}
bool RadarFunc::read(){
  // RadarData Data;
  if(radar.isConnected())
  {
    //  Serial.println(F("OK"));

        // Serial.print(F("Stationary target: "));
        // Serial.print(radar.stationaryTargetDistance());
        // Serial.print(F("cm energy: "));
        // Serial.println(radar.stationaryTargetEnergy());
        distanceActive = radar.stationaryTargetDistance();
        energyActive = radar.stationaryTargetEnergy();
        
        // Serial.print(F("Moving target: "));
        // Serial.print(radar.movingTargetDistance());
        // Serial.print(F("cm energy: "));
        // Serial.println(radar.movingTargetEnergy());
        distanceMoving = radar.movingTargetDistance();
        energyMoving = radar.movingTargetEnergy();
        MaxStation = radar.max_stationary_gate;
        MaxMoving = radar.max_moving_gate;
        inactivity = radar.sensor_idle_time;
        main_tlg.GetData(distanceActive, energyActive, distanceMoving, energyMoving, MaxStation, MaxMoving, inactivity);
    if(radar.presenceDetected())
    {

        // Serial.print(F("Stationary target: "));
        // Serial.print(radar.stationaryTargetDistance());
        // Serial.print(F("cm energy: "));
        // Serial.println(radar.stationaryTargetEnergy());
        // Data.distanceActive = radar.stationaryTargetDistance();
        // Data.energyActive = radar.stationaryTargetEnergy();
        
        // Serial.print(F("Moving target: "));
        // Serial.print(radar.movingTargetDistance());
        // Serial.print(F("cm energy: "));
        // Serial.println(radar.movingTargetEnergy());
        // Data.distanceMoving = radar.movingTargetDistance();
        // Data.energyMoving = radar.movingTargetEnergy();
      if(radar.stationaryTargetDetected())
      {
      }
      if(radar.movingTargetDetected())
      {
      }

      return true;
    }
    else
    {
      // Serial.println(F("nothing detected"));
      return false;
    }
  }

}
void RadarFunc::setup(void)
{

  // RadarData RDData;
  Serial.begin(9600); //Feedback over Serial Monitor
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


  command = "";
        Serial.print(F("Reading configuration from sensor: "));
        if(radar.requestCurrentConfiguration())
        {
          Serial.println(F("OK"));
          Serial.print(F("Maximum gate ID: "));
          Serial.println(radar.max_gate);
          Serial.print(F("Maximum gate for moving targets: "));
          Serial.println(radar.max_moving_gate);
          Serial.print(F("Maximum gate for stationary targets: "));
          Serial.println(radar.max_stationary_gate);
          Serial.print(F("Idle time for targets: "));
          Serial.println(radar.sensor_idle_time);
          Serial.println(F("Gate sensitivity"));

        }
        else
        {
          Serial.println(F("Failed"));
        }

}
// setsensitivity 8 100 100
void RadarFunc::loop()
{
  radar.read(); //Always read frames from the sensor
  if(Serial.available())
  {
    char typedCharacter = Serial.read();
    if(typedCharacter == '\r' || typedCharacter == '\n')
    {
      command.trim();
      if(command.equals("read"))
      {
        command = "";
        Serial.print(F("Reading from sensor: "));
        if(radar.isConnected())
        {
          Serial.println(F("OK"));
          if(radar.presenceDetected())
          {
            if(radar.stationaryTargetDetected())
            {
              Serial.print(F("Stationary target: "));
              Serial.print(radar.stationaryTargetDistance());
              Serial.print(F("cm energy: "));
              Serial.println(radar.stationaryTargetEnergy());
            }
            if(radar.movingTargetDetected())
            {
              Serial.print(F("Moving target: "));
              Serial.print(radar.movingTargetDistance());
              Serial.print(F("cm energy: "));
              Serial.println(radar.movingTargetEnergy());
            }
          }
          else
          {
            Serial.println(F("nothing detected"));
          }
        }
        else
        {
          Serial.println(F("failed to read"));
        }
      }
      else if(command.equals("readconfig"))
      {
        command = "";
        Serial.print(F("Reading configuration from sensor: "));
        if(radar.requestCurrentConfiguration())
        {
          Serial.println(F("OK"));
          Serial.print(F("Maximum gate ID: "));
          Serial.println(radar.max_gate);
          Serial.print(F("Maximum gate for moving targets: "));
          Serial.println(radar.max_moving_gate);
          Serial.print(F("Maximum gate for stationary targets: "));
          Serial.println(radar.max_stationary_gate);
          Serial.print(F("Idle time for targets: "));
          Serial.println(radar.sensor_idle_time);
          Serial.println(F("Gate sensitivity"));
          for(uint8_t gate = 0; gate <= radar.max_gate; gate++)
          {
            Serial.print(F("Gate "));
            Serial.print(gate);
            Serial.print(F(" moving targets: "));
            Serial.print(radar.motion_sensitivity[gate]);
            Serial.print(F(" stationary targets: "));
            Serial.println(radar.stationary_sensitivity[gate]);
          }
        }
        else
        {
          Serial.println(F("Failed"));
        }
      }
      else if(command.startsWith("setmaxvalues"))
      {
        uint8_t firstSpace = command.indexOf(' ');
        uint8_t secondSpace = command.indexOf(' ',firstSpace + 1);
        uint8_t thirdSpace = command.indexOf(' ',secondSpace + 1);
        uint8_t newMovingMaxDistance = (command.substring(firstSpace,secondSpace)).toInt();
        uint8_t newStationaryMaxDistance = (command.substring(secondSpace,thirdSpace)).toInt();
        uint16_t inactivityTimer = (command.substring(thirdSpace,command.length())).toInt();
        if(newMovingMaxDistance > 0 && newStationaryMaxDistance > 0 && newMovingMaxDistance <= 8 && newStationaryMaxDistance <= 8)
        {
          Serial.print(F("Setting max values to gate "));
          Serial.print(newMovingMaxDistance);
          Serial.print(F(" moving targets, gate "));
          Serial.print(newStationaryMaxDistance);
          Serial.print(F(" stationary targets, "));
          Serial.print(inactivityTimer);
          Serial.print(F("s inactivity timer: "));
          command = "";
          if(radar.setMaxValues(newMovingMaxDistance, newStationaryMaxDistance, inactivityTimer))
          {
            Serial.println(F("OK, now restart to apply settings"));
          }
          else
          {
            Serial.println(F("failed"));
          }
        }
        else
        {
          Serial.print(F("Can't set distances to "));
          Serial.print(newMovingMaxDistance);
          Serial.print(F(" moving "));
          Serial.print(newStationaryMaxDistance);
          Serial.println(F(" stationary, try again"));
          command = "";
        }
      }
      else if(command.startsWith("setsensitivity"))
      {
        uint8_t firstSpace = command.indexOf(' ');
        uint8_t secondSpace = command.indexOf(' ',firstSpace + 1);
        uint8_t thirdSpace = command.indexOf(' ',secondSpace + 1);
        uint8_t gate = (command.substring(firstSpace,secondSpace)).toInt();
        uint8_t motionSensitivity = (command.substring(secondSpace,thirdSpace)).toInt();
        uint8_t stationarySensitivity = (command.substring(thirdSpace,command.length())).toInt();
        if(motionSensitivity >= 0 && stationarySensitivity >= 0 && motionSensitivity <= 100 && stationarySensitivity <= 100)
        {
          Serial.print(F("Setting gate "));
          Serial.print(gate);
          Serial.print(F(" motion sensitivity to "));
          Serial.print(motionSensitivity);
          Serial.print(F(" & stationary sensitivity to "));
          Serial.print(stationarySensitivity);
          Serial.println(F(": "));
          command = "";
          if(radar.setGateSensitivityThreshold(gate, motionSensitivity, stationarySensitivity))
          {
            Serial.println(F("OK, now restart to apply settings"));
          }
          else
          {
            Serial.println(F("failed"));
          }
        }
        else
        {
          Serial.print(F("Can't set gate "));
          Serial.print(gate);
          Serial.print(F(" motion sensitivity to "));
          Serial.print(motionSensitivity);
          Serial.print(F(" & stationary sensitivity to "));
          Serial.print(stationarySensitivity);
          Serial.println(F(", try again"));
          command = "";
        }
      }
      else if(command.equals("enableengineeringmode"))
      {
        command = "";
        Serial.print(F("Enabling engineering mode: "));
        if(radar.requestStartEngineeringMode())
        {
          Serial.println(F("OK"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else if(command.equals("disableengineeringmode"))
      {
        command = "";
        Serial.print(F("Disabling engineering mode: "));
        if(radar.requestEndEngineeringMode())
        {
          Serial.println(F("OK"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else if(command.equals("restart"))
      {
        command = "";
        Serial.print(F("Restarting sensor: "));
        if(radar.requestRestart())
        {
          Serial.println(F("OK"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else if(command.equals("readversion"))
      {
        command = "";
        Serial.print(F("Requesting firmware version: "));
        if(radar.requestFirmwareVersion())
        {
          Serial.print('v');
          Serial.print(radar.firmware_major_version);
          Serial.print('.');
          Serial.print(radar.firmware_minor_version);
          Serial.print('.');
          Serial.println(radar.firmware_bugfix_version,HEX);
        }
        else
        {
          Serial.println(F("Failed"));
        }
      }
      else if(command.equals("factoryreset"))
      {
        command = "";
        Serial.print(F("Factory resetting sensor: "));
        if(radar.requestFactoryReset())
        {
          Serial.println(F("OK, now restart sensor to take effect"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else
      {
        Serial.print(F("Unknown command: "));
        Serial.println(command);
        command = "";
      }
    }
    else
    {
      command += typedCharacter;
    }
  }
  /*
  count++;
  if(count > 10000)  //Some data has been received from the radar
  {
    count = 0;
    if(radar.presenceDetected())
    {
      Serial.print(F("Stationary target: "));
      Serial.println(radar.stationaryTargetDistance());
      Serial.print(F("Moving target: "));
      Serial.println(radar.movingTargetDistance());
    }
  }
  //*/
}

void RadarFunc::RadarFunct(char serial){

    char typedCharacter = serial;
    if(typedCharacter == '\r' || typedCharacter == '\n')
    {
      command.trim();
      if(command.equals("read"))
      {
        command = "";
        Serial.print(F("Reading from sensor: "));
        if(radar.isConnected())
        {
          Serial.println(F("OK"));
          if(radar.presenceDetected())
          {
            if(radar.stationaryTargetDetected())
            {
              Serial.print(F("Stationary target: "));
              Serial.print(radar.stationaryTargetDistance());
              Serial.print(F("cm energy: "));
              Serial.println(radar.stationaryTargetEnergy());
            }
            if(radar.movingTargetDetected())
            {
              Serial.print(F("Moving target: "));
              Serial.print(radar.movingTargetDistance());
              Serial.print(F("cm energy: "));
              Serial.println(radar.movingTargetEnergy());
            }
          }
          else
          {
            Serial.println(F("nothing detected"));
          }
        }
        else
        {
          Serial.println(F("failed to read"));
        }
      }
      else if(command.equals("readconfig"))
      {
        command = "";
        Serial.print(F("Reading configuration from sensor: "));
        if(radar.requestCurrentConfiguration())
        {
          Serial.println(F("OK"));
          Serial.print(F("Maximum gate ID: "));
          Serial.println(radar.max_gate);
          Serial.print(F("Maximum gate for moving targets: "));
          Serial.println(radar.max_moving_gate);
          Serial.print(F("Maximum gate for stationary targets: "));
          Serial.println(radar.max_stationary_gate);
          Serial.print(F("Idle time for targets: "));
          Serial.println(radar.sensor_idle_time);
          Serial.println(F("Gate sensitivity"));
          for(uint8_t gate = 0; gate <= radar.max_gate; gate++)
          {
            Serial.print(F("Gate "));
            Serial.print(gate);
            Serial.print(F(" moving targets: "));
            Serial.print(radar.motion_sensitivity[gate]);
            Serial.print(F(" stationary targets: "));
            Serial.println(radar.stationary_sensitivity[gate]);
          }
        }
        else
        {
          Serial.println(F("Failed"));
        }
      }
      else if(command.startsWith("setmaxvalues"))
      {
        uint8_t firstSpace = command.indexOf(' ');
        uint8_t secondSpace = command.indexOf(' ',firstSpace + 1);
        uint8_t thirdSpace = command.indexOf(' ',secondSpace + 1);
        uint8_t newMovingMaxDistance = (command.substring(firstSpace,secondSpace)).toInt();
        uint8_t newStationaryMaxDistance = (command.substring(secondSpace,thirdSpace)).toInt();
        uint16_t inactivityTimer = (command.substring(thirdSpace,command.length())).toInt();
        if(newMovingMaxDistance > 0 && newStationaryMaxDistance > 0 && newMovingMaxDistance <= 8 && newStationaryMaxDistance <= 8)
        {
          Serial.print(F("Setting max values to gate "));
          Serial.print(newMovingMaxDistance);
          Serial.print(F(" moving targets, gate "));
          Serial.print(newStationaryMaxDistance);
          Serial.print(F(" stationary targets, "));
          Serial.print(inactivityTimer);
          Serial.print(F("s inactivity timer: "));
          command = "";
          if(radar.setMaxValues(newMovingMaxDistance, newStationaryMaxDistance, inactivityTimer))
          {
            Serial.println(F("OK, now restart to apply settings"));
          }
          else
          {
            Serial.println(F("failed"));
          }
        }
        else
        {
          Serial.print(F("Can't set distances to "));
          Serial.print(newMovingMaxDistance);
          Serial.print(F(" moving "));
          Serial.print(newStationaryMaxDistance);
          Serial.println(F(" stationary, try again"));
          command = "";
        }
      }
      else if(command.startsWith("setsensitivity"))
      {
        uint8_t firstSpace = command.indexOf(' ');
        uint8_t secondSpace = command.indexOf(' ',firstSpace + 1);
        uint8_t thirdSpace = command.indexOf(' ',secondSpace + 1);
        uint8_t gate = (command.substring(firstSpace,secondSpace)).toInt();
        uint8_t motionSensitivity = (command.substring(secondSpace,thirdSpace)).toInt();
        uint8_t stationarySensitivity = (command.substring(thirdSpace,command.length())).toInt();
        if(motionSensitivity >= 0 && stationarySensitivity >= 0 && motionSensitivity <= 100 && stationarySensitivity <= 100)
        {
          Serial.print(F("Setting gate "));
          Serial.print(gate);
          Serial.print(F(" motion sensitivity to "));
          Serial.print(motionSensitivity);
          Serial.print(F(" & stationary sensitivity to "));
          Serial.print(stationarySensitivity);
          Serial.println(F(": "));
          command = "";
          if(radar.setGateSensitivityThreshold(gate, motionSensitivity, stationarySensitivity))
          {
            Serial.println(F("OK, now restart to apply settings"));
          }
          else
          {
            Serial.println(F("failed"));
          }
        }
        else
        {
          Serial.print(F("Can't set gate "));
          Serial.print(gate);
          Serial.print(F(" motion sensitivity to "));
          Serial.print(motionSensitivity);
          Serial.print(F(" & stationary sensitivity to "));
          Serial.print(stationarySensitivity);
          Serial.println(F(", try again"));
          command = "";
        }
      }
      else if(command.equals("enableengineeringmode"))
      {
        command = "";
        Serial.print(F("Enabling engineering mode: "));
        if(radar.requestStartEngineeringMode())
        {
          Serial.println(F("OK"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else if(command.equals("disableengineeringmode"))
      {
        command = "";
        Serial.print(F("Disabling engineering mode: "));
        if(radar.requestEndEngineeringMode())
        {
          Serial.println(F("OK"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else if(command.equals("restart"))
      {
        command = "";
        Serial.print(F("Restarting sensor: "));
        if(radar.requestRestart())
        {
          Serial.println(F("OK"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else if(command.equals("readversion"))
      {
        command = "";
        Serial.print(F("Requesting firmware version: "));
        if(radar.requestFirmwareVersion())
        {
          Serial.print('v');
          Serial.print(radar.firmware_major_version);
          Serial.print('.');
          Serial.print(radar.firmware_minor_version);
          Serial.print('.');
          Serial.println(radar.firmware_bugfix_version,HEX);
        }
        else
        {
          Serial.println(F("Failed"));
        }
      }
      else if(command.equals("factoryreset"))
      {
        command = "";
        Serial.print(F("Factory resetting sensor: "));
        if(radar.requestFactoryReset())
        {
          Serial.println(F("OK, now restart sensor to take effect"));
        }
        else
        {
          Serial.println(F("failed"));
        }
      }
      else
      {
        Serial.print(F("Unknown command: "));
        Serial.println(command);
        command = "";
      }
    }
    else
    {
      command += typedCharacter;
    }
  
  /*
  count++;
  if(count > 10000)  //Some data has been received from the radar
  {
    count = 0;
    if(radar.presenceDetected())
    {
      Serial.print(F("Stationary target: "));
      Serial.println(radar.stationaryTargetDistance());
      Serial.print(F("Moving target: "));
      Serial.println(radar.movingTargetDistance());
    }
  }
  //*/
}