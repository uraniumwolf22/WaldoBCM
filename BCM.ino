//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

String incomingdata = "";

void setup(){
    //set up serial communication
    incomingdata.reserve(200);
    Serial.begin(115200);
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
}
//Detects if a serial event has taken place and changes the BCMstate accordingly
void serialEvent(){             //check serial and get data
    while(Serial.available()){
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        }
                            //change state according to recieved serial data
    if(incomingdata == "f"){
        bcmstate = 1;}


    incomingdata = "";
}