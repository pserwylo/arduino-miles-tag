#include <IRremote.h>
#include <IRremoteInt.h>
#include <IRMilesTag.h>
#include <IRMilesTagDefines.h>

#define FIRE_ENABLED
#define HIT_ENABLED

int PIN_HIT_LED   = 7;
int PIN_FIRE_LED  = 5;
int PIN_IR_INPUT  = 9;

int myPlayer = 11;
int myTeam   = 3;
int myDamage = MT_DAMAGE_75;

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
  
  Serial.print("Timer PWM pin");
  Serial.println(TIMER_PWM_PIN, DEC);
  
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
  
    Serial.print("Firing shot: Player ");
    Serial.print(myPlayer, DEC);
    Serial.print(", Team ");
    Serial.print(myTeam, DEC);
    Serial.print(", Damage ");
    Serial.println(myDamage, DEC);
  
    showStatusLed(PIN_FIRE_LED, &lastFire);
    irSend.sendShot(myPlayer, myTeam, myDamage);
    
    #ifdef HIT_ENABLED
      irRecv.enableIRIn();
    #endif
  #endif
}

void receiveIR() {
  #ifdef HIT_ENABLED
    if (irRecv.recv(&results)) {
      
      Serial.print("Hit by: Player ");
      Serial.print(MT_PLAYER_VALUE(results.value), DEC);
      Serial.print(", Team ");
      Serial.print(MT_TEAM_VALUE(results.value), DEC);
      Serial.print(", Damage ");
      Serial.println(MT_DAMAGE_VALUE(results.value), DEC);
  
      
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

