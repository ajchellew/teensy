#include <phukdlib.h>

/* Simple USB Keyboard Example
   Teensy becomes a USB keyboard and types characters
   You must select Keyboard from the "Tools > USB Type" menu
*/

const int ledPin = 13;
const int payloadDelay = 60000;

bool prepared = false;
bool activation1 = false;
bool activation2 = false;

unsigned long lastActivationButtonTime = 0;
bool capsLockTrapSet = false;

bool numLockOn;

void setup() {
  pinMode(ledPin, OUTPUT);
  Keyboard.begin();    
  Serial.begin(9600); 
  delay(1000);  
  ledOn();   
  Serial.println("Start");   

  numLockOn = IsNumbOn();
  //scrollLockOn = IsScrlOn();
}

void loop() {

  delay(100);
  
  if (!activation1) {
    if (IsNumbOn() == numLockOn)      
      return;    
    lastActivationButtonTime = millis();
    Serial.println("activation 1");
    activation1 = true;
    return;
  }

  if (!activation2) {
    if (IsNumbOn() != numLockOn)
      return;    
    activation2 = true;
    Serial.println("Activated");
    ledOff();    
  }

  if (!capsLockTrapSet && !IsCapsOn()){
    Keyboard.press(KEY_CAPS_LOCK);
    Keyboard.releaseAll();
    delay(500);
    capsLockTrapSet = true;
  }

  if (capsLockTrapSet && !IsCapsOn()) {
    delay(payloadDelay);

    runPayload();

    ledOn();
    activation1 = false;
    activation2 = false;
    capsLockTrapSet = false;
  }
}

void runPayload() {
  //browser();
  wander();
  //lightShow();
}

void browser() {
  CommandAtRunBarMSWIN("cmd /c start http://irongeek.com");
}

int jiggleIntervalSecs = 10;

void jiggle() {
  bool scrollLockOn = IsScrlOn();
  
  while (IsScrlOn() == scrollLockOn) {  
    int i;
    for (i=0; i<40; i++) {
      Mouse.move(0, -6);  
      delay(10);
      Mouse.move(0, 6);
      delay(10);
    }

    int j;
    for (j=0; j<jiggleIntervalSecs; j++) {
      if (IsScrlOn() != scrollLockOn){
        Serial.println("End Jiggling");
        Keyboard.press(KEY_SCROLL_LOCK);
        Keyboard.releaseAll();
        return;
      }
      delay(1000);
    }
  }  
}

int wanderIntervalSecs = 30;

void wander() {
  bool scrollLockOn = IsScrlOn();
  
  while (IsScrlOn() == scrollLockOn) {  
    
    Mouse.move(2, 6);  

    int j;
    for (j=0; j<wanderIntervalSecs; j++) {
      if (IsScrlOn() != scrollLockOn){
        Serial.println("End Wandering");
        Keyboard.press(KEY_SCROLL_LOCK);
        Keyboard.releaseAll();
        return;
      }
      delay(1000);
    }
  }  
}

void lightShow() {
  
  bool scrollLockOn = IsScrlOn(); 
  bool numLockOn = IsNumbOn();   
  bool capsOn = IsCapsOn();

  if (IsCapsOn()) {
    Keyboard.press(KEY_CAPS_LOCK);
    Keyboard.releaseAll();
  }
  if (IsScrlOn()) {
    Keyboard.press(KEY_SCROLL_LOCK);
    Keyboard.releaseAll();
  }
  if (IsNumbOn()) {
    Keyboard.press(KEY_NUM_LOCK);
    Keyboard.releaseAll();
  }

  int i;  
  for (int i = 0; i<20; i++) {  
    
    Keyboard.press(KEY_CAPS_LOCK);
    Keyboard.releaseAll();

    delay(200);
    
    Keyboard.press(KEY_SCROLL_LOCK);
    Keyboard.releaseAll();
    
    delay(200);
 
    Keyboard.press(KEY_NUM_LOCK);
    Keyboard.releaseAll();
    
    delay(200);    
  }  

  if (capsOn && !IsCapsOn())
    Keyboard.press(KEY_CAPS_LOCK);

  if (scrollLockOn && !IsScrlOn())
    Keyboard.press(KEY_SCROLL_LOCK);

  if (numLockOn && !IsNumbOn())
    Keyboard.press(KEY_NUM_LOCK);

  Keyboard.releaseAll();
}

void ledOn() {
  digitalWrite(ledPin, HIGH);  
}

void ledOff() {
  digitalWrite(ledPin, LOW);  
}


