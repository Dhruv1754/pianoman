
#define CW 0
#define CCW 1

#define A        8                     // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire) 
#define A_bar    9                     // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire)
#define B        10                    // the pin connected to the wire B of the coil A (or to the H-bridge pin controlling the same wire)
#define B_bar    11                     // the pin connected to the wire B- of the coil A (or to the H-bridge pin controlling the same wire)
#define x        5000                  // smaller values may make the motor produce more speed and less torque
#define stepsPerRevolution 200         // you can the number of steps required to make a complete revolution in the data sheet of your motor


const int a = 2;
const int b = 3;

volatile double angle = 0.0;
short dir = CW; // 0-clockwise, 1-counterclockwise
volatile int cur_pos = 0;
volatile int prev_pos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //sensor
  pinMode(a, INPUT);
  pinMode(b, INPUT); 
  //detect_a = digitalRead(a);
  //detect_b = digitalRead(b);
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
    Serial.print(cur_pos);
    Serial.print("\t");
    Serial.print(dir == CW ? "clockwise" : "counter-clockwise");
    Serial.print("\t");
    Serial.println(angle);
    prev_pos = cur_pos;
  }

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
