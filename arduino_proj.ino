#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <Stepper.h>
// Define Constants
 
// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 
 
//  Amount of Gear Reduction
const float GEAR_RED = 64;
 
// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;
 
// Define Variables
 
// Number of Steps Required
int StepsRequired;
int speedupBut = 2;
int slowdownBut = 3;
int reverse = 4;
int speedStepper = 1000;

const int TrigPin = 6;
const int EchoPin = 5;

const int IRreceiver = 7;

float distance;

// Create Instance of Stepper Class
// Specify Pins used for motor coils
// The pins used are 8,9,10,11 
// Connected to ULN2003 Motor Driver In1, In2, In3, In4 
// Pins entered in sequence 1-3-2-4 for proper step sequencing
 
Stepper steppermotor(STEPS_PER_REV, 8, 10, 9, 11);
IRrecv irrecv(IRreceiver);

decode_results results;

void setup()
{
// Nothing  (Stepper Library sets pins as outputs)
  pinMode(slowdownBut,INPUT);
  pinMode(speedupBut,INPUT);
  pinMode(reverse,INPUT);
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
  irrecv.enableIRIn();
  Serial.begin(9600);
}
 
void loop()
{
  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin,LOW);
  distance = pulseIn(EchoPin,HIGH)/ 58.00;
  Serial.println(distance);
  
  if(digitalRead(slowdownBut) == HIGH){
    if(speedStepper <= 125){
      speedStepper = 125;
    }else{
      speedStepper = speedStepper - 200;
    }
  }
  if(digitalRead(speedupBut) == HIGH){
    if(speedStepper >= 1125){
      speedStepper = 1125;
    }else{
      speedStepper = speedStepper + 200;
    }  
  }

  // ir remote
  if (irrecv.decode(&results))
{
  switch(results.value){
    case 0xFF30CF:
       if(speedStepper <= 125){
         speedStepper = 125;
      }else{
      speedStepper = speedStepper - 200;
    }
    Serial.println(speedStepper);
    break;
    case 0xFF18E7:
     if(speedStepper >= 1125){
      speedStepper = 1125;
    }else{
      speedStepper = speedStepper + 200;
    }  
        Serial.println(speedStepper);

        break;
    }
   irrecv.resume();

  }
  
  steppermotor.setSpeed(speedStepper); 

  StepsRequired  =  STEPS_PER_OUT_REV / 2; 
  
    if(digitalRead(reverse) == HIGH){
      steppermotor.step(-StepsRequired);
    }else{
      steppermotor.step(StepsRequired);
    }
  // Slow - 4-step CW sequence to observe lights on driver board
   // Rotate CW 1/2 turn slowly
  //steppermotor.step(StepsRequired);
  delay(1);
  /*
  // Rotate CCW 1/2 turn quickly
  StepsRequired  =  - STEPS_PER_OUT_REV / 2;   
  steppermotor.setSpeed(700);  
  steppermotor.step(StepsRequired);
  delay(2000);
 */ 
}
