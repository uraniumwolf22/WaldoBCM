#include "pinmapping.h"
#include "Servo.h"
#include"motors.h"
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

int ServoInitialPosition = 0;                                       //initial servo Servodata.position

bool SteppersActive = false;                                        //Are Steppers active
bool ServosActive = false;                                          //Are servos active

String incomingdata = "";                                           //Store incoming data

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
    servo1.write(ServoInitialPosition);        //make sure all of the servos are locked in the 0 deg Servodata.position
    servo2.write(ServoInitialPosition);
    servo3.write(ServoInitialPosition);
    servo4.write(ServoInitialPosition);
    servo5.write(ServoInitialPosition);
    servo6.write(ServoInitialPosition);
}
//====================================================================================================================
void loop() {
    Serial.print("SERVO1 POS= ");
    Serial.println(Servodata.pos[0]);


    for (int i = 0; i <= 5; i++) {     //Update all of the servos
        UpdateServo(i);
    }

    for (int i = 0; i <= 5; i++) {     //Update all of the steppers
        UpdateStepper(i);
    }
}
//====================================================================================================================
void serialEvent() {                    //Get the command
    Serial.println("Serial event detected");
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        delayMicroseconds(150);
    }

    sep1 = incomingdata.indexOf(":");                   //set the Servodata.position of all 5 seperators
    sep2 = incomingdata.indexOf(":", sep1 + 1);            //
    sep3 = incomingdata.indexOf(":", sep2 + 1);            //
    sep4 = incomingdata.indexOf(":", sep3 + 1);            //
    sep5 = incomingdata.indexOf(":", sep4 + 1);            //

    motornum = incomingdata.substring(sep1 + 1, sep2);     //set the motor number from the serial data
    motordir = incomingdata.substring(sep2 + 1, sep3);     //set the motor direction from the serial data (applies only to steppers)
    motortime = incomingdata.substring(sep3 + 1, sep4);    //sets the desired time for action to take from serial data
    motordist = incomingdata.substring(sep4 + 1, sep5);    //sets the desired motor distance in degrees from serial data

    Serial.println(motornum);
    Serial.println(motordir);
    Serial.println(motortime);
    Serial.println(motordist);
    //==================================================================================================================================
    MotorToUpdate = let_to_num(motornum) - 1;

    if (MotorToUpdate <= 5) {                               //Check if the requested motor is in the stepper range
        Stepperdata.SSTPC[MotorToUpdate] = 0;
        Stepperdata.SST2D[MotorToUpdate] = calcsteps(motordist.toInt());            //calculate how many steps need to be done
        Stepperdata.SSPDOFF[MotorToUpdate] = calcspeed(Stepperdata.SST2D[MotorToUpdate]);       //calculate how long it needs to take for each step

        if (motordir == "F") {                                          //Check if the motor direction is forward
            Stepperdata.SDIR[MotorToUpdate] = true;
        }
        else if (motordir == "B") {                                     //Check if the motor direction is backwards
            Stepperdata.SDIR[MotorToUpdate] = false;
        }
    }
    //================================================================================================================================
    if (MotorToUpdate > 5) {                                            //Check if the requested motor is in the servo range
        Servodata.SVDEG[MotorToUpdate - 6] = (motordist.toInt() / 3) * 2;
        long diff = Servodata.SVDEG[MotorToUpdate - 6] - Servodata.pos[MotorToUpdate - 6];
        diff = abs(diff);
        Servodata.SVTIME[MotorToUpdate - 6] = motortime.toInt() / diff;
    }
    incomingdata = "";                                                  //Reset the incoming data
}
//====================================================================================================================================
void UpdateServo(int SVNUM) {                                               //Update the servo
    if (Servodata.SVSTATE[SVNUM] == false) {
        Servodata.SVSTART[SVNUM] = currentmicro();
        Servodata.SVEND[SVNUM] = Servodata.SVSTART[SVNUM] + Servodata.SVTIME[SVNUM] * 1000;

        //DEBUGGING STUFF DONT UNCOMMENT IN PRODUCTION CAUSES DELAY DUM DUM

         //Serial.print("TIME: ");
         //Serial.println(Servodata.SVTIME[SVNUM]);

         //Serial.print("START: ");
         //Serial.println(Servodata.SVSTART[SVNUM]);

         //Serial.print("END: ");
         //Serial.println(Servodata.SVEND[SVNUM]);

        Servodata.SVSTATE[SVNUM] = true;
        Servodata.SVA[SVNUM] = false;
        Servodata.SVB[SVNUM] = false;
        Servodata.SVCHANGEEN[SVNUM] = true;
    }
    if (currentmicro() >= Servodata.SVSTART[SVNUM] && currentmicro() <= Servodata.SVEND[SVNUM] && Servodata.SVA[SVNUM] == false && Servodata.SVCHANGEEN[SVNUM] == true) {      //Stage 1 servo logic
        if (Servodata.pos[SVNUM] <= Servodata.SVDEG) {
            Servodata.pos[SVNUM] = Servodata.pos[SVNUM] + 1;
            WriteServo(SVNUM, Servodata.pos[SVNUM]);
        }
        else if (Servodata.pos[SVNUM] > Servodata.SVDEG[SVNUM]) {
            Servodata.pos[SVNUM] = Servodata.pos[SVNUM] - 1;
            WriteServo(SVNUM, Servodata.pos[SVNUM]);
        }
        Servodata.SVCHANGEEN[SVNUM] = false;
        Servodata.SVA[SVNUM] = true;
    }
    if (currentmicro() >= Servodata.SVEND[SVNUM] && Servodata.SVB[SVNUM] == false) {                    //Stage 2 servo logic
        Servodata.SVB[SVNUM] = true;
        Servodata.SVSTATE[SVNUM] = false;
    }
}
//====================================================================================================================================
void UpdateStepper(int SNUM) {                                                      //update the stepper motor
    if (Stepperdata.SST2D[SNUM] > Stepperdata.SSTPC[SNUM]) {
        if (Stepperdata.SSTATE[SNUM] == false) {
            Stepperdata.SSTART[SNUM] = currentmicro();                              //Calculate the Stepper stage times
            Stepperdata.SMID[SNUM] = currentmicro() + speed;
            Stepperdata.SEND[SNUM] = Stepperdata.SMID[SNUM] + speed + (Stepperdata.SSPDOFF[SNUM] * 1000);
            Stepperdata.SSTATE[SNUM] = true;
            Stepperdata.SA[SNUM] = false;
            Stepperdata.SB[SNUM] = false;
            Stepperdata.SC[SNUM] = false;
        }
        if (currentmicro() >= Stepperdata.SSTART[SNUM] && currentmicro() <= Stepperdata.SMID[SNUM] && Stepperdata.SA[SNUM] == false) {      //Stage 1 stepper logic
            if (Stepperdata.SDIR[SNUM] == true) {
                SwitchStepperDirection(SNUM, true);
            }
            if (Stepperdata.SDIR[SNUM] == false) {
                SwitchStepperDirection(SNUM, false);
            }
            StepControl(SNUM, true);
            Stepperdata.SA[SNUM] = true;
        }
        if (currentmicro() >= Stepperdata.SMID[SNUM] && currentmicro() <= Stepperdata.SEND[SNUM] && Stepperdata.SB[SNUM] == false) {        //Stage 2 stepper logic
            StepControl(SNUM, false);
            Stepperdata.SB[SNUM] = true;
        }
        if (currentmicro() >= Stepperdata.SEND[SNUM] && Stepperdata.SC[SNUM] == false) {            //Stage 3 stepper logic
            Stepperdata.SSTATE[SNUM] = false;
            Stepperdata.SC[SNUM] = true;
            Stepperdata.SSTPC[SNUM]++;
        }
    }
}
//=====================================================================================================================================
void StepControl(int num, bool action) {
    if (num == 0) {
        if (action == true) { digitalWrite(S1_S, HIGH); }
        else { digitalWrite(S1_S, LOW); }
    }
    else if (num == 1) {
        if (action == true) { digitalWrite(S2_S, HIGH); }
        else { digitalWrite(S2_S, LOW); }
    }
    else if (num == 2) {
        if (action == true) { digitalWrite(S3_S, HIGH); }
        else { digitalWrite(S3_S, LOW); }
    }
    else if (num == 3) {
        if (action == true) { digitalWrite(S4_S, HIGH); }
        else { digitalWrite(S4_S, LOW); }
    }
    else if (num == 4) {
        if (action == true) { digitalWrite(S5_S, HIGH); }
        else { digitalWrite(S5_S, LOW); }
    }
    else if (num == 5) {
        if (action == true) { digitalWrite(S6_S, HIGH); }
        else { digitalWrite(S6_S, LOW); }
    }
}
//=====================================================================================================================================

void SwitchStepperDirecton(int num, bool dir) {
    switch (num) {
    case 0:
        if (dir == true) { digitalWrite(S1_D, HIGH); }
        else { digitalWrite(S1_D, LOW); }

    case 1:
        if (dir == true) { digitalWrite(S2_D, HIGH); }
        else { digitalWrite(S2_D, LOW); }

    case 2:
        if (dir == true) { digitalWrite(S3_D, HIGH); }
        else { digitalWrite(S3_D, LOW); }

    case 3:
        if (dir == true) { digitalWrite(S4_D, HIGH); }
        else { digitalWrite(S4_D, HIGH); }

    case 4:
        if (dir == true) { digitalWrite(S5_D, HIGH); }
        else { digitalWrite(S5_D, LOW); }

    case 5:
        if (dir == true) { digitalWrite(S6_D, HIGH); }
        else { digitalWrite(S6_D, LOW); }
    }
}
//===================================================================================================================================
void WriteServo(int num, int Servodata.pos) {
    if (num == 0) { servo1.write(Servodata.pos); }
    else if (num == 1) { servo2.write(Servodata.pos); }
    else if (num == 2) { servo3.write(Servodata.pos); }
    else if (num == 3) { servo4.write(Servodata.pos); }
    else if (num == 4) { servo5.write(Servodata.pos); }
    else if (num == 5) { servo6.write(Servodata.pos); }
}
//==================================================================================================================================
int let_to_num(String motorNumber) {     //function to convert the motor letter to the motor number
    int motor;
    if (motorNumber == "A") { motor = 1; }
    else if (motorNumber == "B") { motor = 2; }
    else if (motorNumber == "C") { motor = 3; }
    else if (motorNumber == "D") { motor = 4; }
    else if (motorNumber == "E") { motor = 5; }
    else if (motorNumber == "F") { motor = 6; }
    else if (motorNumber == "G") { motor = 7; }
    else if (motorNumber == "H") { motor = 8; }
    else if (motorNumber == "I") { motor = 9; }
    else if (motorNumber == "J") { motor = 10; }
    else if (motorNumber == "K") { motor = 11; }
    else if (motorNumber == "L") { motor = 12; }
    return motor;
}
//==================================================================================================================================
int calcsteps(int deg) {                 //calculate how many steps the servo needs to take based on degrees
    int steps;
    steps = deg / 1.8;
    return(steps);
}
//=================================================================================================================================
unsigned long calcspeed(int steps_) {
    unsigned long speedoffset;                            //calculate the offset to achieve desired stepper time
    long requestedspeed = motortime.toInt();            //convert the offset time to a long so we can work with numbers over 36000
    speedoffset = 2 * ((requestedspeed * 500) - (steps_ * speed)) / steps_;    //calculate the offset based on current working variables
    if (speedoffset < 0) {                                //if the offset is smaller than the minumum required time make the offset 0 aka max speed
        speedoffset = 0;
    }
    return(speedoffset);
}
//=================================================================================================================================
unsigned long currentmicro() {
    return(micros());
}
//=================================================================================================================================