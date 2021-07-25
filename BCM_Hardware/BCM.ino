//Waldo body control Firmware for the operation of
//6 stepper motors and 6 servos with serial interface
//Created with love by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

/* -------------------------------------------------------------------------- */
/*                          GENERIC GLOBAL VARIABLES                          */
/* -------------------------------------------------------------------------- */

int speed = 500;        //global speed of stepper motor step time

String motornum;        //what number motor we are operating
String motordir;        //motor direction
String motortime;       //how long the motor should take to do the operation
String motordist;       //how far the motor is traveling in degrees

int sep1,sep2,sep3,sep4,sep5;   //all the seperating locations used to split up serial command

String incomingdata = "";       //string that holds the incoming data from the serial port

int pos[] = {0,0,0,0,0,0,0};    //array to store all the servo positions


/* -------------------------------------------------------------------------- */
/*                       STEPPER MOTOR WORKING VARIABLES                      */
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/*                           SERVO WORKING VARIABLES                          */
/* -------------------------------------------------------------------------- */

bool SV1STATE = false;      //state of the Servo motor
bool SV1CHANGEEN;           //Enable a degree state change for the next check
int SV1DEG = 0;             //Requested servo angle
unsigned long SV1START;     //servo move start time
unsigned long SV1END;       //servo move end time
bool SV1A;                  //Servo event 1 completion status
bool SV1B;                  //Servo event 2 completion status
unsigned long SV1TIME;      //offset appled to the difference of SVSTART and SVEND

bool SV2STATE = false;
bool SV2CHANGEEN;
int SV2DEG = 0;
unsigned long SV2START;
unsigned long SV2END;
bool SV2A;
bool SV2B;
unsigned long SV2TIME;

bool SV3STATE = false;
bool SV3CHANGEEN;
int SV3DEG = 0;
unsigned long SV3START;
unsigned long SV3END;
bool SV3A;
bool SV3B;
unsigned long SV3TIME;

bool SV4STATE = false;
bool SV4CHANGEEN;
int SV4DEG = 0;
unsigned long SV4START;
unsigned long SV4END;
bool SV4A;
bool SV4B;
unsigned long SV4TIME;

bool SV5STATE = false;
bool SV5CHANGEEN;
int SV5DEG = 0;
unsigned long SV5START;
unsigned long SV5END;
bool SV5A;
bool SV5B;
unsigned long SV5TIME;

bool SV6STATE = false;
bool SV6CHANGEEN;
int SV6DEG = 0;
unsigned long SV6START;
unsigned long SV6END;
bool SV6A;
bool SV6B;
unsigned long SV6TIME;

/* -------------------------------------------------------------------------- */
/*                                PROGRAM SETUP                               */
/* -------------------------------------------------------------------------- */

void setup(){                   //all the functions that run once when the MCU us turned on
    Serial.begin(115200);       //start serial

                            //set all stepper pins to output
    pinMode(S1_S,OUTPUT);   //
    pinMode(S1_D,OUTPUT);   //
    pinMode(S2_S,OUTPUT);   //
    pinMode(S2_D,OUTPUT);   //
    pinMode(S3_S,OUTPUT);   //
    pinMode(S3_D,OUTPUT);   //
    pinMode(S4_S,OUTPUT);   //
    pinMode(S4_D,OUTPUT);   //
    pinMode(S5_S,OUTPUT);   //
    pinMode(S5_D,OUTPUT);   //
    pinMode(S6_S,OUTPUT);   //
    pinMode(S6_D,OUTPUT);   //

    servo1.attach(A5);      //attach servos to their pins
    servo2.attach(A0);      //
    servo3.attach(A4);      //
    servo4.attach(A3);      //
    servo5.attach(A2);      //
    servo6.attach(A1);      //
    servo1.write(0);        //make sure all of the servos are locked in the 0 deg position
    servo2.write(0);        //
    servo3.write(0);        //
    servo4.write(0);        //
    servo5.write(0);        //
    servo6.write(0);        //
}

/* -------------------------------------------------------------------------- */
/*                              MAIN PROGRAM LOOP                             */
/* -------------------------------------------------------------------------- */

void loop(){
    updateS1();             //update the stepper position / status based on the last data 
    updateS2();             //from serial commands
    updateS3();             //
    updateS4();             //
    updateS5();             //
    updateS6();             //

    updateSV1();            //update the servo position / status based on the last data
    updateSV2();            //from serial commands
    updateSV3();            // 
    updateSV4();            //
    updateSV5();            //
    updateSV6();            //
}

/* -------------------------------------------------------------------------- */
/*                           SERIAL EVENT DETECTION                           */
/* -------------------------------------------------------------------------- */

void serialEvent(){                                     //Function that gets serial data and updates the command variables
    while(Serial.available()){                          //checks if serial is available
        char inChar = (char)Serial.read();              //read the serial data
        incomingdata += inChar;                         //add the serial data to the incoming data variable
        delayMicroseconds(150);                         //delay so that the serial data does not get broken up
    }
    //Serial.println(incomingdata);                     //print the incoming data

    sep1 = incomingdata.indexOf(":");                   //set the position of all 5 seperators
    sep2 = incomingdata.indexOf(":",sep1+1);            //
    sep3 = incomingdata.indexOf(":",sep2+1);            //
    sep4 = incomingdata.indexOf(":",sep3+1);            //
    sep5 = incomingdata.indexOf(":",sep4+1);            //
    
    motornum = incomingdata.substring(sep1+1,sep2);     //set the motor number from the serial data
    motordir = incomingdata.substring(sep2+1,sep3);     //set the motor direction from the serial data (applies only to steppers)
    motortime = incomingdata.substring(sep3+1,sep4);    //sets the desired time for action to take from serial data
    motordist = incomingdata.substring(sep4+1,sep5);    //sets the desired motor distance in degrees from serial data

/* -------------------------------------------------------------------------- */
/*                     SET WORKING VARS FOR STEPPER MOTORS                    */
/* -------------------------------------------------------------------------- */

    if(let_to_num(motornum) == 1){                      //check what the desired motor number is and if its a stepper based on alpha serial data
        S1STPC = 0;                                     //set the stepper motor current step counter to 0
        S1ST2D = 0;                                     //set the stepper motor steps to do counter to 0
        S1ST2D = calcsteps(motordist.toInt());          //set the stepper motor steps to do to the number of steps calculated by the degrees in serial data
        S1SPDOFF = calcspeed(S1ST2D);                   //calculate and set the speed offset to make stepper arrived at desired time
        if(motordir == "F"){                            //check the desired motor directon
            S1DIR = true;                               //set the direction to true if the desired direction is forwards
        }                               
        
        else if(motordir == "B"){                            //check the desired motor direction
            S1DIR = false;                              //set the direction to false if the desired direction is backwards
        }
    }

    else if(let_to_num(motornum) == 2){                      //same function as above
        S2STPC = 0;                                     //
        S2ST2D = 0;                                     //
        S2ST2D = calcsteps(motordist.toInt());          //
        S2SPDOFF = calcspeed(S2ST2D);                   //
        if(motordir == "F"){                            //
            S2DIR = true;                               //
        }
        
        else if(motordir == "B"){
            S2DIR = false;
        }
    }

    else if(let_to_num(motornum) == 3){
        S3STPC = 0;
        S3ST2D = 0;
        S3ST2D = calcsteps(motordist.toInt());
        S3SPDOFF = calcspeed(S3ST2D);
        if(motordir == "F"){
            S3DIR = true;
        }
        
        else if(motordir == "B"){
            S3DIR = false;
        }
    }

    else if(let_to_num(motornum) == 4){
        S4STPC = 0;
        S4ST2D = 0;
        S4ST2D = calcsteps(motordist.toInt());
        S4SPDOFF = calcspeed(S4ST2D);
        if(motordir == "F"){
            S4DIR = true;
        }
        
        else if(motordir == "B"){
            S4DIR = false;
        }
    }

    else if(let_to_num(motornum) == 5){
        S5STPC = 0;
        S5ST2D = 0;
        S5ST2D = calcsteps(motordist.toInt());
        S5SPDOFF = calcspeed(S5ST2D);
        if(motordir == "F"){
            S5DIR = true;
        }
        
        else if(motordir == "B"){
            S5DIR = false;
        }
    }

    else if(let_to_num(motornum) == 6){
        S6STPC = 0;
        S6ST2D = 0;
        S6ST2D = calcsteps(motordist.toInt());
        S6SPDOFF = calcspeed(S6ST2D);
        if(motordir == "F"){
            S6DIR = true;
        }
        
        else if(motordir == "B"){
            S6DIR = false;
        }
    }

/* -------------------------------------------------------------------------- */
/*                         SET WORKING VARS FOR SERVOS                        */
/* -------------------------------------------------------------------------- */

    if(let_to_num(motornum) == 1+6){
        SV1DEG = (motordist.toInt()/3)*2;   //Servos are 270 deg servos while the library is 180 based to compensate we have to calculate 2/3 of the desired angle 
        long diff = SV1DEG - pos[1];
        diff = abs(diff);
        SV1TIME = motortime.toInt() / diff;
    }

    else if(let_to_num(motornum) == 2+6){
        SV2DEG = (motordist.toInt()/3)*2;
        long diff = SV2DEG - pos[2];
        diff = abs(diff);
        SV2TIME = motortime.toInt() / diff;
    }
    else if(let_to_num(motornum) == 3+6){
        SV3DEG = (motordist.toInt()/3)*2;
        long diff = SV3DEG - pos[3];
        diff = abs(diff);
        SV3TIME = motortime.toInt() / diff;
    }
    else if(let_to_num(motornum) == 4+6){
        SV4DEG = (motordist.toInt()/3)*2;
        long diff = SV4DEG - pos[4];
        diff = abs(diff);
        SV4TIME = motortime.toInt() / diff;
    }
    else if(let_to_num(motornum) == 5+6){
        SV5DEG = (motordist.toInt()/3)*2;
        long diff = SV5DEG - pos[5];
        diff = abs(diff);
        SV5TIME = motortime.toInt() / diff;
    }
    else if(let_to_num(motornum) == 6+6){
        SV6DEG = (motordist.toInt()/3)*2;
        long diff = SV6DEG - pos[6];
        diff = abs(diff);
        SV6TIME = motortime.toInt() / diff;
    }

    incomingdata = "";                                  //reset the incoming data variable
}

/* -------------------------------------------------------------------------- */
/*                         FUNCTIONS TO UPDATE SERVOS                         */
/* -------------------------------------------------------------------------- */

void updateSV1(){                                       //function to update the stepper motor state
    if(SV1STATE == false){
        // Serial.println("update start");
        SV1START = currentmicro();                      //set the starting time to the current microsecond
        SV1END = SV1START + SV1TIME * 1000;             //set the ending time to the start time plus the offset time
        // Serial.println((SV1TIME * 1000));
        SV1STATE = true;                                //set the servo activity state to active
        SV1A = false;                                   //set the servo event 1 state to false
        SV1B = false;                                   //set the servo event 2 state to false
        SV1CHANGEEN = true;                             //set the enable change flag to true to enable servo angle update
    }

    if(currentmicro() >= SV1START && currentmicro() <= SV1END && SV1A == false && SV1CHANGEEN == true){ //check if it should update the sevo position
        if(pos[1] <= SV1DEG){                           //check if the current servo position is less than what it should be
            pos[1] = pos[1] + 1;                        //if the position is less than what it should be increase angle
            servo1.write(pos[1]);                       //tell ther servo what position it should be at
        }
        if(pos[1] > SV1DEG){                            //check if the serbo position is more than what it should be
            pos[1] = pos[1] - 1;
            servo1.write(pos[1]);
        }
        SV1CHANGEEN = false;
        SV1A == true;
    }

    if(currentmicro() >= SV1END && SV1B == false){
        SV1B == true;
        SV1STATE = false;
        // Serial.println("update end");
        // Serial.println(pos[1]);
    }
}

void updateSV2(){
    if(SV2STATE == false){
        // Serial.println("update start");
        SV2START = currentmicro();
        SV2END = SV2START + SV2TIME * 1000;
        // Serial.println((SV1TIME * 1000));
        SV2STATE = true;
        SV2A = false;
        SV2B = false;
        SV2CHANGEEN = true;
    }

    if(currentmicro() >= SV2START && currentmicro() <= SV2END && SV2A == false && SV2CHANGEEN == true){
        if(pos[2] <= SV2DEG){
            pos[2] = pos[2] + 1;
            servo2.write(pos[2]);
        }
        if(pos[2] > SV2DEG){
            pos[2] = pos[2] - 1;
            servo2.write(pos[2]);
        }
        SV2CHANGEEN = false;
        SV2A == true;
    }

    if(currentmicro() >= SV2END && SV2B == false){
        SV2B == true;
        SV2STATE = false;
        // Serial.println("update end");
        // Serial.println(pos[2]);
    }
}

void updateSV3(){
    if(SV3STATE == false){
        // Serial.println("update start");
        SV3START = currentmicro();
        SV3END = SV3START + SV3TIME * 1000;
        // Serial.println((SV1TIME * 1000));
        SV3STATE = true;
        SV3A = false;
        SV3B = false;
        SV3CHANGEEN = true;
    }

    if(currentmicro() >= SV3START && currentmicro() <= SV3END && SV3A == false && SV3CHANGEEN == true){
        if(pos[3] <= SV3DEG){
            pos[3] = pos[3] + 1;
            servo3.write(pos[3]);
        }
        if(pos[3] > SV3DEG){
            pos[3] = pos[3] - 1;
            servo3.write(pos[3]);
        }
        SV3CHANGEEN = false;
        SV3A == true;
    }

    if(currentmicro() >= SV3END && SV3B == false){
        SV3B == true;
        SV3STATE = false;
        // Serial.println("update end");
        // Serial.println(pos[3]);
    }
}

void updateSV4(){
    if(SV4STATE == false){
        // Serial.println("update start");
        SV4START = currentmicro();
        SV4END = SV4START + SV4TIME * 1000;
        // Serial.println((SV1TIME * 1000));
        SV4STATE = true;
        SV4A = false;
        SV4B = false;
        SV4CHANGEEN = true;
    }

    if(currentmicro() >= SV4START && currentmicro() <= SV4END && SV4A == false && SV4CHANGEEN == true){
        if(pos[4] <= SV4DEG){
            pos[4] = pos[4] + 1;
            servo4.write(pos[4]);
        }
        if(pos[4] > SV4DEG){
            pos[4] = pos[4] - 1;
            servo4.write(pos[4]);
        }
        SV4CHANGEEN = false;
        SV4A == true;
    }

    if(currentmicro() >= SV4END && SV4B == false){
        SV4B == true;
        SV4STATE = false;
        // Serial.println("update end");
        // Serial.println(pos[4]);
    }
}

void updateSV5(){
    if(SV5STATE == false){
        // Serial.println("update start");
        SV5START = currentmicro();
        SV5END = SV5START + SV5TIME * 1000;
        // Serial.println((SV1TIME * 1000));
        SV5STATE = true;
        SV5A = false;
        SV5B = false;
        SV5CHANGEEN = true;
    }

    if(currentmicro() >= SV5START && currentmicro() <= SV5END && SV5A == false && SV5CHANGEEN == true){
        if(pos[5] <= SV5DEG){
            pos[5] = pos[5] + 1;
            servo5.write(pos[5]);
        }
        if(pos[5] > SV5DEG){
            pos[5] = pos[5] - 1;
            servo5.write(pos[5]);
        }
        SV5CHANGEEN = false;
        SV5A == true;
    }

    if(currentmicro() >= SV5END && SV5B == false){
        SV5B == true;
        SV5STATE = false;
        // Serial.println("update end");
        // Serial.println(pos[5]);
    }
}

void updateSV6(){
    if(SV6STATE == false){
        // Serial.println("update start");
        SV6START = currentmicro();
        SV6END = SV6START + SV6TIME * 1000;
        // Serial.println((SV1TIME * 1000));
        SV6STATE = true;
        SV6A = false;
        SV6B = false;
        SV6CHANGEEN = true;
    }

    if(currentmicro() >= SV6START && currentmicro() <= SV6END && SV6A == false && SV6CHANGEEN == true){
        if(pos[6] <= SV6DEG){
            pos[6] = pos[6] + 1;
            servo6.write(pos[6]);
        }
        if(pos[6] > SV6DEG){
            pos[6] = pos[6] - 1;
            servo6.write(pos[6]);
        }
        SV6CHANGEEN = false;
        SV6A == true;
    }

    if(currentmicro() >= SV6END && SV6B == false){
        SV6B == true;
        SV6STATE = false;
        // Serial.println("update end");
        // Serial.println(pos[6]);
    }
}

/* -------------------------------------------------------------------------- */
/*                       UPDATE STEPPER MOTOR FUNCTIONS                       */
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/*                              UTILITY FUNCTIONS                             */
/* -------------------------------------------------------------------------- */

int let_to_num(String motorNumber){     //function to convert the motor letter to the motor number
    int motor;
    if (motorNumber == "A"){
        motor = 1;
    }
    else if (motorNumber == "B"){
        motor = 2;
    }
    else if (motorNumber == "C"){
        motor = 3;
    }
    else if (motorNumber == "D"){
        motor = 4;
    }
    else if (motorNumber == "E"){
        motor = 5;
    }
    else if (motorNumber == "F"){
        motor = 6;
    }
    else if (motorNumber == "G"){
        motor = 7;
    }
    else if (motorNumber == "H"){
        motor = 8;
    }
    else if (motorNumber == "I"){
        motor = 9;
    }
    else if (motorNumber == "J"){
        motor = 10;
    }
    else if (motorNumber == "K"){
        motor = 11;
    }
    else if (motorNumber == "L"){
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
    speedoffset = 2 * ((requestedspeed * 500) - (steps_ * speed)) / steps_;    //calculate the offset based on current working variables
    if(speedoffset < 0){                                //if the offset is smaller than the minumum required time make the offset 0 aka max speed
        speedoffset = 0;
    }
    return(speedoffset);
}

unsigned long currentmicro(){
    return(micros());
}