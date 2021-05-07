//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

int speed = 500;        //global speed of stepper motor step time

String motornum;        //what number motor we are operating
String motordir;        //motor direction
String motortime;       //how long the motor should take to do the operation
String motordist;       //how far the motor is traveling in degrees

int motor_;             //motor number working var
int direction_;         //direction working var
int servonum;           //servo num working var
int servodeg_;          //servo degrees working var
int servotime_;         //servo time to complete working var

bool actionready = false;       //Variable that tells the main loop if a serial command is ready to execute
int sep1,sep2,sep3,sep4,sep5;   //all the seperating locations used to split up serial command

String incomingdata = "";       //string that holds the incoming data from the serial port

int pos[] = {0,0,0,0,0,0,0};    //array to store all the servo positions

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

unsigned long S1SPDOFF;
unsigned long S1START;
unsigned long S1MID;
unsigned long S1END;
bool S1A = false;
bool S1B = false;
bool S1C = false;
bool S1STATE = false;
bool S1DIR;
int S1STPC = 0;
int S1ST2D = 0;

void loop(){
    if(S1ST2D > S1STPC){
        if(S1STATE == false){
            S1START = currentmicro();
            S1MID = currentmicro() + speed;
            S1END = S1MID + speed + (S1SPDOFF*1000);
            S1STATE = true;
            S1A = false;
            S1B = false;
            S1C = false;
        }

        if(currentmicro() >= S1START && currentmicro() <= S1MID && S1A == false){
            if(S1DIR == true){
                digitalWrite(S1_D,HIGH);
            }

            if(S1DIR == false){
                digitalWrite(S1_D,LOW);
            }

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
            S1STPC++;
        }
    }
}




unsigned long currentmicro(){
    return(micros());
}

void serialEvent(){                                     //function that collects and parses the serial data
    while(Serial.available()){
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        delayMicroseconds(150);                         //this delay has to be here to keep the data from not being broken up
    }
    Serial.println(incomingdata);                       //find all the seperator locations
    sep1 = incomingdata.indexOf(":");
    sep2 = incomingdata.indexOf(":",sep1+1);
    sep3 = incomingdata.indexOf(":",sep2+1);
    sep4 = incomingdata.indexOf(":",sep3+1);
    sep5 = incomingdata.indexOf(":",sep4+1);
    
    motornum = incomingdata.substring(sep1+1,sep2);     //split the data based on the seperator locations
    motordir = incomingdata.substring(sep2+1,sep3);
    motortime = incomingdata.substring(sep3+1,sep4);
    motordist = incomingdata.substring(sep4+1,sep5);

    if(let_to_num(motornum) == 1){
        S1STPC = 0;
        S1ST2D = 0;
        S1ST2D = calcsteps(motordist.toInt());
        S1SPDOFF = calcspeed(S1ST2D);
        if(motordir == "F"){
            S1DIR = true;
        }
        
        if(motordir == "B"){
            S1DIR = false;
        }
    }

    incomingdata = "";                                  //reset the incoming data variable
    actionready = true;                                 //set the action ready flag so the main loop knows there is a command to execute
}

int let_to_num(String motorNumber){     //function to convert the motor letter to the motor number
    int motor;
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
    if (motorNumber == "G"){
        motor = 7;
    }
    if (motorNumber == "H"){
        motor = 8;
    }
    if (motorNumber == "I"){
        motor = 9;
    }
    if (motorNumber == "J"){
        motor = 10;
    }
    if (motorNumber == "K"){
        motor = 11;
    }
    if (motorNumber == "L"){
        motor = 12;
    }
    return motor;
}

int calcsteps(int deg){                 //calculate how many steps the servo needs to take based on degrees
    int steps;
    steps = deg/1.8;
    return(steps);
}

unsigned long calcspeed(int steps_){            
    unsigned long speedoffset;                            //calculate the offset to achieve desired stepper time
    long requestedspeed = motortime.toInt();            //convert the offset time to a long so we can work with numbers over 36000
    speedoffset = ((requestedspeed - (steps_ * ((speed * 2)/1000)))/steps_);    //calculate the offset based on current working variables
    if(speedoffset < 0){                                //if the offset is smaller than the minumum required time make the offset 0 aka max speed
        speedoffset = 0;
    }
    return(speedoffset);
}