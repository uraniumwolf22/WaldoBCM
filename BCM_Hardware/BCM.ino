//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

void setup(){                   //all the functions that run once when the MCU us turned on
    //set up serial communication


    Serial.begin(115200);       //start serial
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
}
long speed = 500;
unsigned long prevmicro = 0;
unsigned long S1START;
unsigned long S1MID;
unsigned long S1END;

bool S1A = false;
bool S1B = false;
bool S1C = false;

bool S1STATE = false;

void loop(){
    if(S1STATE == false){
        S1START = currentmicro();
        S1MID = currentmicro() + 500;
        S1END = S1MID + 500;
        S1STATE = true;
        S1A = false;
        S1B = false;
        S1C = false;
    }

    if(currentmicro() >= S1START && currentmicro() <= S1MID && S1A == false){
        digitalWrite(S1_D,HIGH);
        digitalWrite(S1_S,HIGH);
        S1A = true;
    }

    if(currentmicro() >= S1MID && currentmicro() <= S1END && S1B == false){
        digitalWrite(S1_S,LOW);
        S1B = true;
    }

    if(currentmicro() >= S1END && S1C == false){
        S1STATE = false;
        S1C = true;
    }
}

// void serialEvent(){                                     //function that collects and parses the serial data
//     while(Serial.available()){
//         char inChar = (char)Serial.read();
//         incomingdata += inChar;
//         delayMicroseconds(100);                         //this delay has to be here to keep the data from not being broken up
//     }

// }

unsigned long currentmicro(){
    return(micros());
}