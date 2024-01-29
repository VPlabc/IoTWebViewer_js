
#ifndef Radar_
#define Radar_
#include "Arduino.h"

// typedef struct RadarData
// {
//     uint16_t distanceMoving;
//     uint8_t energyMoving;
//     uint16_t distanceActive;
//     uint8_t energyActive;
// } RadarData;

class RadarFunc
{
public:


    uint16_t distanceMoving;
    uint8_t energyMoving;
    uint16_t distanceActive;
    uint8_t energyActive;
    uint16_t MaxStation;
    uint16_t MaxMoving;
    uint16_t inactivity;

    bool engineeringMode = false;
    String command;
    int count = 0;

    uint8_t targetmoving[9]; //
    uint8_t targetstation[9]; //
    void setup();
    void loop();
    bool read();
    void RadarFunct(char serial);
    bool setGateSensitivityThresholdRD(uint8_t gate,uint8_t motionSensitivity,uint8_t stationarySensitivity);
    void GetConfig();
//----------------------------------------------------------------

private:

};
extern RadarFunc RADAR_;


#endif //Radar_

// class myValues
// {
//   public:
//     int a, b, c;

//     myValues() // initial constructor
//     {
//       a = b = c = 0;
//     }

//     myValues(int A, int B, int C) // second constructor that gets data and stores it
//     {
//       a = A;
//       b = B;
//       c = C;
//     }

//     myValues data() // a function to add in values, return type must be the class name
//     {
//       return myValues(45, 200, 35);
//     }
// };
// //---------------END_OF_CLASS----------------

// myValues mVal; // constructor
// void setup() 
// {
//   // put your setup code here, to run once:
//   Serial.begin(115200);

//   myValues D = mVal.data(); // call the data function which puts some values into the second constructor, then
//                             // using another constructor, you collect that data.
//   Serial.println(D.a);
//   Serial.println(D.b);
//   Serial.println(D.c);
// }

// void loop() {
//   // put your main code here, to run repeatedly:

// }