# WaldoBCM
# PYTHON SERVER


Required python packages:

-Pyserial
-PySimpleGUI



# BCM Firmware

Movement commands are as follows.

S A A F

1 `MOTOR TYPE`: Defines the type of motor,  __S__ for Stepper or __V__ for Servo.

2 `MOTOR NUMBER`: Defines which motor to control, A-F coorospond to different motors on the BCM.

3 `MOTOR RESOLUTION`: Defines the motor resolution.  Only applies to steppers and can be A - D or 1,5,10,100 steps.

4 `MOTOR DIRECTION`: Defines which direction the motors should be moving.  F for forward and B for Backwards.

Note: BCM Will send the character X when the move has been completed across serial.
:trollface: 🍆
