/*  Program finds the number of left and right motor encoder counts
 *   using microcontroller interrupts.
*/   
//  encoder variables
extern void LogoPrint();
const int COUNTS_PER_CM = 71;
const int DIST_TO_TRAVEL_OUTER = COUNTS_PER_CM * 300;
const int DIST_TO_TRAVEL_INNER = COUNTS_PER_CM * 188.37;

volatile long LtCount = 0; 
volatile long RtCount = 0;

int leftDir_Pin = 8;
int leftPWM_Pin = 9; 
int rightDir_Pin = 10; 
int rightPWM_Pin = 11; 

int leftDutyCyclePercent;
int rightDutyCyclePercent; 
int leftDutyCycle; 
int rightDutyCycle; 

int LtCountDel;
int RtCountDel;
int LtPrev;
int RtPrev;

int state = 0;
int next = 0;
int flag = 1;
unsigned long T0;
unsigned long T;

void LtEncoderRead() {
  LtCount += 1;
}

void RtEncoderRead() {
  RtCount += 1;
}

void setMotorSpeeds(int dutyCyclePercent)
{
  leftDutyCycle = dutyCyclePercent*255/100;
  rightDutyCycle = dutyCyclePercent*255/100;
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  attachInterrupt(1, LtEncoderRead, RISING);     //pin 3
  attachInterrupt(0, RtEncoderRead, RISING);     //pin 2

//setup motor control pins  
  pinMode(leftDir_Pin, OUTPUT);
  pinMode(leftPWM_Pin, OUTPUT);
  pinMode(rightDir_Pin, OUTPUT);
  pinMode(rightPWM_Pin, OUTPUT);

//initialize motor speeds to equal values
  leftDutyCyclePercent = 20;
  rightDutyCyclePercent = 20;
  leftDutyCycle = leftDutyCyclePercent*255/100;
  rightDutyCycle = rightDutyCyclePercent*255/100;

//set motor direction forward
  digitalWrite(leftDir_Pin, HIGH); 
  digitalWrite(rightDir_Pin, HIGH);
 
// turn on motors
  analogWrite(leftPWM_Pin, leftDutyCycle);
  analogWrite(rightPWM_Pin, rightDutyCycle); 

  LtCount = 0;
  RtCount = 0;   
}
  
void loop() 
{
/*  while loop attempts to compensate for different motor speed differences  
    encoder counts furnished by interrupts
    attempt to go straight for 12720 counts keeping left motor speed constant
    and adjusting the right motor speed
*/
//  Reinitialize variables each time thru loop
  LtPrev = LtCount;
  RtPrev = RtCount;
  T0 = millis();   

  digitalWrite(leftDir_Pin, HIGH); 
  digitalWrite(rightDir_Pin, HIGH);
  
  while (1) 
  {
    T = millis();
    // Count encoder pulses every 100 ms and adjust motor speeds   
    /*if(T >= T0 + 100) {
      LtCountDel = LtCount - LtPrev;
      RtCountDel = RtCount - RtPrev;
      LtPrev = LtCount;   
      RtPrev = RtCount;
      T0 = millis();      
      
      if(LtCountDel > RtCountDel)
        rightDutyCycle = rightDutyCycle + 1;
      else if (RtCountDel > LtCountDel)
        rightDutyCycle = rightDutyCycle - 1;      

      
      }*/  

      analogWrite(rightPWM_Pin, rightDutyCycle); 
      analogWrite(leftPWM_Pin, leftDutyCycle*1.5926); 
      Serial.println(state);
      switch(state)
      {
        case 0:
        {
          if(LtCount >= DIST_TO_TRAVEL_OUTER && RtCount >= DIST_TO_TRAVEL_INNER)
          {
            state = 1;
            //delay(50);
          }
          
          break;
        }
        case 1:
        {
          if(flag){
            LtCount = 0;
            RtCount = 0;
            flag = 0;
          }
          analogWrite(rightPWM_Pin, rightDutyCycle*1.5926); 
          analogWrite(leftPWM_Pin, leftDutyCycle); 
          if(LtCount >= DIST_TO_TRAVEL_INNER && RtCount >= DIST_TO_TRAVEL_OUTER)
          {
            Serial.println("boh shit whatup");
            state = 2;
          }
          break;
        }
        /*case 2:
        {
          delay(1500);
          while(1){
            setMotorSpeeds(0); 
          }
        }*/
      }
      if(state == 2)
          {
            delay(4200);
            while(1){
              setMotorSpeeds(0); 
              analogWrite(rightPWM_Pin, rightDutyCycle);
              analogWrite(leftPWM_Pin, leftDutyCycle);
            }
          }
      Print();
      LogoPrint();       
    }
}




