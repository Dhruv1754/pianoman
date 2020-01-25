
//int pinA = A0;   
//int pinB = A1;   
//int valA = 0;   
//int valB = 0;// variable to store the value coming from the sensor
//int prevA =0;
//int counterA=0;
//int prevB =0;
//int counterB=0;
//void setup() {
//    // declare the ledPin as an OUTPUT
//
//  Serial.begin(9600);
//}
//
//void loop() {
//  prevA=valA;
//  if (analogRead(pinA)>800){
//    valA = 1;  
//    }
//   else{
//    valA=0;
//   }
//  prevB=valB;
//  if (analogRead(pinB)>800){
//    valB = 1;  
//    }
//   else{
//    valB=0;
//   }
//  Serial.println(valA);// stop the program for some time
//  Serial.println(valB);
//  Serial.println("-----------edges of clockA ");
//  Serial.println(counterA);
//  Serial.println("-----------edges of clockA ");
//  if (valA != prevA){
//    counterA++;
//  }
//  Serial.println("-----------edges of clockB ");
//  Serial.println(counterB);
//  Serial.println("-----------edges of clockB ");
//  if (valB != prevB){
//    counterB++;
//  }
//  
//  
//}

#define CW 0
#define CCW 1

const int a = 2;
const int b = 3;
//volatile int detect_b = 0;
//volatile int detect_a = 0;
//volatile int counter = 0;
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
  /*detect_b = digitalRead(b);
  if (detect_b != detect_b_last) {
    if (detect_b == HIGH) {
      counter++;
      angle = (counter/100.0) * 360.0;
      Serial.print(counter);
      Serial.print(" ");
      Serial.println(angle);
    } 
    delay(50);
  }
  detect_b_last = detect_b;*/
  //detect_a = digitalRead(a);
  //Serial.println(detect_a);
  /*Serial.print(" ");
  Serial.print(detect_b);
  Serial.print(" ");
  Serial.println(angle);
  delay(1000);*/
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
