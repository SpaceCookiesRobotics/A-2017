#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl2,  tip,            sensorTouch)
#pragma config(Sensor, dgtl11, ,               sensorDigitalOut)
#pragma config(Sensor, dgtl12, ,               sensorDigitalOut)
#pragma config(Sensor, I2C_1,  rightGrabber,   sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           frontLeft,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           bottomLeft,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           bottomRight,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           grabber,       tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port5,           backLeft,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           frontRight,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           topLeft,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           topRight,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          backRight,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../Shared/Competition.h"

int speedDriver = 1; // divider for slow speed on chassis
int speedGrabber = 1;
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

void closeGrabber(void){ motor[grabber] = 127/speedGrabber;}
void openGrabber(void){ motor[grabber] = -127/speedGrabber;}

void grabCone(void){
	//for grabbers to grab object
	//will not give back any new values
	int lastVal = nMotorEncoder[grabber];
	//last right grabber motor value is its encoder value
	bool stillMoving = true;
	// turn motor on
	closeGrabber();//motor[grabber] = 127/speedGrabber;
	//to bring grabber to cone to grab it
	while (stillMoving){
		// pause for ten milliseconds to let encoder gather information
		sleep(100);
		// current grabber motor value is the new encoder value
		int curVal = nMotorEncoder[grabber];
		//"still moving" occurs when:
		//current grabber motor value has changed since last check
		stillMoving = (curVal != lastVal);
		//checking for latest  motor value
		lastVal = curVal;
	}//function goal acheived, so exit while true loop
}//end grabCone
void releaseCone(void){
	//for grabbers to release object
	//will not give back any new values
	int lastVal = nMotorEncoder[grabber];
	//last grabber motor value is its encoder value
	bool stillMoving = true;
	// turn motor on
	openGrabber();//motor[grabber] = -127/speedGrabber;
	//to bring grabber to cone to grab it
	while (stillMoving){
		// pause for ten milliseconds to let encoder gather information
		sleep(100);
		// current grabber motor value is the new encoder value
		int curVal = nMotorEncoder[grabber];
		//"still moving" occurs when:
		//current grabber motor value has changed since last check
		stillMoving = (curVal != lastVal);
		//checking for latest motor value
		lastVal=curVal;
	}	//function goal acheived, so exit while true loop
}//end

// for autonomous
void riseLift(void){
	//left side up
	motor[bottomLeft] =	127/speedGrabber;
	motor[topLeft] =	127/speedGrabber;
	//right side up
	motor[bottomRight] =	127/speedGrabber;
	motor[topRight] =	127/speedGrabber;
	//time it takes for lift to get to the top
	wait10Msec(400); //35
	//left side stop
	motor[bottomLeft] =	0;
	motor[topLeft] =	0;
	//right side stop
	motor[bottomRight] =	0;
	motor[topRight] =	0;
}
void sendLiftDown(void){
	//left side down
	motor[bottomLeft] =	-127/speedGrabber;
	motor[topLeft] =	-127/speedGrabber;
	//right side down
	motor[bottomRight] =	-127/speedGrabber;
	motor[topRight] =	-127/speedGrabber;
}
void stopLift(void){
	//left side stop
	motor[bottomLeft] =	0;
	motor[topLeft] =	0;
	//right side stop
	motor[bottomRight] = 0;
	motor[topRight] =	0;
}

void fallLift(void){
	sendLiftDown();
	//time it takes for lift to fall to sationary goal
	wait10Msec(200); //10
	stopLift();
}
void startForwards(void) {
	//left side down
	motor[backLeft] =	127;
	motor[frontLeft] =	127;
	//right side down
	motor[backRight] =	127;
	motor[frontRight] =	127;
}
void stopDriving(void){
	//left side stop
	motor[backLeft] =	0;
	motor[frontLeft] =	0;
	//right side stop
	motor[backRight] =	0;
	motor[frontRight] =	0;
}

void driveForwards(int time) {
	startForwards();
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}
void driveBackwards (int time){
	//left side backward
	motor[backLeft] =	-127;
	motor[frontLeft] =	-127;
	//right side backward
	motor[backRight] =	-127;
	motor[frontRight] =	-127;
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}
void turnLeft (int time){
	//left side backward
	motor[backLeft] =	-127;
	motor[frontLeft] =	-127;
	//right side forward
	motor[backRight] =	127;
	motor[frontRight] =	127;
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}
void turnRight (int time){
	//left side forward
	motor[backLeft] =	127;
	motor[frontLeft] =	127;
	//right side backward
	motor[backRight] =	-127;
	motor[frontRight] =	-127;
	//time it takes for chassis to get to stationary goal
	wait10Msec(time);
	stopDriving();
}
//closes grabber to get cone, then lifts up lift
task liftCone (){
	while (true) {
		// if partner driver presses button 5 Up, closes grabber to get cone, then lifts up lift
		if (vexRT[Btn5UXmtr2]==1){
			grabCone();
			wait10Msec(50);
			riseLift ();
			wait10Msec(50);
		}//if
	}//while
};
bool active = false; // whether we want the real time control active
int target = 0; // what encoder value we want the claw to go to
int curPosition = 0; // where we are now
// task to do real time control of the claw, if desired
task niceClaw () {
	float kp = 0.4; //0.5 proportional gain.  Bigger = faster response, but possiblly unstable
	float ki = 0.01; // integral gain. cleans up steady state error at end
	float kd = 0.5; // derivative gain, to damp out the oscillations
	int errorSum=0;
	int errorChange=0;
	int oldError=0;
	while (true) {
		if (active) { // control only if told to do it
			clearLCDLine(0); displayLCDString(0,0,"controlling");
			curPosition = nMotorEncoder(grabber); // where are we now
			int errorPos= curPosition-target; // current position errors
			errorSum += errorPos; // build up a sum of error, for integral gain
			errorChange = errorPos-oldError; // estimate rate of change of error
			oldError=errorPos; // save for next time thru
			// compute new control value
			int m = (int)(-kp * errorPos - ki*errorSum - kd*errorChange);
			if (m > 127) m = 127; // limit to max motor value
				if (m < -127) m = -127; // limit to min motor value
				clearLCDLine(1); displayLCDNumber(1,8,m);
			displayLCDNumber(1,0,curPosition);
			motor [grabber] = m ;// send control to motor
			sleep(10);
		}//if
	}//whiles
}//end task niceClaw

bool claw45 = false; // button press makes this true; tells doClaw45 to start
task doClaw45 () {
	while (true) {
		if (claw45){ // if button was pressed
			clearLCDLine(0); displayLCDString(0,0,"doClaw45");
			target = -110; // encoder value for claws at 45 deg
			active = true; // tell niceClaw to control it
			curPosition = nMotorEncoder(grabber);
			int tick = 0;
			int error = curPosition-target;
			bool notThere = true;
			int countsThere = 0;
			bool timedOut=false;
			int sleepMS=10;
			while(notThere){
				// want to be in position for several counts
				if(abs(error)<10){
					countsThere++;
				}//if
				else {countsThere = 0;}
				if(countsThere>5){notThere = false;}//we've arrived for 50 milliseconds
				error = curPosition-target;
				tick ++;
				if (tick > (4*1000)/sleepMS){
					timedOut=true;
					break ; // timeout after 4 seconds
				}
				sleep(sleepMS);
			}//while

			clearLCDLine(0);
			if(timedOut) displayLCDString(0,0,"timeout");
			else displayLCDString(0,0,"arrived");
			active = false; // tell niceClaw to stop controlling
			claw45 = false;  // reset "button was pressed" indicator
		}//end if
	}//end while
}//end task doClaw45

//this function will not send back new numbers
bool isTipping=false;
void joystick(){
	displayLCDString(0,0,"start");

	startTask(chassisSlow); // monitor slow speed buttons
	startTask(liftCone); //grabs cone and lifts it up
	startTask(niceClaw); //sets the the grabber to predetermined position so the jaws don't hit the wheel
	startTask(doClaw45);//sets the grabber open to about 45 degrees using above task

	//continue forever
	while(true){
		//displayLCDNumber(1,0,speedDriver);
		//chassis motors
		isTipping= (SensorValue[dgtl2]==0);
		if (isTipping & ! vexRT[Btn6U]){
			// run back wheels backwards to prevent the robot from tipping
			motor[backLeft] = -127;
			motor[backRight] = -127;
		}
		else {
			motor[backLeft] = vexRT[Ch3]/speedDriver;
			motor[backRight] = vexRT[Ch2]/speedDriver;
		}
		motor[frontLeft] = vexRT[Ch3]/speedDriver;
		motor[frontRight] = vexRT[Ch2]/speedDriver;

		//lift motors
		//with slow speed function on 8U
		motor[bottomLeft] = vexRT[Ch2Xmtr2]/speedGrabber;
		motor[bottomRight] = vexRT[Ch2Xmtr2]/speedGrabber;
		motor[topLeft] = vexRT[Ch2Xmtr2]/speedGrabber;
		motor[topRight] = vexRT[Ch2Xmtr2]/speedGrabber;

		//grabber motor
		// up button=open, down button = close
		//button for grabber to close
		if (vexRT[Btn6UXmtr2] && !vexRT[Btn6DXmtr2]) {
			closeGrabber();
		};//end if
		//button for grabber to open
		if (vexRT[Btn6DXmtr2] && !vexRT[Btn6UXmtr2]) {
			openGrabber();
		};
		//if neither button is pressed, right grabber does nothing
		if (!claw45 && // not otherwise trying to control the claw
			!(vexRT[Btn6UXmtr2] || vexRT[Btn6DXmtr2])) {
			motor[grabber] = 0;
		};//end if
		if (vexRT[Btn8DXmtr2]){ // partner button 8 down = move claw to 45
			clearLCDLine(0); displayLCDString(0,0,"btn 8D");
			claw45 = true;
		};//end if
	}//exit while loop
}//end taskMain

void scoreLeftCone() {
	//drive backward or forward??
	driveForwards(50);
	//drive lift down
	fallLift();
	//turn left
	turnLeft(100);

	//move forward
	driveForwards(50);
	//grab cone
	grabCone();
	//lift up all the way
	riseLift();
	//turn right
	turnRight(70);
	//move forward
	driveForwards(50);
	//drop down to stationary goal
	fallLift();
	//release cone
	releaseCone();
	//drive backwards
	driveBackwards(100);
}
void scorePresetCone(){
	//scoring on stationary goals
	claw45 = true; // will make claw at nice angle
	wait10Msec(50); //wait for claw to move
	//driving lift down
	sendLiftDown();  wait10Msec(50);
	//grab cone
	grabCone();
	//lift up all the way
	riseLift();
	//drive forwards
	driveForwards(100);//10
	//drop down to stationary goal
	fallLift();
	//release cone
	releaseCone();
	//drive backwards
	driveBackwards(100);
}

void autonomous() {
	displayLCDString(0,0,"a-mouse");
	scorePresetCone();

	//scoreLeftCone();
}
