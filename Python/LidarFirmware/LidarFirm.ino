#define motorPWM 2

void setup(){
    pinMode(motorPWM, OUTPUT);
}

void loop(){
    digitalWrite(motorPWM,HIGH);
}