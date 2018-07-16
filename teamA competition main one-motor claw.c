#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl2,  tip,            sensorTouch)
#pragma config(Sensor, dgtl9,  jumper,         sensorDigitalIn)
#pragma config(Sensor, dgtl10, led,            sensorDigitalOut)
#pragma config(Sensor, dgtl11, ,               sensorDigitalOut)
#pragma config(Sensor, dgtl12, ,               sensorDigitalOut)
#pragma config(Sensor, I2C_1,  rightRear,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           frontLeft,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           bottomLeft,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           bottomRight,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           grabber,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           backLeft,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           frontRight,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           topLeft,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           topRight,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          backRight,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../Shared/Competition.h"

int speedDriver = 1; // divider for slow speed on chassis
int speedGrabber = 1;

bool isClosed = false;
// continuously monitor the slow speed button
// when the slow speed button is pressed, change the speed dividor speedDriver
// normal speeds are divided by this value
task chassisSlow(){
	while(true){
		// if main driver presses button 5 Up, go at half speed
		if (vexRT[Btn5U]==1){
			speedDriver = 2;
			// chassis speed to half
		}
		else{
			speedDriver = 1;
			//otherwise normal speed
		}
		// if partner presses button 8 up, grabber and lift go at half speed
		if (vexRT[Btn8UXmtr2]==1){
			speedGrabber = 3;
			// grabber and lift speed to half
		}
		else{
			speedGrabber = 1;
			// otherwise normal grabber and lift speed
		}
		//displayLCDNumber(0,0,speedDriver);
	}//while
};
//grabber motor
//void closeGrabber(void){ motor[grabber] = 127/speedGrabber;}
//void openGrabber(void){ motor[grabber] = -127/speedGrabber;}
void closeGrabber(void){
	isClosed = true;
	motor[grabber] = -70;
}
void openGrabber(void){
	isClosed = false;
	motor[grabber] = 70;
}
void relaxGrabber(void){
	if (isClosed) {
		motor[grabber] = -30;
		} else {
		motor[grabber] = 0;
	}
}

task runGrabber () {
	// up button = open, down button = close
	//button for grabber to close
	while (true) {
		if (vexRT[Btn6UXmtr2] && !vexRT[Btn6DXmtr2]) {
			closeGrabber();
		};//end if
		//button for grabber to open
		if (vexRT[Btn6DXmtr2] && !vexRT[Btn6UXmtr2]) {
			openGrabber();
		};
		// if you aren't pressing either button, then motor relaxes (power 0)
		if (!vexRT[Btn6DXmtr2] && !vexRT[Btn6UXmtr2]) {
			relaxGrabber();
		};
	};
};

//for autonomous
void riseLift(int time, int power){
	//left side up
	motor[bottomLeft] =	power;
	motor[topLeft] =	power;
	//right side up
	motor[bottomRight] =	power;
	motor[topRight] =	power;
	//time it takes for lift to get to the top
	wait10Msec(time);
	//left side stop
	motor[bottomLeft] =	0;
	motor[topLeft] =	0;
	//right side time
	motor[bottomRight] =	0;
	motor[topRight] =	0;
}
void fallLift(int time){
	//left side down
	motor[bottomLeft] =	-127;
	motor[topLeft] =	-127;
	//right side down
	motor[bottomRight] =	-127;
	motor[topRight] =	-127;
	//time it takes for lift to fall to sationary goal
	wait10Msec(time);
	//left side stop
	motor[bottomLeft] =	0;
	motor[topLeft] =	0;
	//right side stop
	motor[bottomRight] = 0;
	motor[topRight] =	0;
}

void startForwards(void) {
	//left side forwards
	motor[backLeft] =	127;
	motor[frontLeft] =	127;
	//right side forwards
	motor[backRight] =	127;
	motor[frontRight] =	127;
}
void startBackwards(void) {
	//left side backward
	motor[backLeft] =	-127;
	motor[frontLeft] =	-127;
	//right side backward
	motor[backRight] =	-127;
	motor[frontRight] =	-127;
}
void stopDriving(void){
	//left side stop
	motor[backLeft] =	0;
	motor[frontLeft] =	0;
	//right side stop
	motor[backRight] =	0;
	motor[frontRight] =	0;
}
//integrated motor encoder
const float wheelDiaInches=4;
const float ticksPerRevolution=627.2;
const float pi=3.1415926535;
const float circumference=pi*wheelDiaInches;

void driveForwDistance(int distInches) {
	resetMotorEncoder(backRight);
	//writeDebugStream("enocoder reset to: %d\n", getMotorEncoder(backRight));
	const float wheelTurns=distInches/circumference;
	float distanceTicks= ticksPerRevolution*wheelTurns;
	startForwards();
	//writeDebugStream("distanceTicks: %d\n", (int) distanceTicks);
	while(getMotorEncoder(backRight)*(-1) <= (int)distanceTicks)
	{ wait1Msec(5);
		//writeDebugStream("encoder: %d\n", getMotorEncoder(backRight));
	}; //waits for the right Rear to run for x amount of rotations on the chassis
	stopDriving();

}

void driveBackDistance(int distInches) {
	resetMotorEncoder(backRight);
	writeDebugStream("[driveBackDistance] enocoder reset to: %d\n", getMotorEncoder(backRight));
	startBackwards();
	const float wheelTurns=distInches/circumference;
	float distanceTicks= ticksPerRevolution*wheelTurns;
	writeDebugStream("[driveBackDistance] distanceTicks backwards: %d\n", (int) distanceTicks);
	while(getMotorEncoder(backRight) <= (int)distanceTicks){
		writeDebugStream("[driveBackDistance] encoder back:%d\n", getMotorEncoder(backRight));
	}; //waits for the right Rear to run for x amount of rotations on the chassis
	stopDriving();
}

void driveForwards(int time) {
	startForwards();
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}
void driveBackwards (int time){
	startBackwards();
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}
void turnLeft (int time, int speed=127){
	//left side backward
	motor[backLeft] =	-speed;
	motor[frontLeft] =	-speed;
	//right side forward
	motor[backRight] =	speed;
	motor[frontRight] =	speed;
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}
void turnRight (int time, int speed=127){
	//left side forward
	motor[backLeft] =	speed;
	motor[frontLeft] =	speed;
	//right side backward
	motor[backRight] =	-speed;
	motor[frontRight] =	-speed;
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}

//this function will not send back new numbers
void joystick(){
	displayLCDString(0,0,"start");

	startTask(chassisSlow); // monitor slow speed buttons
	//grabber motors
	startTask(runGrabber); //check for grabber buttons
	//continue forever
	while(true){
		//displayLCDNumber(1,0,speedDriver);
		//chassis motors
	//	bool isTipping = (SensorValue[dgtl2]==0);
		//if (isTipping &! vexRT[Btn6U]){
			// run back wheels backwards to prevent the robot from tipping
		//	motor[backLeft] = -127;
		//	motor[backRight] = -127;
		//}
	//	else {
		//	motor[backLeft] = vexRT[Ch3]/speedDriver;
		//	motor[backRight] = vexRT[Ch2]/speedDriver;
		//}
		motor[frontLeft] = vexRT[Ch3]/speedDriver;
		motor[frontRight] = vexRT[Ch2]/speedDriver;
		motor[backLeft] = vexRT[Ch3]/speedDriver;
		motor[backRight] = vexRT[Ch2]/speedDriver;

		//lift motors
		//with slow speed function on 8U
		motor[bottomLeft] = vexRT[Ch2Xmtr2]/speedGrabber;
		motor[bottomRight] = vexRT[Ch2Xmtr2]/speedGrabber;
		motor[topLeft] = vexRT[Ch2Xmtr2]/speedGrabber;
		motor[topRight] = vexRT[Ch2Xmtr2]/speedGrabber;

	}//exit while loop
}//end taskMain

/*
//How to do structs in Robot C
typedef struct  {
int waitStart;
bool turnDir;
float turnAngle;
} AutonomousParameterType;

AutonomousParameterType leftRed;
AutonomousParameterType rightRed;

void initParams(){
// static struct initialization is not supported
leftRed.waitStart=100;
leftRed.turnDir=true;

rightRed.waitStart=234;
rightRed.turnDir=false;
}

AutonomousParameterType runParams=leftRed;
runParams = rightRed;
// inside your code, you then use
wait10Msec( runParams.waitStart );
*/

void scorePresetCone(){
	//scoring on stationary goals
	//driving lift down
	//fallLift(50);//runs for 0.5 seconds -removed because there is nothing in the way of the grabber any more
	//grab cone
	closeGrabber(); //grabCone();
	wait10Msec(35); //wait for claw to move
	//lift up all the way
	riseLift(200, 127);//drives lift up for 2 seconds
	//drive forwards
	//driveForwards(105);//runs for 1.05 second
	driveForwDistance(20);
	//drop down to stationary goal
	fallLift(125);//drives lift down for 1.25 seconds
	//release cone
	openGrabber(); //releaseCone();
	wait10Msec(60);//runs for 0.6 seconds so claw can open
	relaxGrabber();
	//drive backwards
	//driveBackwards(55);
	driveBackDistance(10);
}

void scoreLeftCone(bool switchAuton) {
	//drive forward
	//driveForwards(50);  //completely unecessary
	fallLift(175); //drives lift down for 2.5  - fallLift is called at the end of ScorePresetCone()
	//turn left
	if(switchAuton){
		turnRight(45);//runs for 0.5 seconds
	}
	else{
		turnLeft(45);//runs for 0.5 seconds
	}
	//move forward
	//driveForwards(60);//drives forward for 0.6 seconds
	driveForwDistance(10);
	//grab cone
	closeGrabber(); //grabCone();
	wait10Msec(35);//runs for 10 second for the claw to close
	//turn right
	//rise lift
	riseLift(20, 127); //runs for 0.2 seconds to ensure that the cone if recieved
	if(switchAuton) {
		turnLeft(60); //runs for 0.7 seconds
	}
	else{
		turnRight(60);//runs for 0.7 seconds
	}
	wait10Msec(1);//pauses inbetween the two actions
	//lift up all the way
	riseLift(250, 127);//dirves lift up for 2.5 seconds at full motor power
	//wait10Msec(1);//pauses for 10 milliseconds so they don't run together
	//driveForwards(63);//drives forward for 0.63 seconds
	driveForwDistance(8);
	//drop down to stationary goal
	fallLift(130);//drives lift down for 1.25 seconds
	//release cone
	openGrabber();
	wait10Msec(60);//runs for 0.60 seconds so claw can open
	relaxGrabber();
	//drive backwards
	//driveBackwards(100);//drives back for 1 second
	driveBackDistance(10);
}

void autonomous() {
	//	displayLCDString(0,0,"a-mouse");
	//driveForwDistance(12);

	//switching autonomous
	bool switchAuton = (SensorValue[dgtl9]==1); //when jumper is in digital port 9
	// turn led on so you know you got the jumper in right
	SensorValue[dgtl10]= SensorValue[dgtl9];
	if (SensorValue[dgtl11] == 1) {
		//to drive out of the way of our alliances autonomous
		//driveForwards (75);
		driveForwDistance(12);

	}
	else  {
		//wait10Msec(550);//waits for partner to score
		scorePresetCone();
		wait10Msec(1);
		scoreLeftCone(switchAuton);
	}
}//end autonomous
