#include <IRremote.h>
#include <IRremoteInt.h>
#include <IRMilesTagDefines.h>
#include <IRMilesTag.h>

#define FIRE_ENABLED
#define HIT_ENABLED

int PIN_HIT_LED   = 7;
int PIN_FIRE_LED  = 5;
int PIN_IR_INPUT  = 9;

// Not used - just here for documentation. 
// The IRremote library sets the output pin to
//  - Pin 3  for the Uno/Eleven
//  - Pin 13 for the Leonardo/LeoStick
// See IRremoteInt.h for TIMER_PWM_PIN.
// int PIN_IR_OUTPUT = ;

int HIT_STATUS_DURATION  = 100;
int FIRE_STATUS_DURATION = 100;
int TIME_BETWEEN_FIRE    = 2000;

int IR_FREQ = 36000;

unsigned long lastHit  = 0;
unsigned long lastFire = 0;

#ifdef FIRE_ENABLED
  IRMilesTagSend irSend;
#endif

#ifdef HIT_ENABLED
  IRMilesTagRecv irRecv(PIN_IR_INPUT);
  decode_results results;
#endif

void setup() {
  Serial.begin(9600);
  Serial.println(TIMER_PWM_PIN);
  
  #ifdef HIT_ENABLED
    irRecv.enableIRIn();
  #endif
  
  pinMode(PIN_HIT_LED,   OUTPUT);
  pinMode(PIN_FIRE_LED,  OUTPUT);
  
  digitalWrite(PIN_HIT_LED,  LOW);
  digitalWrite(PIN_FIRE_LED, LOW);
}

void loop() {
  updateStatusLeds();
  receiveIR();
  if (hasTriggerFired()) {
    fire();
  }
}

void fire() {
  #ifdef FIRE_ENABLED
    showStatusLed(PIN_FIRE_LED, &lastFire);

    irSend.sendShot(9, 2, 88);
    
    #ifdef HIT_ENABLED
      irRecv.enableIRIn();
    #endif
  #endif
}

void receiveIR() {
  #ifdef HIT_ENABLED
    if (irRecv.decode(&results)) {
      Serial.println(results.value, HEX);
      irRecv.resume();
      showStatusLed(PIN_HIT_LED, &lastHit);
    }
  #endif
}

void showStatusLed(int pin, unsigned long *timeShown) {
  digitalWrite(pin, HIGH);
  *timeShown = millis();
}

void updateStatusLeds() {
  #ifdef HIT_ENABLED
    updateStatusLed(PIN_HIT_LED,  &lastHit,  HIT_STATUS_DURATION);
  #endif
  
  #ifdef FIRE_ENABLED
    updateStatusLed(PIN_FIRE_LED, &lastFire, FIRE_STATUS_DURATION); 
  #endif
}

void updateStatusLed(int pin, unsigned long int *timeStarted, int duration) {
  if (*timeStarted > 0 && millis() - *timeStarted > duration) {
    digitalWrite(pin, LOW);
  }
}
 
// TODO: Hook this up to a button
int hasTriggerFired() {  
  return (millis() - lastFire > TIME_BETWEEN_FIRE) ? 1 : 0;
}

