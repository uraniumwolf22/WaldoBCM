//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

String incomingdata = "";
String motorType = "";
String motorNumber = "";
String motorResolution = "";
String motorDirection = "";

bool stepToDo = false;

int res;
int motor;
int dir;
int speed = 500;

void setup(){
    //set up serial communication
    incomingdata.reserve(100);
    motorType.reserve(20);
    motorNumber.reserve(20);
    motorResolution.reserve(20);
    motorDirection.reserve(20);


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

//Detects if a serial event has taken place and changes the BCMstate accordingly
void serialEvent(){             //check serial and get data
    while(Serial.available()){
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        delayMicroseconds(100);
        }

        Serial.println(incomingdata);
        motorType = incomingdata.substring(0,1);
        motorNumber = incomingdata.substring(1,2);
        motorResolution = incomingdata.substring(2,3);
        motorDirection = incomingdata.substring(3,4);
    incomingdata = "";
    stepToDo = true;
}

void loop(){

    if(stepToDo == true){
        if (motorNumber == "A"){
            motor = 1;
        }
        if (motorNumber == "B"){
            motor = 2;
        }
        if (motorNumber == "C"){
            motor = 3;
        }
        if (motorNumber == "D"){
            motor = 4;
        }
        if (motorNumber == "E"){
            motor = 5;
        }
        if (motorNumber == "F"){
            motor = 6;
        }
        

        if(motorResolution == "A"){
            res = 1;
        }
        
        if(motorResolution == "B"){
            res = 5;
        }
        if(motorResolution == "C"){
            res = 10;
        }
        if(motorResolution == "D"){
            res = 100;
        }


        if(motorDirection == "F"){
            dir = 1;
        }
        if(motorDirection == "B"){
            dir = 0;
        }

    executestepcommand(res,motor,dir);
    stepToDo = false;
    }
}

int executestepcommand(int res, int motor, int dir){
    Serial.println(motor);
    Serial.println(res);
    Serial.println(dir);

    if(motor == 1){
        for(int i = 0; i < res; i++){
            if(dir == 1){
                digitalWrite(S1_D,HIGH);
                digitalWrite(S1_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S1_S,LOW);
            }

            if(dir == 0){
                digitalWrite(S1_D,LOW);
                digitalWrite(S1_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S1_S,LOW);
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
            }
            if(dir == 0){
                digitalWrite(S2_D,LOW);
                digitalWrite(S2_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S2_S,LOW);
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
            }
            if(dir == 0){
                digitalWrite(S3_D,LOW);
                digitalWrite(S3_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S3_S,LOW);
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
            }
            if(dir == 0){
                digitalWrite(S4_D,LOW);
                digitalWrite(S4_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S4_S,LOW);
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
            }
            if(dir == 0){
                digitalWrite(S5_D,LOW);
                digitalWrite(S5_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S5_S,LOW);
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
            }
            if(dir == 0){
                digitalWrite(S6_D,LOW);
                digitalWrite(S6_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S6_S,LOW);
            }
        }
    }
}