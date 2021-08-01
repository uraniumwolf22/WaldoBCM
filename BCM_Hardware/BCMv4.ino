#include "pinmapping.h"
#include <servo.h>

/// <summary>
/// Waldo BCM controller V4 By logan Ross
/// Licensed under MIT.
/// 
/// Waldo BCM Firmware controls the Asyncrinous functions of 6 servos and 6 Stepper motors
/// via Serial interface 115200 Baud.  Parameters include Step speed Step size and direction
/// </summary>

int speed = 500;                                                    //Inital speed of motors

String motortime;
String motordist;
String motornum;
String motordir;

int sep1, sep2, sep3, sep4, sep5;

int pos[] = {0,0,0,0,0,0};                                        //Current ideal position of all servos
int ServoInitialPosition = 0;                                       //initial servo position

bool SteppersActive = false;                                        //Are Steppers active
bool ServosActive = false;                                          //Are servos active

String incomingdata = "";                                           //Store incoming data

bool SVCHANGEEN[] = { false,false,false,false,false,false };        //Servo change enabled
bool SVSTATE[] = { false,false,false,false,false,false };           //Servo state
bool SVA[] = { false,false,false,false,false,false };               //Servo checkpoint A
bool SVB[] = { false,false,false,false,false,false };               //Servo checkpoint B

unsigned long SVSTART[] = { 0,0,0,0,0,0 };                          //Servo start time
unsigned long SVEND[] = { 0,0,0,0,0,0 };                            //Servo end time
unsigned long SVTIME[] = {0,0,0,0,0,0}                              //servo time

int SVDEG[] = { 0,0,0,0,0,0 };                                      //Servo current deg

bool SSTATE[] = { false,false,false,false,false,false };            //Stepper state
bool SDIR[] = { false,false,false,false,false,false };              //Stepper direction
bool SA[] = { false,false,false,false,false,false };                //Stepper checkpoint A
bool SB[] = { false,false,false,false,false,false };                //Stepper checkpoint B
bool SC[] = { false,false,false,false,false,false };                //Stepper checkpoint C

unsigned long SSPDOFF[] = { 0,0,0,0,0,0 };                          //Stepper speed offset
unsigned long SSTART[] = { 0,0,0,0,0,0 };                           //Stepper start time
unsigned long SMID[] = { 0,0,0,0,0,0 };                             //Stepper midpoint time
unsigned long SEND[] = { 0,0,0,0,0,0 };                             //Stepper end time

int SSTPC[] = { 0,0,0,0,0,0 };                                      //Stepper current step count
int SST2D[] = { 0,0,0,0,0,0 };                                      //Stepper steps to do

int MotorToUpdate;

//====================================================================================================

void setup() {                   //all the functions that run once when the MCU us turned on
    Serial.begin(115200);       //start serial

                              //set all stepper pins to output
    pinMode(S1_S, OUTPUT);
    pinMode(S1_D, OUTPUT);
    pinMode(S2_S, OUTPUT);
    pinMode(S2_D, OUTPUT);
    pinMode(S3_S, OUTPUT);
    pinMode(S3_D, OUTPUT);
    pinMode(S4_S, OUTPUT);
    pinMode(S4_D, OUTPUT);
    pinMode(S5_S, OUTPUT);
    pinMode(S5_D, OUTPUT);
    pinMode(S6_S, OUTPUT);
    pinMode(S6_D, OUTPUT);
    servo1.attach(A5);      //attach servos to their pins
    servo2.attach(A0);
    servo3.attach(A4);
    servo4.attach(A3);
    servo5.attach(A2);
    servo6.attach(A1);
    servo1.write(ServoInitialPosition);        //make sure all of the servos are locked in the 0 deg position
    servo2.write(ServoInitialPosition);
    servo3.write(ServoInitialPosition);
    servo4.write(ServoInitialPosition);
    servo5.write(ServoInitialPosition);
    servo6.write(ServoInitialPosition);
}
//====================================================================================================================
void loop() {
    while (int i = 0; i <= 5 i++) {     //Update all of the servos
        UpdateServo(i);}

    while (int i = 0; i <= 5 i++) {     //Update all of the steppers
        UpdateStepper(i);}
}
//====================================================================================================================
void Serialevent() {                    //Get the command
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        delayMicroseconds(150);
    }

    sep1 = incomingdata.indexOf(":");                   //set the position of all 5 seperators
    sep2 = incomingdata.indexOf(":", sep1 + 1);            //
    sep3 = incomingdata.indexOf(":", sep2 + 1);            //
    sep4 = incomingdata.indexOf(":", sep3 + 1);            //
    sep5 = incomingdata.indexOf(":", sep4 + 1);            //

    motornum = incomingdata.substring(sep1 + 1, sep2);     //set the motor number from the serial data
    motordir = incomingdata.substring(sep2 + 1, sep3);     //set the motor direction from the serial data (applies only to steppers)
    motortime = incomingdata.substring(sep3 + 1, sep4);    //sets the desired time for action to take from serial data
    motordist = incomingdata.substring(sep4 + 1, sep5);    //sets the desired motor distance in degrees from serial data
    //==================================================================================================================================
    MotorToUpdate = let_to_num(motornum) - 1;

    if (MotorToUpdate <= 5) {                               //Check if the requested motor is in the stepper range
        SSTPC[MotorToUpdate] = 0;
        SST2D[MotorToUpdate] = calcsteps(motordist.toInt());            //calculate how many steps need to be done
        SSPDOFF[MotorToUpdate] = calcspeed(SST2D[MotorToUpdate]);       //calculate how long it needs to take for each step

        if (motordir == "F") {                                          //Check if the motor direction is forward
            SDIR[MotorToUpdate] = true;
        }
        else if (motordir == "B") {                                     //Check if the motor direction is backwards
            SDIR[MotorToUpdate] = false;
        }
    }
    //================================================================================================================================
    if (MotorToUpdate > 5) {                                            //Check if the requested motor is in the servo range
        SVDEG[MotorToUpdate - 6] = (motordist.toInt() / 3) * 2;
        long diff = SVDEG[MotorToUpdate - 6] - pos[MotorToUpdate - 6];
        diff = abs(diff);
        SVTIME[MotorToUpdate - 6] = motortime.toInt() / diff;
    }
    incomingdata = "";                                                  //Reset the incoming data
}
//====================================================================================================================================
void UpdateServo(int SVNUM) {                                               //Update the servo
    if (SVSTATE[SVNUM] == false) {
        SVSTART[SVNUM] = currentmicro();
        SVEND[SVNUM] = SVSTART[SVNUM] + SVTIME[SVNUM] * 1000;
        SVSTATE[SVNUM] = true;
        SVA[SVNUM] = false;
        SVB[SVNUM] = false;
        SVCHANGEEN[SVNUM] = true;
    }
    if (currentmicro() >= STSTART[SVNUM] && currentmicro() <= SVEND[SVNUM] && SVA[SVNUM] == false && SVCHANGEEN[SVNUM] == true){      //Stage 1 servo logic
        if (pos[SVNUM] <= SVDEG) {
            pos[SVNUM] = pos[SVNUM] + 1;
            WriteServo(SVNUM, pos[SVNUM]);
        }
        else if (pos[SVNUM] > SVDEG[SVNUM]) {
            pos[SVNUM] = pos[SVNUM] - 2;
            WriteServo(SVNUM,pos[SVNUM])
        }
        SVCHANGEEN[SVNUM] = false;
        SVA[SVNUM] = true;
    }
    if (currentmicro() >= SVEND[SVNUM] && SVB[SVNUM] == false) {                    //Stage 2 servo logic
        SVB[SVNUM] = true;
        SVSTATE[SVNUM] = false;
    }
}
//====================================================================================================================================
void UpdateStepper(int SNUM) {                                                      //update the stepper motor
    if (SST2D[SNUM] > SSTPC[SNUM]) {
        if (SSTATE[SNUM] == false) {
            SSTART[SNUM] = currentmicro();                              //Calculate the Stepper stage times
            SMID[SNUM] = currentmicro() + speed;
            SEND[SNUM] = SMID[SNUM] + speed + (SSPDOFF[SNUM] * 1000);
            SSTATE[SNUM] = true;
            SA[SNUM] = false;
            SB[SNUM] = false;
            SC[SNUM] = false;
        }
        if (currentmicro() >= SSTART[SNUM] && currentmicro() <= SMID[SNUM] && SA[SNUM] == false) {      //Stage 1 stepper logic
            if (SDIR[SNUM] == true) {
                SwitchStepperDirection(SNUM, true);
            }
            if (SDIR[SNUM] == false) {
                SwitchStepperDirection(SNUM, false);
            }
            StepControl(SNUM, true);
            SA[SNUM] = true;
        }
        if (currentmicro() >= SMID[SNUM] && currentmicro() <= SEND[SNUM] && SB[SNUM] == false) {        //Stage 2 stepper logic
            StepControl(SNUM, false);
            SB[SNUM] = true;
        }
        if (currentmicro() >= SEND[SNUM] && SC[SNUM] == false) {            //Stage 3 stepper logic
            SSTATE[SNUM] = false;
            SC[SNUM] = true;
            SSTPC[SNUM]++;
        }
    }
}
//=====================================================================================================================================
void StepControl(int num, bool action) {
    if (num == 0) {
        if (action == true) {digitalWrite(S1_S, HIGH);}
        else {digitalWrite(S1_S, LOW)}}
    else if (num == 1) {
        if (action == true) {digitalWrite(S2_S, HIGH);}
        else {digitalWrite(S2_S, LOW);}}
    else if (num == 2) {
        if (action == true) {digitalWrite(S3_S, HIGH);}
        else {digitalWrite(S3_S, LOW);}}
    else if (num == 3) {
        if (action == true) {digitalWrite(S4_S, HIGH);}
        else {digitalWrite(S4_S, LOW);}}
    else if (num == 4) {
        if (action == true) {digitalWrite(S5_S, HIGH);}
        else {digitalWrite(S5_S, LOW);}}
    else if (num == 5) {
        if (action == true) {digitalWrite(S6_S, HIGH);}
        else {digitalWrite(S6_S, LOW);}}
}
//=====================================================================================================================================
void SwitchStepperDirection(int num, bool dir) {
    if (num == 0) {
        if (dir == true) {digitalWrite(S1_D, HIGH);}
        else {digitalWrite(S1_D, LOW);}}
    else if (num == 1) {
        if (dir == true) {digitalWrite(S2_D, HIGH);}
        else {digitalWrite(S2_D, LOW);}}
    else if (num == 2) {
        if (dir == true) {digitalWrite(S3_D, HIGH);}
        else {digitalWrite(S3_D, LOW);}}
    else if (num == 3) {
        if (dir == true) {digitalWrite(S4_D, HIGH);}
        else {digitalWrite(S4_D, HIGH);}}
    else if (num == 4) {
        if (dir == true) {digitalWrite(S5_D, HIGH);}
        else {digitalWrite(S5_D, LOW);}}
    else if (num == 5) {
        if (dir == true) {digitalWrite(S6_D, HIGH);}
        else {digitalWrite(S6_D, LOW);}}
}
//===================================================================================================================================
void WriteServo(int num, int pos) {
    if (num == 0) {servo1.write(pos);}
    else if (num == 1) {servo2.write(pos);}
    else if (num == 2) {servo3.write(pos);}
    else if (num == 3) {servo4.write(pos);}
    else if (num == 4) {servo5.write(pos);}
    else if (num == 5) {servo6.write(pos);}
}
//==================================================================================================================================
int let_to_num(String motorNumber){     //function to convert the motor letter to the motor number
    int motor;
    if (motorNumber == "A"){motor = 1;}
    else if (motorNumber == "B"){motor = 2;}
    else if (motorNumber == "C"){motor = 3;}
    else if (motorNumber == "D"){motor = 4;}
    else if (motorNumber == "E"){motor = 5;}
    else if (motorNumber == "F"){motor = 6;}
    else if (motorNumber == "G"){motor = 7;}
    else if (motorNumber == "H"){motor = 8;}
    else if (motorNumber == "I"){motor = 9;}
    else if (motorNumber == "J"){motor = 10;}
    else if (motorNumber == "K"){motor = 11;}
    else if (motorNumber == "L"){motor = 12;}
    return motor;
}
//==================================================================================================================================
int calcsteps(int deg){                 //calculate how many steps the servo needs to take based on degrees
    int steps;
    steps = deg/1.8;
    return(steps);
}
//=================================================================================================================================
unsigned long calcspeed(int steps_){
    unsigned long speedoffset;                            //calculate the offset to achieve desired stepper time
    long requestedspeed = motortime.toInt();            //convert the offset time to a long so we can work with numbers over 36000
    speedoffset = 2 * ((requestedspeed * 500) - (steps_ * speed)) / steps_;    //calculate the offset based on current working variables
    if(speedoffset < 0){                                //if the offset is smaller than the minumum required time make the offset 0 aka max speed
        speedoffset = 0;}
    return(speedoffset);
}
//=================================================================================================================================
unsigned long currentmicro(){
    return(micros());}
//=================================================================================================================================