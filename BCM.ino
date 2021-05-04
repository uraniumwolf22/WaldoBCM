//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3
//testing

#include<Servo.h>

//Stepper motor pin mapping
#define S1_S 7
#define S1_D 6
#define S2_S 3
#define S2_D 2
#define S3_S 10
#define S3_D 11
#define S4_S 5
#define S4_D 4
#define S5_S 13
#define S5_D 12
#define S6_S 9
#define S6_D 8
//init servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
//init servo positions
int SPOS1 = 0;
int SPOS2 = 0;
int SPOS3 = 0;
int SPOS4 = 0;
int SPOS5 = 0;
int SPOS6 = 0;

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
        forward();
    }

    if(bcmstate == 2){
        calcspeed(1);
        backward();
    }

    if(bcmstate == 3){
        calcspeed(1);
        left();
    }

    if(bcmstate == 4){
        calcspeed(1);
        right();
    }
}
//Detects if a serial event has taken place and changes the BCMstate accordingly
void serialEvent(){             //check serial and get data
    while(Serial.available()){
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        }
    
                            //change state according to recieved serial data
    if(incomingdata == "w"){
        bcmstate = 1;
    }

    if(incomingdata == "s"){
        bcmstate = 2;
    }

    if(incomingdata == "a"){
        bcmstate = 3;
    }

    if(incomingdata == "d"){
        bcmstate = 4;
    }
    incomingdata = "";
}


int calcspeed(int actions){         //calculate the timing needed to speed stepper motors based on global speed and number of actions at once
    speed = globalspeed / actions;
    return(0);
}

void forward(){
    //Set motor directions
    digitalWrite(S1_D,HIGH);
    digitalWrite(S2_D,HIGH);
    //step motors once
    digitalWrite(S1_S,HIGH);
    digitalWrite(S2_S,HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S,LOW);
    digitalWrite(S2_S,LOW);
    delayMicroseconds(speed);
}

void backward(){
    //Set motor directions
    digitalWrite(S1_D,LOW);
    digitalWrite(S2_D,LOW);
    //step motors once
    digitalWrite(S1_S,HIGH);
    digitalWrite(S2_S,HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S,LOW);
    digitalWrite(S2_S,LOW);
    delayMicroseconds(speed);
}

void right(){
    //Set motor directions
    digitalWrite(S1_D,HIGH);
    digitalWrite(S2_D,LOW);
    //step motors once
    digitalWrite(S1_S,HIGH);
    digitalWrite(S2_S,HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S,LOW);
    digitalWrite(S2_S,LOW);
    delayMicroseconds(speed);
}

void left(){
    //Set motor directions
    digitalWrite(S1_D,LOW);
    digitalWrite(S2_D,HIGH);
    //step motors once
    digitalWrite(S1_S,HIGH);
    digitalWrite(S2_S,HIGH);
    delayMicroseconds(speed);
    digitalWrite(S1_S,LOW);
    digitalWrite(S2_S,LOW);
    delayMicroseconds(speed);
}

void headleft(){
    digitalWrite(S3_D,HIGH);

    digitalWrite(S3_S,HIGH);
    delayMicroseconds(speed);
    digitalWrite(S3_S,LOW);
    delayMicroseconds(speed);
}
void headright(){
    digitalWrite(S3_D,LOW);

    digitalWrite(S3_S,HIGH);
    delayMicroseconds(speed);
    digitalWrite(S3_S,LOW);
    delayMicroseconds(speed);
}