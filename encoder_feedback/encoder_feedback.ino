
#define CW 0
#define CCW 1

#include <Stepper.h>
#include<Wire.h>

// Define number of steps per revolution:
const int stepsPerRevolution = 200;
// Initialize the stepper library on pins 8 through 11:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);

const int a = 2;
const int b = 3;

volatile double angle = 0.0;
short dir = CW; // 0-clockwise, 1-counterclockwise
volatile int cur_pos = 0;
volatile int prev_pos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(a, INPUT);
  pinMode(b, INPUT); 
  //detect_a = digitalRead(a);
  //detect_b = digitalRead(b);
  attachInterrupt(digitalPinToInterrupt(a), ISR_A, RISING);
  //attachInterrupt(digitalPinToInterrupt(b), ISR_B, RISING);  

// Set the motor speed (RPMs):
  myStepper.setSpeed(100);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(cur_pos != prev_pos)
  {
    Serial.print(cur_pos);
    Serial.print("\t");
    Serial.print(dir == CW ? "clockwise" : "counter-clockwise");
    Serial.print("\t");
    Serial.println(angle);
    prev_pos = cur_pos;
  }
  myStepper.step(200);
  delay(500);

 if(cur_pos != prev_pos)
  {
    Serial.print(cur_pos);
    Serial.print("\t");
    Serial.print(dir == CW ? "clockwise" : "counter-clockwise");
    Serial.print("\t");
    Serial.println(angle);
    prev_pos = cur_pos;
  }
  //Step on revolution in the other direction:
  myStepper.step(-200);
  delay(500);

}

void ISR_A () {
  int detect_a = digitalRead(a);
  int detect_b = digitalRead(b);

  if (detect_a==detect_b) 
  {
    //b is leading a (CCW)
    cur_pos--;
    dir = CCW;
    //Serial.println("b");
  }
  else
  {
    //a is leading b (CW)
    cur_pos++;
    dir = CW;
    //Serial.println("a");
  }
  angle = (cur_pos/100.0) * 360.0;
  
}
