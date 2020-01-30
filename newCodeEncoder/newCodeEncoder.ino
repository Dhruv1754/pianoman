//works most of the time. (would sometimes start at a random huge number)
//link to library: https://github.com/NicksonYap/digitalWriteFast
#include <digitalWriteFast.h> 

const int a = 2;
const int b = 3;

//volatile double angle = 0.0;
volatile bool a_cur_pos;
volatile bool b_cur_pos;
volatile bool a_prev_pos;
volatile bool b_prev_pos;
volatile long count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(a, INPUT);
  pinMode(b, INPUT); 
  //attachInterrupt(digitalPinToInterrupt(a), ISR_A, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(b), ISR_B, CHANGE);  
  attachInterrupt(0, ISR_A, RISING);
  attachInterrupt(1, ISR_B, RISING); 
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.print("Encoder Ticks: ");
    Serial.print(count);
    Serial.print("\t");
    Serial.print("Revolution: ");
    Serial.println(count/400.0);
}

void ISR_A () {
  a_cur_pos = digitalReadFast(a);
  b_cur_pos = digitalReadFast(b);

  count += ParseEncode();

  a_prev_pos = a_cur_pos;
  b_prev_pos = b_cur_pos;
}

void ISR_B () {
  a_cur_pos = digitalReadFast(a);
  b_cur_pos = digitalReadFast(b);

  count += ParseEncode();
  
  a_prev_pos = a_cur_pos;
  b_prev_pos = b_cur_pos;
}

int ParseEncode () {
  if(a_prev_pos && b_prev_pos){
    if (!a_cur_pos && b_cur_pos) return 1;
    if (a_cur_pos && !b_cur_pos) return -1;
  }
  else if(!a_prev_pos && b_prev_pos){
    if (!a_cur_pos && !b_cur_pos) return 1;
    if (a_cur_pos && b_cur_pos) return -1;
  }
  else if(!a_prev_pos && !b_prev_pos){
    if (a_cur_pos && !b_cur_pos) return 1;
    if (!a_cur_pos && b_cur_pos) return -1;
  }
  else if(a_prev_pos && !b_prev_pos){
    if (a_cur_pos && b_cur_pos) return 1;
    if (!a_cur_pos && !b_cur_pos) return -1;
  }
}
