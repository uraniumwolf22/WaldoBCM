/*Holds all of the definitons that cause clutter in main code*/



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