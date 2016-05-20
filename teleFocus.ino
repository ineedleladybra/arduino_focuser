/*
   Daniel C Bradley
   20/05/2016
   DIY Stepper Focuser using 28BYJ48
   Using momentry buttons & potentiometer
   Version 1.0.0
*/

#include <AccelStepper.h>

//LED pins
int activityLEDPin = 13;

//POTENTIOMETER
const int analogInPin = A0;   // Analog input pin that the potentiometer is attached to
int sensorValue = 0;          // value read from the pot
long mappedSensorValue = 0.0; // Mapped value from the pot

//BUTTONS
const int greenButtonPin = 3;
const int redButtonPin = 2;
boolean active = false;

//MOTOR PINS
#define motorPin1  8     // Blue   - 28BYJ48 pin 1
#define motorPin2  9     // Pink   - 28BYJ48 pin 2
#define motorPin3  10    // Yellow - 28BYJ48 pin 3
#define motorPin4  11    // Orange - 28BYJ48 pin 4

#define STEPS 8

// ROTATIONS
const long STARTING_POSITION = 0.0;
const long SINGLE_STEP = 1.0;

//RPM speeds
const int MAX_SPEED = 700;
const int VERY_FAST = 699;
const int FAST      = 599;
const int MEDIUM    = 499;
const int SLOW      = 399;
const int VERY_SLOW = 299;
const int FINE      = 199;
const int VERY_FINE = 99;
const int NUDGE     = 9; //0.00027777;

//NOTE: Sequence 1-3-2-4 is required
AccelStepper stepperA( STEPS, motorPin1, motorPin3, motorPin2, motorPin4 );

//Functions
void moveStepperTo(long value);
void setStepperSpeed(long value);

//
// SETUP
//
void setup() {

  //Configure LED
  pinMode( activityLEDPin, OUTPUT );
  digitalWrite( activityLEDPin, LOW );

  //Configue buttons
  pinMode( greenButtonPin, INPUT );
  pinMode( redButtonPin, INPUT );
  digitalWrite( greenButtonPin, HIGH );
  digitalWrite( redButtonPin, HIGH );

  //Configure Stepper
  stepperA.setCurrentPosition ( STARTING_POSITION );
  stepperA.setMaxSpeed( MAX_SPEED );
  stepperA.setSpeed( MAX_SPEED );
  stepperA.setAcceleration( MAX_SPEED );

  //Configure Serial
  Serial.begin(9600);

}


void loop() {

  //Update POTENTIOMETER
  sensorValue = analogRead( analogInPin );
  mappedSensorValue = map( sensorValue, 0, 1023, NUDGE, MAX_SPEED );

  //Check for any button presses
  checkForButtonPresses();

  //Keep checking stepper for movement requests
  if ( active ) {
    stepperA.run();
  }

}

void checkForButtonPresses() {

  int greenInput = digitalRead( greenButtonPin );
  int redInput = digitalRead( redButtonPin );

  //Check Button is off
  if ( greenInput == HIGH || redInput == HIGH ) {
    active = false;
  }

  //Check button is on
  if ( greenInput == LOW ) {
    moveStepperTo( SINGLE_STEP );
    setStepperSpeed( mappedSensorValue );
    active = true;
  }

  if ( redInput == LOW ) {
    moveStepperTo( -SINGLE_STEP );
    setStepperSpeed( -mappedSensorValue );
    active = true;
  }

}

//
// MOVEMENT CW & CCW
//
void moveStepperTo(long value) {
  stepperA.move( value );
}

//
// SPEED & ACCELERATION
//
void setStepperSpeed(long value) {

  if ( abs(value) < NUDGE ) {
    value = NUDGE;
  } else if ( abs(value) > MAX_SPEED ) {
    value = MAX_SPEED;
  }

  stepperA.setAcceleration( value / 2 );
  stepperA.setSpeed( value );

}
