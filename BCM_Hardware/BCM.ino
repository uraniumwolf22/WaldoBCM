//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

String motornum;
String motordir;
String motortime;
String motordist;


bool actionready = false;
int sep1,sep2,sep3,sep4,sep5;

String incomingdata = "";
void setup(){
    //set up serial communication


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
    // servo1.attach(A5);
    // servo2.attach(A0);
    // servo3.attach(A4);
    // servo4.attach(A3);
    // servo5.attach(A2);
    // servo6.attach(A1);
    Serial.println("Serial link established");
}


void serialEvent(){
    while(Serial.available()){
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        delayMicroseconds(100);
    }
    Serial.println(incomingdata);
    sep1 = incomingdata.indexOf(":");
    sep2 = incomingdata.indexOf(":",sep1+1);
    sep3 = incomingdata.indexOf(":",sep2+1);
    sep4 = incomingdata.indexOf(":",sep3+1);
    sep5 = incomingdata.indexOf(":",sep4+1);
    
    motornum = incomingdata.substring(sep1+1,sep2);
    motordir = incomingdata.substring(sep2+1,sep3);
    motortime = incomingdata.substring(sep3+1,sep4);
    motordist = incomingdata.substring(sep4+1,sep5);

    incomingdata = "";
    actionready = true;
}

void loop(){
    if(actionready ==  true){
        



        actionready = false;
    }
}


int executestepcommand(int res, int motor, int dir){
    if(motor == 1){
        for(int i = 0; i < res; i++){
            if(dir == 1){
                digitalWrite(S1_D,HIGH);
                digitalWrite(S1_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S1_S,LOW);
                delayMicroseconds(speed);
            }

            if(dir == 0){
                digitalWrite(S1_D,LOW);
                digitalWrite(S1_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S1_S,LOW);
                delayMicroseconds(speed);
            }
        }
    }

    if(motor == 2){
        for(int i = 0; i < res; i++){
            if(dir == 1){
                digitalWrite(S2_D,HIGH);
                digitalWrite(S2_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S2_S,LOW);
                delayMicroseconds(speed);
            }
            if(dir == 0){
                digitalWrite(S2_D,LOW);
                digitalWrite(S2_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S2_S,LOW);
                delayMicroseconds(speed);
            }
        }
    }

    if(motor == 3){
        for(int i = 0; i < res; i++){
            if(dir == 1){
                digitalWrite(S3_D,HIGH);
                digitalWrite(S3_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S3_S,LOW);
                delayMicroseconds(speed);
            }
            if(dir == 0){
                digitalWrite(S3_D,LOW);
                digitalWrite(S3_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S3_S,LOW);
                delayMicroseconds(speed);
            }
        }
    }

    if(motor == 4){
        for(int i = 0; i < res; i++){
            if(dir == 1){
                digitalWrite(S4_D,HIGH);
                digitalWrite(S4_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S4_S,LOW);
                delayMicroseconds(speed);
            }
            if(dir == 0){
                digitalWrite(S4_D,LOW);
                digitalWrite(S4_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S4_S,LOW);
                delayMicroseconds(speed);
            }
        }
    }

    if(motor == 5){
        for(int i = 0; i < res; i++){
            if(dir == 1){
                digitalWrite(S5_D,HIGH);
                digitalWrite(S5_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S5_S,LOW);
                delayMicroseconds(speed);
            }
            if(dir == 0){
                digitalWrite(S5_D,LOW);
                digitalWrite(S5_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S5_S,LOW);
                delayMicroseconds(speed);
            }
        }
    }

    if(motor == 6){
        for(int i = 0; i < res; i++){
            if(dir == 1){
                digitalWrite(S6_D,HIGH);
                digitalWrite(S6_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S6_S,LOW);
                delayMicroseconds(speed);
            }
            if(dir == 0){
                digitalWrite(S6_D,LOW);
                digitalWrite(S6_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S6_S,LOW);
                delayMicroseconds(speed);
            }
        }
    }
}