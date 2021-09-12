
// Include the library
#include <L298N.h>

// Pin definition Line Green
const unsigned int IN1A_1 = 8;
const unsigned int IN2A_1 = 9;
// Create one motor instance
L298N motorA_1(IN1A_1, IN2A_1);

// Pin definition Line Blue
const unsigned int IN1A_2 = 10;
const unsigned int IN2A_2 = 11;
// Create one motor instance
L298N motorA_2(IN1A_2, IN2A_2);

// Pin definition Line Yellow
const unsigned int IN1B_1 = 4;
const unsigned int IN2B_1 = 5;
// Create one motor instance
L298N motorB(IN1B_1, IN2B_1);

// Pin definition ps2 joy Right
int RX = A1;
int RY = A0;
int sw_R = 2;

// Pin definition ps2 joy Left
int LX = A2;

void setup()
{
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial)
  {
    //do nothing
  }

  // Set initial speed
  motorA_1.setSpeed(100);
  motorA_2.setSpeed(100);
  motorB.setSpeed(100);

}

void loop()
{
  int joy_RX = analogRead(RX);
  int joy_RY = analogRead(RY);
  int joy_LX = analogRead(LX);
  
  // Change speed
  motorA_1.setSpeed(255);
  motorA_2.setSpeed(255);
  motorB.setSpeed(255);

  // Tell the motor to go forward (may depend by your wiring)
  if (joy_RX >= 1000) {
    goForward();
  }
  else if (joy_RX <= 300) {
    goBackward();
  }
  else if (joy_RY <= 300) {
    goLeft();
  }
  else if (joy_RY >= 700) {
    goRight();
  }
  else {
    getStop();
  }

  if (joy_LX >= 700) {
    goUp();
  }
  else if (joy_LX < 50) {
    goDown();
  }
  else {
    motorB.stop();
  }

  printSomeInfo();
  Serial.println("---------------------");
  printJoyInfo();
  Serial.println("---------------------");
}

void goForward() {
  motorA_1.backward();
  motorA_2.forward();
}

void goLeft() {
  motorA_1.forward();
  motorA_2.forward();
}

void goRight() {
  motorA_1.backward();
  motorA_2.backward();
}

void goBackward() {
  motorA_1.forward();
  motorA_2.backward();
}

void getStop() {
  motorA_1.stop();
  motorA_2.stop();
}

void goUp() {
  motorB.backward();
}

void goDown() {
  motorB.forward();
}


/*
Print some informations in Serial Monitor
*/
void printSomeInfo()
{
  Serial.print("Motor is moving = [");
  Serial.print(motorA_1.isMoving());
  Serial.print(", ");
  Serial.print(motorA_2.isMoving());
  Serial.print(", ");
  Serial.print(motorB.isMoving());
  Serial.print("], at speed = [");
  Serial.print(motorA_1.getSpeed());
  Serial.print(", ");
  Serial.print(motorA_2.getSpeed());
  Serial.print(", ");
  Serial.println(motorB.getSpeed());
  Serial.print("]");
}

void printJoyInfo()
{
  int joy_RX = analogRead(RX);
  int joy_RY = analogRead(RY);
  int joy_LX = analogRead(LX);
  Serial.print("R : [");
  Serial.print(joy_RX);
  Serial.print(",");
  Serial.print(joy_RY);
  Serial.print("], L : [");
  Serial.print(joy_LX);
  Serial.println("]");
}
