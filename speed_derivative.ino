#include "SR04.h"

#define TRIG_PIN 11
#define ECHO_PIN 12
#define BUTTON 6

const byte NUMBER_OF_PINS = 3;
byte ledPin[NUMBER_OF_PINS] = {8,9,10};

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

unsigned long currentTime, previousTime;
double distance, speed, previousDistance;
bool startReading = false;

int readings = 0;
int maxReadings = 50;

int nb_presses = 0;
boolean lastButtonState = LOW;
boolean currentButtonState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  for(int led: ledPin) {
    pinMode(led, OUTPUT);
  }
  pinMode(BUTTON, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  previousTime = millis();
  double previousDistance = 0;
}

void turnOnOff(int led) {
  for(int i = 0; i<NUMBER_OF_PINS; i++) {
    if (ledPin[i] == led) {
      digitalWrite(ledPin[i], HIGH);
    } else {
      digitalWrite(ledPin[i], LOW);     
    }   
  }
}

void loop() {
  currentButtonState = digitalRead(BUTTON);
  if (lastButtonState == LOW && currentButtonState == HIGH && startReading == false) {
    startReading = true;
    nb_presses++;
  } else if (lastButtonState == LOW && currentButtonState == HIGH && startReading == true) {
    startReading = false;
    nb_presses++;
    for(int led: ledPin) {
      digitalWrite(led, LOW);
    }
  }

  lastButtonState = currentButtonState;  

  if (startReading == true) {
    readings++;
    currentTime = millis();
    int distance = sr04.Distance();

    speed = (distance - previousDistance) / ((currentTime - previousTime) / 1000.0);
    previousDistance = distance;
    previousTime = currentTime;

    if (speed > 450) {
      turnOnOff(ledPin[0]);
    } else if (speed > 10) {
      turnOnOff(ledPin[2]);
    } else {
      turnOnOff(ledPin[1]);
    }

    Serial.print((currentTime - (nb_presses-1)*maxReadings*200)/1000.0);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(",");
    Serial.println(speed);
    delay(200);

    if (readings == maxReadings) {
      startReading = false;
      readings = 0;
    }
  }
}


