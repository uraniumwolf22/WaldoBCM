#pragma once



struct Stepperdata {
	bool SSTATE[6];            //Stepper state
	bool SDIR[6];              //Stepper direction
	bool SA[6];                //Stepper checkpoint A
	bool SB[6];                //Stepper checkpoint B
	bool SC[6];                //Stepper checkpoint C

	unsigned long SSPDOFF[6];  //Stepper speed offset
	unsigned long SSTART[6];   //Stepper start time
	unsigned long SMID[6];     //Stepper midpoint time
	unsigned long SEND[6];     //Stepper end time

	int SSTPC[6];              //Stepper current step count
	int SST2D[6];              //Stepper steps to do
};

struct Servodata {

	int pos[6];                //Current ideal position of all servos
	bool SVCHANGEEN[6];        //Servo change enabled
	bool SVSTATE[6];           //Servo state
	bool SVA[6];               //Servo checkpoint A
	bool SVB[6];               //Servo checkpoint B

	unsigned long SVSTART[6];  //Servo start time
	unsigned long SVEND[6];    //Servo end time
	unsigned long SVTIME[6];   //servo time

	int SVDEG[6];              //Servo current deg
};