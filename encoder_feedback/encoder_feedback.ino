#define CW 0
#define CCW 1

#define A        8                     // the pin connected to the wire A 
#define A_bar    9                     // the pin connected to the wire A
#define B        10                    // the pin connected to the wire B
#define B_bar    11                     // the pin connected to the wire B
#define x        5000                  // smaller values may make the motor produce more speed and less torque
#define stepsPerRevolution 200         // you can the number of steps required to make a complete revolution in the data sheet of your motor


//encoder pins
const int a = 2;
const int b = 3;

//variables for encoder reading
volatile double angle = 0.0;
short dir = CW; // 0-clockwise, 1-counterclockwise
volatile int cur_pos = 0;
volatile int prev_pos = 0;


//steps for right and left motor control desired position
int cwStepCount = 0;
int ccwStepCount = 0;

// direction flag
int dirFlag = 0;
//int move flag
int moveFlag = 0;
//delayCount
int delayCount = 0;

//****MOTOR CONTROLLER TO MOTOR PINS***////

//white (motor pin 1)  - out3 (green wire) - phase A
//blue  (motor pin 5) -  out4 (blue wire) - phase A -
//purple (motor pin 4) - out1 (blue wire) - phase B
//orange (motor pin 8) - out2 (green wire) -  phase B-

////***POWER SUPPLY SETTINGS***/////

//set current to 0.52 Amps


int incomingInt = 0; 
int toMove;
int toMoveSteps;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //sensor
  pinMode(a, INPUT);
  pinMode(b, INPUT); 
  attachInterrupt(digitalPinToInterrupt(a), ISR_A, RISING); 
  
  //motor
  pinMode(A, OUTPUT);
  pinMode(A_bar, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(B_bar, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:



 if (cur_pos != prev_pos){
    prev_pos = cur_pos;
     Serial.print(dir == CW ? "clockwise" : "counter-clockwise");
    Serial.println(angle);
  }


  /*if (Serial.available() > 0) {
    // read the incoming byte:
    incomingInt = Serial.parseInt(); 
    moveFlag = 1;
  }*/

 
  /*if (incomingInt != 0){
    toMove = incomingInt;
  }
  if (moveFlag == 1){

       toMoveSteps = toMove/6;
       go_cw(toMoveSteps);
  }*/

//Serial.println(toMove);

 /*else if (dirFlag == 2){
    go_ccw(-12);
  }
  else{
    delayMove(200);
  }*/

/*
  Serial.print(cwStepCount);
  Serial.print("   ");
  Serial.print(ccwStepCount);
  Serial.print("   ");
  Serial.print(delayCount);
  Serial.print("   ");
  Serial.print("dirFlag: ");
  Serial.print(dirFlag);

  Serial.println("");
  */

}

//target_pos right now is in steps
void go_cw(int target_pos){
   
      if (cwStepCount != target_pos){
  
          digitalWrite(A, HIGH);
          digitalWrite(A_bar, LOW);
          digitalWrite(B, LOW); 
          digitalWrite(B_bar, HIGH);
          delayMicroseconds (x);
        
          digitalWrite(A, LOW);
          digitalWrite(A_bar, HIGH);
          digitalWrite(B, LOW);
          digitalWrite(B_bar, HIGH);
          delayMicroseconds (x);
        
          digitalWrite(A, LOW);
          digitalWrite(A_bar, HIGH);
          digitalWrite(B, HIGH);
          digitalWrite(B_bar, LOW);
          delayMicroseconds (x);
        
          digitalWrite(A, HIGH);
          digitalWrite(A_bar, LOW);
          digitalWrite(B, HIGH);
          digitalWrite(B_bar, LOW);
          delayMicroseconds (x);

          cwStepCount++;
      }

      else{
        moveFlag = 0;
        cwStepCount = 0;
        dirFlag = 1;
      }
}


//target_pos right now is in steps
void go_ccw(int target_pos){

      if (ccwStepCount != target_pos){
         //donothing
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, LOW); 
        digitalWrite(B_bar, HIGH);
        delayMicroseconds (x);
          
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delayMicroseconds (x);
  
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delayMicroseconds (x);
  
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delayMicroseconds (x);
  
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, LOW); 
        digitalWrite(B_bar, HIGH);
        delayMicroseconds (x);

        ccwStepCount--;
      }

      else{
        ccwStepCount = 0;
        dirFlag = 3;
      }
}


void delayMove(int desiredDelay){
  
   if (delayCount != desiredDelay){
       delay(1);           
       /*Serial.print("\t");
       Serial.print(dir == CW ? "clockwise" : "counter-clockwise");
       Serial.print("\t");
       Serial.println(angle);*/
       delayCount++;
   }

   else{
       dirFlag = 0;
   }
   /*else if (dirFlag == 1){
       delayCount = 0;
       dirFlag = 2;
   }
   else if (dirFlag == 3){
       delayCount = 0;
       dirFlag = 0;
   }*/
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
