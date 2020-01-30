#define A        8                     // the pin connected to the wire A 
#define A_bar    9                     // the pin connected to the wire A
#define B        10                    // the pin connected to the wire B
#define B_bar    11                     // the pin connected to the wire B
#define x        5000                  // smaller values may make the motor produce more speed and less torque

#define mult 1
#define P0 52
#define P1 50
#define P2 48
#define P3 46
#define P4 44
#define P5 42
#define P6 40
#define P7 38

//int SEL2 = 12;
#define SEL1 14
#define OE 13
#define HOME 2

byte result_high;
byte result_low;
byte high_new;
byte high_old;
byte low_new;
byte low_old;
byte byte_data;
byte buffer_low;
byte buffer_high;

double count;

///desired serial entered value for position
int incomingInt = 0; 
int toMove;
int toMoveSteps;
int cwStepCount = 0;
int ccwStepCount = 0;
int moveFlag = 0;

//****MOTOR CONTROLLER TO MOTOR PINS***////

//white (motor pin 1)  - out3 (green wire) - phase A
//blue  (motor pin 5) -  out4 (blue wire) - phase A -
//purple (motor pin 4) - out1 (blue wire) - phase B
//orange (motor pin 8) - out2 (green wire) -  phase B-


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //encoder pins
  pinMode(52, INPUT);
  pinMode(50, INPUT); 
  pinMode(48, INPUT);
  pinMode(46, INPUT); 
  pinMode(44, INPUT);
  pinMode(42, INPUT); 
  pinMode(40, INPUT);
  pinMode(38, INPUT);
  pinMode(SEL1, OUTPUT);
  //pinMode(SEL2, OUTPUT); 
  pinMode(OE, OUTPUT);

  //motor pins
    //motor
  pinMode(A, OUTPUT);
  pinMode(A_bar, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(B_bar, OUTPUT);
  
}

void loop() {


  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingInt = Serial.parseInt(); 
  }

  if (incomingInt != 0){
    toMove = incomingInt;
    moveFlag = 1;
  }

  //Serial.println(toMove);
  if (moveFlag == 1){

       if (toMove >0){
       toMoveSteps = toMove/7;
       go_cw(toMoveSteps);
       }
       else{
       toMoveSteps = toMove/7;
       go_ccw(toMoveSteps);
       }
  }
  
  // put your main code here, to run repeatedly:
  //Input to decoder chip to select upper byte 
  digitalWrite(OE, LOW);
  digitalWrite(SEL1, LOW);
  //digitalWrite(SEL2, LOW);
  get_high();

  //Input to chip to select lower byte
  digitalWrite(SEL1, HIGH);
  //digitalWrite(SEL2, LOW);
  get_low();
 
  digitalWrite(OE, HIGH);

  //data_manipulations
  count = (result_high<<8) | result_low;
  count = double(count);
  //Serial.println((PINB & B00000010), BIN);
  Serial.println(count);

}



//function for getting 2nd LSB
void get_high (){
  high_new = get_data();
  high_old = get_data();

  while (!(high_new == high_old)){
    high_new = get_data();
    high_old = get_data();
    }
  result_high = high_new ;
  return;
} 


//function for getting LSB
void get_low () {
  low_new = get_data();
  low_old = get_data();
  
  while (!(low_new == low_old)){
    low_new = get_data();
    low_old = get_data();
    }
  result_low = low_new ;
  return;
}


byte get_data() {
  byte_data = (
  ((PINB & B00000010)  >> 1)|                 //52, PB1, 0
  ((PINB & B00001000)  >> 2)|                 //50, PB3, 1
  ((PINL & B00000010)  << 1)|                 //48, PL1, 2
  ((PINL & B00001000)      )|                 //46, PL3, 3 
  ((PINL & B00100000)  >> 1)|                 //44, PL5, 4
  ((PINL & B10000000)  >> 2)|                 //42, PL7, 5
  ((PING & B00000010)  << 5)|                 //40, PG1, 6
  ((PIND & B10000000)      )                  //38, PD7, 7
  );
//  bitWrite(byte_data, 0, digitalRead(52));
//  bitWrite(byte_data, 1, digitalRead(50));
//  bitWrite(byte_data, 2, digitalRead(48));
//  bitWrite(byte_data, 3, digitalRead(46));
//  bitWrite(byte_data, 4, digitalRead(44));
//  bitWrite(byte_data, 5, digitalRead(42));
//  bitWrite(byte_data, 6, digitalRead(40));
//  bitWrite(byte_data, 7, digitalRead(38));   
  return byte_data;  
}



//motor functions

void go_cw(int numSteps){
   
     if(cwStepCount != numSteps){
      
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
         cwStepCount = 0;
         moveFlag = 0;
     }
}


void go_ccw(int numSteps){
   
     if(ccwStepCount != numSteps){

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
         moveFlag = 0;
     }
}
