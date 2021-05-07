//Waldo Body Control Module serial control software
//Created with care by Logan Ross <3

#include<Servo.h>
#include "pinmapping.h"

int speed = 500;        //global speed of stepper motor step time
long speedoffset = 0;   //speed offset to lengthen stepper time

String motornum;        //what number motor we are operating
String motordir;        //motor direction
String motortime;       //how long the motor should take to do the operation
String motordist;       //how far the motor is traveling in degrees

int steps_;             //Steps working var
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
    //servo pin mapping
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


void serialEvent(){                                     //function that collects and parses the serial data
    while(Serial.available()){
        char inChar = (char)Serial.read();
        incomingdata += inChar;
        delayMicroseconds(100);                         //this delay has to be here to keep the data from not being broken up
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

    incomingdata = "";                                  //reset the incoming data variable
    actionready = true;                                 //set the action ready flag so the main loop knows there is a command to execute
}


void loop(){                                            //main loop (where the magic happens)

    if(actionready && let_to_num(motornum) < 7){        //check if there is an action ready and if the command is for the stepper motors

        steps_ = calcsteps(motordist.toInt());          //calculate the steps from the degrees
        motor_ = let_to_num(motornum);                  //get the motor number from the motor letter

        if(motordir == "F"){                            //set the motor direction based on the command
            direction_ = 1;
        }
        if(motordir == "B"){
            direction_ = 0;
        }

        calcspeed();                                    //calculate the offset to achieve the target time based on the current working variables
        executestepcommand(steps_,motor_,direction_);   //execute the stepper motor function

        actionready = false;                            //reset the action ready flag
    }

    if(actionready && let_to_num(motornum) > 6){        //check if there is an action ready and chek if the motor type is a servo

        servonum = let_to_num(motornum) - 6;            //store the servo number based on the letter and subtract 6 so that is only in the scope of servos
        servodeg_ = motordist.toInt();                  //store the servo degrees
        servotime_ = motortime.toInt();                 //set the servo requested time

        executeservocommand(servodeg_,servotime_,servonum);     //Execute the servo command
        actionready = false;                            //reset the action ready flag
    }
}


int calcspeed(){                                        //calculate the offset to achieve desired stepper time
    long requestedspeed = motortime.toInt();            //convert the offset time to a long so we can work with numbers over 36000
    speedoffset = ((requestedspeed - (steps_ * ((speed * 2)/1000)))/steps_);    //calculate the offset based on current working variables
    if(speedoffset < 0){                                //if the offset is smaller than the minumum required time make the offset 0 aka max speed
        speedoffset = 0;
    }
}


int calcsteps(int deg){                 //calculate how many steps the servo needs to take based on degrees
    int steps;
    steps = deg/1.8;
    return(steps);
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

int executeservocommand(int deg,int time,int servonum){         //functions to command the servos
    int diff = deg - pos[servonum];
    diff = abs(diff);
    int delay = time / diff;
    if(pos[servonum] <= deg){                                   //check if the desired position is smaller than the current position
        for(int i = pos[servonum]; i <= deg; i++){              //itterate until we have the desired angle
            if(servonum == 1){
                pos[servonum] = pos[servonum] + 1;              //increase the position variable
                servo1.write(pos[servonum]);                    //tell the servo to move to the next position
            }
            if(servonum == 2){
                pos[servonum] = pos[servonum] + 1;
                servo2.write(pos[servonum]);
            }
            if(servonum == 3){
                pos[servonum] = pos[servonum] + 1;
                servo3.write(pos[servonum]);
            }
            if(servonum == 4){
                pos[servonum] = pos[servonum] + 1;
                servo4.write(pos[servonum]);
            }
            if(servonum == 5){
                pos[servonum] = pos[servonum] + 1;
                servo5.write(pos[servonum]);
            }
            if(servonum == 6){
                pos[servonum] = pos[servonum] + 1;
                servo6.write(pos[servonum]);
            }   
            delay(delay);                                         //wait
        }
    }


    if(pos[servonum] > deg){                                   //check if the desired position is larger than the current position
        for(int i = pos[servonum]; i >= deg; i--){
            if(servonum == 1){
                pos[servonum] = pos[servonum] - 1;
                servo1.write(pos[servonum]);
            }
            if(servonum == 2){
                pos[servonum] = pos[servonum] - 1;
                servo2.write(pos[servonum]);
            }
            if(servonum == 3){
                pos[servonum] = pos[servonum] - 1;
                servo3.write(pos[servonum]);
            }
            if(servonum == 4){
                pos[servonum] = pos[servonum] - 1;
                servo4.write(pos[servonum]);
            }
            if(servonum == 5){
                pos[servonum] = pos[servonum] - 1;
                servo5.write(pos[servonum]);
            }
            if(servonum == 6){
                pos[servonum] = pos[servonum] - 1;
                servo6.write(pos[servonum]);
            }
            delay(delay;
        }
    }
}

int executestepcommand(int res, int motor, int dir){        //execute step function based on the resolution the motor number and the direction
    if(motor == 1){
        for(int i = 0; i < res; i++){                       //step as many times as needed to achive desired angle
            if(dir == 1){                                   //check the wanted direction
                digitalWrite(S1_D,HIGH);                    //set directon pin
                digitalWrite(S1_S,HIGH);                    //turn on the step pin
                delayMicroseconds(speed);                   //delay the global speed time
                digitalWrite(S1_S,LOW);                     //turn off the speed pin
                delayMicroseconds(speed);                   //delay the global speed time
                delay(speedoffset);                         //wait the offset to make the stepper motor reach the desired time
            }

            if(dir == 0){
                digitalWrite(S1_D,LOW);
                digitalWrite(S1_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S1_S,LOW);
                delayMicroseconds(speed);
                delay(speedoffset);
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
                delay(speedoffset);
            }
            if(dir == 0){
                digitalWrite(S2_D,LOW);
                digitalWrite(S2_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S2_S,LOW);
                delayMicroseconds(speed);
                delay(speedoffset);
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
                delay(speedoffset);
            }
            if(dir == 0){
                digitalWrite(S3_D,LOW);
                digitalWrite(S3_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S3_S,LOW);
                delayMicroseconds(speed);
                delay(speedoffset);
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
                delay(speedoffset);
            }
            if(dir == 0){
                digitalWrite(S4_D,LOW);
                digitalWrite(S4_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S4_S,LOW);
                delayMicroseconds(speed);
                delay(speedoffset);
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
                delay(speedoffset);
            }
            if(dir == 0){
                digitalWrite(S5_D,LOW);
                digitalWrite(S5_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S5_S,LOW);
                delayMicroseconds(speed);
                delay(speedoffset);
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
                delay(speedoffset);
            }
            if(dir == 0){
                digitalWrite(S6_D,LOW);
                digitalWrite(S6_S,HIGH);
                delayMicroseconds(speed);
                digitalWrite(S6_S,LOW);
                delayMicroseconds(speed);
                delay(speedoffset);
            }
        }
    }
}
