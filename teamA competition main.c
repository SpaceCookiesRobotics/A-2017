#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,	 leftGrabber,		 sensorQuadEncoderOnI2CPort,		, AutoAssign )
#pragma config(Sensor, I2C_2,	 rightGrabber,	 sensorQuadEncoderOnI2CPort,		, AutoAssign )
#pragma config(Motor,	 port1,						frontLeft,		 tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,	 port2,						bottomLeft,		 tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,	 port3,						bottomRight,	 tmotorVex393_MC29, openLoop)
#pragma config(Motor,	 port4,						rightGrabber,	 tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,	 port5,						backLeft,			 tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,	 port6,						frontRight,		 tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,	 port7,						leftGrabber,	 tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,	 port8,						topLeft,			 tmotorVex393_MC29, openLoop)
#pragma config(Motor,	 port9,						topRight,			 tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,	 port10,					backRight,		 tmotorVex393_HBridge, openLoop, reversed)

int speedDriver = 1; // divider for slow speed functionality
int speedGrabber = 1;
// continuously monitor the slow speed button
// when the slow speed button is pressed, change the speed dividor speedDriver
// normal speeds are divided by this value
task chassisSlow(){
	while(true){
	//clearLCDLine(0);
	//clearLCDLine(1);
	// if main driver presses button 5 Up, go at half speed
	if (vexRT[Btn5U]==1){
		speedDriver = 2;
		//displayLCDNumber(1,0,77);
		}
	else{
		speedDriver = 1;
		//displayLCDNumber(1,0,88);
	}
	// if partner presses button 8 up, grabber goes at half speed
	if (vexRT[Btn8UXmtr2]==1){
		speedGrabber = 2;
	}
	else{
		speedGrabber = 1;
	}
	displayLCDNumber(0,0,speedDriver);

  }//while
};

void closeLeft(void){ motor[leftGrabber] = 127/speedGrabber;}
void openLeft(void){ motor[leftGrabber] = -127/speedGrabber;}
void closeRight(void){ motor[rightGrabber] = 127/speedGrabber;}
void openRight(void){ motor[rightGrabber] = -127/speedGrabber;}


//open grabber completely at start, to give the grabber a starting position
void homeFinger(int mot,int enc){
	//last value variable is equal to the encoder value
	int lastVal = nMotorEncoder(enc);
	// still moving true/false variable is true
	bool stillMoving = true;
	// turn motor on
	motor[mot] = 127;
	// for as long as still moving variable is true (second given premise)
	while (stillMoving){
		// pause for ten milliseconds to let encoder gather information
		sleep(100);
		// current value variable is equal to the encoder value
		int curVal = nMotorEncoder(enc);
		// still moving variable is true when current value does not equal last value
		stillMoving = (curVal != lastVal);
		lastVal=curVal;
		// exit while true loop
	}
	// make this measured position as "zero" (to base off of for next commands)
	nMotorEncoder(enc) = 0;
	// turn motor off
	motor[mot] = 0;
}

// grabbers grab object
void snatch(void){
	//
	int lastRightVal = nMotorEncoder(2);
	//
	int lastLeftVal = nMotorEncoder(1);
	//
	bool stillMoving = true;
	// turn motor on
	closeLeft();//motor[leftGrabber] = -127/speedGrabber;
	//
	closeRight(); //motor[rightGrabber] = -127/speedGrabber;
	//
	while (stillMoving){
		// pause for ten milliseconds to let encoder gather information
		sleep(100);
		// current value variable is equal to the encoder value
		int curLeftVal = nMotorEncoder(1);
		//
		int curRightVal = nMotorEncoder(2);
		//
		stillMoving = (curLeftVal != lastLeftVal) && (curRightVal != lastRightVal);
		lastLeftVal=curLeftVal;
		lastRightVal=curRightVal;
		// exit while true loop
	}
	// make this measured position as "zero" (to grip well on cone)
	motor[rightGrabber] = -20;
	motor[leftGrabber] = -20;
}

void fingerToPosn(int command){
	int curPosn=nMotorEncoder(1);
	while( abs(curPosn-command)>10){
		int toMotor= (int)(0.01*(curPosn-command));
		motor[leftGrabber] = toMotor;
		motor[rightGrabber] = toMotor;
		curPosn=nMotorEncoder(1);
	}//end while loop
}


task main()
//homeFinger(leftGrabber,1);
{

	startTask (chassisSlow); // monitor slow speed buttons

	while(true){
displayLCDNumber(1,0,speedDriver);
	//chassis
	motor[frontLeft] = vexRT[Ch3]/speedDriver;
	motor[backLeft] = vexRT[Ch3]/speedDriver;
	motor[frontRight] = vexRT[Ch2]/speedDriver;
	motor[backRight] = vexRT[Ch2]/speedDriver;
	//lift motors
	motor[bottomLeft] = vexRT[Ch2Xmtr2]/speedGrabber;
	motor[bottomRight] = vexRT[Ch2Xmtr2]/speedGrabber;
	motor[topLeft] = vexRT[Ch2Xmtr2]/speedGrabber;
	motor[topRight] = vexRT[Ch2Xmtr2]/speedGrabber;
	//grabber motors
	// up button=close, down button = open
	if (vexRT[Btn5UXmtr2] &&! vexRT[Btn5DXmtr2]) {
		closeLeft();
	};
	if (vexRT[Btn5DXmtr2] &&! vexRT[Btn5UXmtr2]) {
		openLeft();
	};
	if (!(vexRT[Btn5UXmtr2] || vexRT[Btn5DXmtr2])) {
		motor[leftGrabber] = 0;
	};
	if (vexRT[Btn6UXmtr2] && !vexRT[Btn6DXmtr2]) {
		closeRight();
	};
	if (vexRT[Btn6DXmtr2] && !vexRT[Btn6UXmtr2]) {
		openRight();
	};
	if (!(vexRT[Btn6UXmtr2] || vexRT[Btn6DXmtr2])) {
		motor[rightGrabber] = 0;
	};
	if (vexRT[Btn7UXmtr2]){
		homeFinger(leftGrabber,1);
		homeFinger(rightGrabber,2);
	};
	if (vexRT[Btn7DXmtr2]){
		snatch();
	};
	if (vexRT[Btn8DXmtr2]){
		fingerToPosn(-460);
	};
}//while
}//taskMain
