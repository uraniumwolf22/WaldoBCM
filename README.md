#Waldo Hexapod

This site contains all the code for the Waldo Hexapod robot.

Test python GUI is located in the Python folder.
You can locate the C firmware for the BCM under Hardware_BCM/BCM.ino
You can locate the lidar software under LiDar System.

#Hardware

The hardware schematics can be found in schematics as well as a full PCB layout
The chip used as the BCM needs to be an AVR chip (328p or 2560)
Main board input voltage needs to be ~16V not below 12V and no more than 26V.
Active cooling on the stepper motor chips and the servo VRMs is needed

#Known issues
--328p resets on COM initiation needs to have a 20uf capacitor from reset to ground to prevent this.
--After 70 minutes the micros timer register will overflow and set to 0 potentially breaking the operation of the bot
is currently on
--CAPS NEED TO BE 24V NOT 16V OR YOUR EARS WILL REGRET IT
--Steppers always have holding torque
