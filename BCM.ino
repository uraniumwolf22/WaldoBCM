//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"
int globalspeed = 350 ;  //Set global stepper motor speed
int speed = 0;  //stepper motor speed working value

int bcmstate = 0;   //the current mode of the motors
String incomingdata = "";

void setup(){
    //set up serial communication
    incomingdata.reserve(200);
    Serial.begin(9600);
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

/*in the loop it is consistantly moving the motors one step in the right direction
based on the bcmstate variable.  we recalculate the time of each step every time.
this is because if we have a 3 move step the time for the step to take place is actually 3 times greater
than it would be if there was only 1 step so we calculate the true stepping speed based on the global stepping speed
using the calcspeed function */

void loop(){
    if(bcmstate == 1){      
        calcspeed(1);
        forward();}

    if(bcmstate == 2){
        calcspeed(1);
        backward();}

    if(bcmstate == 3){
        calcspeed(1);
        left();}

    if(bcmstate == 4){
        calcspeed(1);
        right();}
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

    if(incomingdata == "b"){
        bcmstate = 2;}

    if(incomingdata == "l"){
        bcmstate = 3;}

    if(incomingdata == "r"){
        bcmstate = 4;}

    if(incomingdata == "h"){
        halt();
    }
    incomingdata = "";
}


int calcspeed(int actions){         //calculate the timing needed to speed stepper motors based on global speed and number of actions at once
    speed = globalspeed / actions;
    return(0);
}

// functions that were cluttering main file

void forward() {
    //Set motor directions
    digitalWrite(S1_D, HIGH);
    digitalWrite(S2_D, HIGH);
    //step motors once
    digitalWrite(S1_S, HIGH);
    digitalWrite(S2_S, HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S, LOW);
    digitalWrite(S2_S, LOW);
    delayMicroseconds(speed);
}

void backward() {
    //Set motor directions
    digitalWrite(S1_D, LOW);
    digitalWrite(S2_D, LOW);
    //step motors once
    digitalWrite(S1_S, HIGH);
    digitalWrite(S2_S, HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S, LOW);
    digitalWrite(S2_S, LOW);
    delayMicroseconds(speed);
}

void right() {
    //Set motor directions
    digitalWrite(S1_D, HIGH);
    digitalWrite(S2_D, LOW);
    //step motors once
    digitalWrite(S1_S, HIGH);
    digitalWrite(S2_S, HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S, LOW);
    digitalWrite(S2_S, LOW);
    delayMicroseconds(speed);
}

void left() {
    //Set motor directions
    digitalWrite(S1_D, LOW);
    digitalWrite(S2_D, HIGH);
    //step motors once
    digitalWrite(S1_S, HIGH);
    digitalWrite(S2_S, HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S, LOW);
    digitalWrite(S2_S, LOW);
    delayMicroseconds(speed);
}

void headleft() {
    digitalWrite(S3_D, HIGH);

    digitalWrite(S3_S, HIGH);
    delayMicroseconds(speed);
    digitalWrite(S3_S, LOW);
    delayMicroseconds(speed);
}
void headright() {
    digitalWrite(S3_D, LOW);
    digitalWrite(S3_S, HIGH);
    delayMicroseconds(speed);
    digitalWrite(S3_S, LOW);
    delayMicroseconds(speed);
}

void halt(){
    bcmstate = 0;
    //This will contain more later once there is more going on with servos/ect
}