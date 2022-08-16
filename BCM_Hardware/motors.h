#pragma once



struct Stepperdata {
	bool SSTATE[] = { false,false,false,false,false,false };            //Stepper state
	bool SDIR[] = { false,false,false,false,false,false };              //Stepper direction
	bool SA[] = { false,false,false,false,false,false };                //Stepper checkpoint A
	bool SB[] = { false,false,false,false,false,false };                //Stepper checkpoint B
	bool SC[] = { false,false,false,false,false,false };                //Stepper checkpoint C

	unsigned long SSPDOFF[] = { 0,0,0,0,0,0 };                          //Stepper speed offset
	unsigned long SSTART[] = { 0,0,0,0,0,0 };                           //Stepper start time
	unsigned long SMID[] = { 0,0,0,0,0,0 };                             //Stepper midpoint time
	unsigned long SEND[] = { 0,0,0,0,0,0 };                             //Stepper end time

	int SSTPC[] = { 0,0,0,0,0,0 };                                      //Stepper current step count
	int SST2D[] = { 0,0,0,0,0,0 };                                      //Stepper steps to do
};

struct Servodata {

	int pos[] = { 0,0,0,0,0,0 };                                          //Current ideal position of all servos
	bool SVCHANGEEN[] = { false,false,false,false,false,false };        //Servo change enabled
	bool SVSTATE[] = { false,false,false,false,false,false };           //Servo state
	bool SVA[] = { false,false,false,false,false,false };               //Servo checkpoint A
	bool SVB[] = { false,false,false,false,false,false };               //Servo checkpoint B

	unsigned long SVSTART[] = { 0,0,0,0,0,0 };                          //Servo start time
	unsigned long SVEND[] = { 0,0,0,0,0,0 };                            //Servo end time
	unsigned long SVTIME[] = { 0,0,0,0,0,0 };                              //servo time

	int SVDEG[] = { 0,0,0,0,0,0 };                                      //Servo current deg
};