#define motor_neg        8                     // the pin connected to the wire A 
#define motor_pos        9                     // the pin connected to the wire A

#define P0 52
#define P1 50
#define P2 48
#define P3 46
#define P4 44
#define P5 42
#define P6 40
#define P7 38

#define SEL1 14
#define OE 13
#define HOME 2

#define pwm_out_pin_ENB 10

byte result_high;
byte result_low;
byte high_new;
byte high_old;
byte low_new;
byte low_old;
byte byte_data;
byte buffer_low;
byte buffer_high;

//PID parameters
double Kp = 160;
double Ki = 5;
double Kd = 0.5;

double output;
double actual_pwm; 
double count;

int target_count;
int incomingInt;
int lastIncomingInt;
int diff;

float last_error=0;
float error=0;
float dError=0;
float integral = 0;
float derivative = 0;
float iteration_time;


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
  pinMode(motor_neg, OUTPUT);
  pinMode(motor_pos, OUTPUT);


  target_count = -90;

}

void loop() {

  //Add serial input back in when done tuning before demo
  /*if(Serial.available() > 0){
    incomingInt= Serial.parseInt();
  }
  
  if (incomingInt != 0){
    target_count = incomingInt;
  }
*/
  
  pid();

  if(output < 0.0){
     digitalWrite(motor_neg, LOW);
     digitalWrite(motor_pos, HIGH);
  }
  else{   
     digitalWrite(motor_neg, HIGH);
     digitalWrite(motor_pos, LOW);
  }


  output = constrain(output, -255, 255);

  actual_pwm=abs(output); 

  analogWrite(pwm_out_pin_ENB, actual_pwm);

  // put your main code here, to run repeatedly:
  //Input to decoder chip to select upper byte 
  digitalWrite(OE, LOW);
  digitalWrite(SEL1, LOW);
  get_high();

  //Input to chip to select lower byte
  digitalWrite(SEL1, HIGH);
  //digitalWrite(SEL2, LOW);
  get_low();
 
  digitalWrite(OE, HIGH);

  //data_manipulations
  count = (result_high<<8) | result_low;
  count = double(count)/5.275;

  Serial.println(output); 
 

}

void pid(){
  
  unsigned long  currentMicros = micros();

  error = target_count - count;
  
  dError= error - last_error;

  integral += error * iteration_time*0.000001;

  derivative = dError / (iteration_time*0.000001);

  output = (Kp*error)+(Kd*derivative) + (Ki*integral);

  iteration_time = micros() - currentMicros;

  last_error = error ; 
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

  return byte_data;  
}
