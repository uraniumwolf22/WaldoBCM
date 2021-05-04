**WaldoBCM**


**PYTHON SERVER**
Required python packages:
-Pyserial

open the serial port only once.  every time it opens something in my C code makes all the stepper motors
freak out.  still working on that issue but for now we need to open the serial port and keep it open.
Also remember whenever there is not a command not being sent to the BCM over serial all steppers have automatic holding torque

**C CLIENT**

the movement mapping for the arduino is as follows:
the direction you want to move followed by "t" to terminate the current serial command
an example of this would be "ft"  to move forward for "bt"  to move backwards.

the 5 current movement commands are f,b,l,r,h for forward back left right and halt.