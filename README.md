#Waldo Hexapod

This site contains all the code for the Waldo robot.

Test python GUI is located in the Python folder.
You can locate the C firmware for the ATMEL 328P.
You can locate the lidar software in LiDar System.

#Hardware

The hardware schematics can be found in schematics as well as a full PCB layout
The chip used as the BCM needs to be atleast 8MHZ with 2K of RAM 4K would be best
Main board input voltage needs to be ~24V not below 16V and no more than 26V.
Active cooling on the stepper motor chips and the servo VRMs is needed

#Known issues
--Arduino resets on COM initiation needs to have a 20uf capacitor from reset to ground to prevent this.
--After 70 minutes the micros register will overflow and set to 0 potentially breaking the operation the bot
is currently on
--CAPS NEED TO BE 24V NOT 16V OR YOUR EARS WILL REGRET IT
--Steppers always have holding torque
