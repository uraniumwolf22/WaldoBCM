//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

String incomingdata = "";

String motorType = "";
String motorNumber = "";
String motorResolution = "";
String motorDirection = "";

int motorres = 0;

void setup(){
    //set up serial communication
    incomingdata.reserve(100);
    motorType.reserve(20);
    motorNumber.reserve(20);
    motorResolution.reserve(20);
    motorDirection.reserve(20);


    Serial.begin(9600);
    // set all the pin modes to output.
    pinMode(S1_S,OUTPUT);
    pinMode(S1_D,OUTPUT);
    pinMode(S2_S,OUTPUT);
    pinMode(S2_D,OUTPUT);
    pinMode(S3_S,OUTPUT);
    pinMode(S3_D,OUTPUT);
    pinMode(S4_S,OUTPUT);
    pinMode(S4_D,OUTPUT);
    pinMode(S5_S,OUTPUT);
    pinMode(S5_D,OUTPUT);
    pinMode(S6_S,OUTPUT);
    pinMode(S6_D,OUTPUT);
    //servo pin mapping
    servo1.attach(A5);
    servo2.attach(A0);
    servo3.attach(A4);
    servo4.attach(A3);
    servo5.attach(A2);
    servo6.attach(A1);
    delay(2000);
    Serial.println("Serial link established");
}
void loop(){
    delay(1000);
    Serial.println(motorType);
    Serial.println(motorNumber);
    Serial.println(motorResolution);
    Serial.println(motorDirection);
}
//Detects if a serial event has taken place and changes the BCMstate accordingly
void serialEvent(){             //check serial and get data
    while(Serial.available()){
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        }
        motorType = incomingdata.substring(0,1);
        motorNumber = incomingdata.substring(1,2);
        motorResolution = incomingdata.substring(2,3);
        motorDirection = incomingdata.substring(3,4);

    incomingdata = "";
}

