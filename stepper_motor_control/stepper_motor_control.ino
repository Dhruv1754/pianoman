/* Example sketch to control a stepper motor with L298N motor driver, Arduino UNO and Stepper.h library. More info: https://www.makerguides.com */
// Include the Stepper library:
#include <Stepper.h>
#include<Wire.h>
// Define number of steps per revolution:
const int stepsPerRevolution = 200;
// Initialize the stepper library on pins 8 through 11:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);

//Here is the code. It uses I2C. I took some parts of code from internet.(I2C part)

int val = 0;
int potPin = A2;

void setup() {
  // Set the motor speed (RPMs):
  myStepper.setSpeed(100);
  Serial.begin(9600); 
}
void loop() {
  // Step one revolution in one direction:
  myStepper.step(200);
  delay(500);
  
  val = analogRead(potPin);    // read the value from the sensor 
  Serial.println(val);// stop the program for some time
  delay(500);
  
  //Step on revolution in the other direction:
  myStepper.step(-200);
  delay(500);
  
  val = analogRead(potPin);    // read the value from the senso
  Serial.println(val);// stop the program for some time
  delay(500);

}
