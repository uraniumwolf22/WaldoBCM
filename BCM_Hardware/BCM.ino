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

unsigned long S1SPDOFF;         //stepper speed offset
unsigned long S1START;          //stepper start time
unsigned long S1MID;            //stepper midpoint time
unsigned long S1END;            //stepper end time
bool S1A = false;               //step A complete
bool S1B = false;               //step B complete
bool S1C = false;               //step C complete
bool S1STATE = false;           //whole step complete/reset status flag
bool S1DIR;                     //stepper direction
int S1STPC = 0;                 //stepper current step count
int S1ST2D = 0;                 //stepper steps to do count

unsigned long S2SPDOFF;
unsigned long S2START;
unsigned long S2MID;
unsigned long S2END;
bool S2A = false;
bool S2B = false;
bool S2C = false;
bool S2STATE = false;
bool S2DIR;
int S2STPC = 0;
int S2ST2D = 0;

unsigned long S3SPDOFF;
unsigned long S3START;
unsigned long S3MID;
unsigned long S3END;
bool S3A = false;
bool S3B = false;
bool S3C = false;
bool S3STATE = false;
bool S3DIR;
int S3STPC = 0;
int S3ST2D = 0;

unsigned long S4SPDOFF;
unsigned long S4START;
unsigned long S4MID;
unsigned long S4END;
bool S4A = false;
bool S4B = false;
bool S4C = false;
bool S4STATE = false;
bool S4DIR;
int S4STPC = 0;
int S4ST2D = 0;

unsigned long S5SPDOFF;
unsigned long S5START;
unsigned long S5MID;
unsigned long S5END;
bool S5A = false;
bool S5B = false;
bool S5C = false;
bool S5STATE = false;
bool S5DIR;
int S5STPC = 0;
int S5ST2D = 0;

unsigned long S6SPDOFF;
unsigned long S6START;
unsigned long S6MID;
unsigned long S6END;
bool S6A = false;
bool S6B = false;
bool S6C = false;
bool S6STATE = false;
bool S6DIR;
int S6STPC = 0;
int S6ST2D = 0;

bool SV1STATE = false;
int SV1DEG = 0;
unsigned long SV1START;
unsigned long SV1END;
bool SV1A;
bool SV1B;
int SV1TIME;

bool SV2STATE = false;
int SV2DEG = 0;
unsigned long SV2START;
unsigned long SV2END;
bool SV2A;
bool SV2B;

bool SV3STATE = false;
int SV3DEG = 0;
unsigned long SV3START;
unsigned long SV3END;
bool SV3A;
bool SV3B;

bool SV4STATE = false;
int SV4DEG = 0;
unsigned long SV4START;
unsigned long SV4END;
bool SV4A;
bool SV4B;

bool SV5STATE = false;
int SV5DEG = 0;
unsigned long SV5START;
unsigned long SV5END;
bool SV5A;
bool SV5B;

bool SV6STATE = false;
int SV6DEG = 0;
unsigned long SV6START;
unsigned long SV6END;
bool SV6A;
bool SV6B;

void setup(){                   //all the functions that run once when the MCU us turned on
    Serial.begin(115200);       //start serial
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
    servo1.attach(A5);
    servo2.attach(A0);
    servo3.attach(A4);
    servo4.attach(A3);
    servo5.attach(A2);
    servo6.attach(A1);
    Serial.println("Serial link established");

    servo1.write(0);        //make sure all of the servos are locked in the 0 deg position
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);
    servo5.write(0);
    servo6.write(0);
}

void loop(){
    updateS1();
    updateS2();
    updateS3();
    updateS4();
    updateS5();
    updateS6();
    updateSV1();
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

    if(let_to_num(motornum) == 2){
        S2STPC = 0;
        S2ST2D = 0;
        S2ST2D = calcsteps(motordist.toInt());
        S2SPDOFF = calcspeed(S2ST2D);
        if(motordir == "F"){
            S2DIR = true;
        }
        
        if(motordir == "B"){
            S2DIR = false;
        }
    }

    if(let_to_num(motornum) == 3){
        S3STPC = 0;
        S3ST2D = 0;
        S3ST2D = calcsteps(motordist.toInt());
        S3SPDOFF = calcspeed(S3ST2D);
        if(motordir == "F"){
            S3DIR = true;
        }
        
        if(motordir == "B"){
            S3DIR = false;
        }
    }

    if(let_to_num(motornum) == 4){
        S4STPC = 0;
        S4ST2D = 0;
        S4ST2D = calcsteps(motordist.toInt());
        S4SPDOFF = calcspeed(S4ST2D);
        if(motordir == "F"){
            S4DIR = true;
        }
        
        if(motordir == "B"){
            S4DIR = false;
        }
    }

    if(let_to_num(motornum) == 5){
        S5STPC = 0;
        S5ST2D = 0;
        S5ST2D = calcsteps(motordist.toInt());
        S5SPDOFF = calcspeed(S5ST2D);
        if(motordir == "F"){
            S5DIR = true;
        }
        
        if(motordir == "B"){
            S5DIR = false;
        }
    }

    if(let_to_num(motornum) == 6){
        S6STPC = 0;
        S6ST2D = 0;
        S6ST2D = calcsteps(motordist.toInt());
        S6SPDOFF = calcspeed(S6ST2D);
        if(motordir == "F"){
            S6DIR = true;
        }
        
        if(motordir == "B"){
            S6DIR = false;
        }
    }

    if(let_to_num(motornum) == 1+6){
        SV1DEG = (motordist.toInt()/3)*2;
        int diff = SV1DEG - pos[1];
        diff = abs(diff);
        SV1TIME = motortime.toInt() / diff;
    }

    incomingdata = "";                                  //reset the incoming data variable
    actionready = true;                                 //set the action ready flag so the main loop knows there is a command to execute
}

void updateS1(){
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


void updateSV1(){
    if(SV1STATE == false){
        Serial.println("update start");
        SV1START = currentmicro();
        SV1END = SV1START + SV1TIME * 1000;
        SV1STATE = true;
        SV1A = false;
        SV1B = false;
    }

    if(currentmicro() >= SV1START && currentmicro() <= (SV1START + 16) && SV1A == false){
        if(pos[1] <= SV1DEG){
            pos[1] = pos[1] + 1;
            servo1.write(pos[1]);
        }
        if(pos[1] > SV1DEG){
            pos[1] = pos[1] - 1;
            servo1.write(pos[1]);
        }
        SV1A == true;
    }

    if(currentmicro() >= SV1END && SV1B == false){
        SV1B == true;
        SV1STATE = false;
        Serial.println("update end");
        Serial.println(pos[1]);
    }
}



void updateS2(){
    if(S2ST2D > S2STPC){
        if(S2STATE == false){
            S2START = currentmicro();
            S2MID = currentmicro() + speed;
            S2END = S2MID + speed + (S1SPDOFF*1000);
            S2STATE = true;
            S2A = false;
            S2B = false;
            S2C = false;
        }

        if(currentmicro() >= S2START && currentmicro() <= S2MID && S2A == false){
            if(S2DIR == true){
                digitalWrite(S2_D,HIGH);
            }

            if(S2DIR == false){
                digitalWrite(S2_D,LOW);
            }

            digitalWrite(S2_S,HIGH);
            S2A = true;
        }

        if(currentmicro() >= S2MID && currentmicro() <= S2END && S2B == false){
            digitalWrite(S2_S,LOW);
            S2B = true;
        }

        if(currentmicro() >= S2END && S2C == false){
            S2STATE = false;
            S2C = true;
            S2STPC++;
        }
    }
}

void updateS3(){
    if(S3ST2D > S3STPC){
        if(S3STATE == false){
            S3START = currentmicro();
            S3MID = currentmicro() + speed;
            S3END = S3MID + speed + (S3SPDOFF*1000);
            S3STATE = true;
            S3A = false;
            S3B = false;
            S3C = false;
        }

        if(currentmicro() >= S3START && currentmicro() <= S3MID && S3A == false){
            if(S3DIR == true){
                digitalWrite(S3_D,HIGH);
            }

            if(S3DIR == false){
                digitalWrite(S3_D,LOW);
            }

            digitalWrite(S3_S,HIGH);
            S3A = true;
        }

        if(currentmicro() >= S3MID && currentmicro() <= S3END && S3B == false){
            digitalWrite(S3_S,LOW);
            S3B = true;
        }

        if(currentmicro() >= S3END && S3C == false){
            S3STATE = false;
            S3C = true;
            S3STPC++;
        }
    }
}

void updateS4(){
    if(S4ST2D > S4STPC){
        if(S4STATE == false){
            S4START = currentmicro();
            S4MID = currentmicro() + speed;
            S4END = S4MID + speed + (S4SPDOFF*1000);
            S4STATE = true;
            S4A = false;
            S4B = false;
            S4C = false;
        }

        if(currentmicro() >= S4START && currentmicro() <= S4MID && S4A == false){
            if(S4DIR == true){
                digitalWrite(S4_D,HIGH);
            }

            if(S4DIR == false){
                digitalWrite(S4_D,LOW);
            }

            digitalWrite(S4_S,HIGH);
            S4A = true;
        }

        if(currentmicro() >= S4MID && currentmicro() <= S4END && S4B == false){
            digitalWrite(S4_S,LOW);
            S4B = true;
        }

        if(currentmicro() >= S4END && S4C == false){
            S4STATE = false;
            S4C = true;
            S4STPC++;
        }
    }
}

void updateS5(){
    if(S5ST2D > S5STPC){
        if(S5STATE == false){
            S5START = currentmicro();
            S5MID = currentmicro() + speed;
            S5END = S5MID + speed + (S5SPDOFF*1000);
            S5STATE = true;
            S5A = false;
            S5B = false;
            S5C = false;
        }

        if(currentmicro() >= S5START && currentmicro() <= S5MID && S5A == false){
            if(S5DIR == true){
                digitalWrite(S5_D,HIGH);
            }

            if(S5DIR == false){
                digitalWrite(S5_D,LOW);
            }

            digitalWrite(S5_S,HIGH);
            S5A = true;
        }

        if(currentmicro() >= S5MID && currentmicro() <= S5END && S5B == false){
            digitalWrite(S5_S,LOW);
            S5B = true;
        }

        if(currentmicro() >= S5END && S5C == false){
            S5STATE = false;
            S5C = true;
            S5STPC++;
        }
    }
}

void updateS6(){
    if(S6ST2D > S6STPC){
        if(S6STATE == false){
            S6START = currentmicro();
            S6MID = currentmicro() + speed;
            S6END = S6MID + speed + (S6SPDOFF*1000);
            S6STATE = true;
            S6A = false;
            S6B = false;
            S6C = false;
        }

        if(currentmicro() >= S6START && currentmicro() <= S6MID && S6A == false){
            if(S6DIR == true){
                digitalWrite(S6_D,HIGH);
            }

            if(S6DIR == false){
                digitalWrite(S6_D,LOW);
            }

            digitalWrite(S6_S,HIGH);
            S6A = true;
        }

        if(currentmicro() >= S6MID && currentmicro() <= S6END && S6B == false){
            digitalWrite(S6_S,LOW);
            S6B = true;
        }

        if(currentmicro() >= S6END && S6C == false){
            S6STATE = false;
            S6C = true;
            S6STPC++;
        }
    }
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

unsigned long currentmicro(){
    return(micros());
}