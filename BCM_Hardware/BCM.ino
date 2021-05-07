//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

int S1timer


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
int S1START;
int S1END;
int S1MID;
bool S1SET = false;
void loop(){
    if(S1SET == false){
        S1START = calcend(.1);
        S1MID = calcend(.6);
        S1END = calcend(1.1);
        S1SET = true;
    }

    if(checktime(S1START)){
        digitalWrite(S1_D,HIGH);
        digitalWrite(S1_S,HIGH);
    }

    if(checktime(S1MID)){
        digitalWrite(S1_S,LOW);
    }

    if(checktime(S1END)){
        S1SET = false;
    }


}

// void serialEvent(){                                     //function that collects and parses the serial data
//     while(Serial.available()){
//         char inChar = (char)Serial.read();
//         incomingdata += inChar;
//         delayMicroseconds(100);                         //this delay has to be here to keep the data from not being broken up
//     }

// }

int calcend(int delay){
    return(millis()+delay);
}

int checktime(int time){
    if(millis() == time){
        return(true);
    }
    if(millis() != time){
        return(false);
    }
}