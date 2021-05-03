// Peripheral control board (Adafruit Metro Mini) for the FHK-76

/* MISSING METHODS
 * change LED color
 * LED animations (rotate for front ring, cycle for front ring and sides)
 * Serial commands for changing LED colors, and changing LED animations
 */

#include <Adafruit_NeoPixel.h>

#define SIMULATED

#define TRIGGER 2
#define PUMP 3
#define VALVE 4
#define PIXPIN 6
#define RINGSTART 0
#define RINGSIZE 24
#define LEFTPIX 24
#define RIGHTPIX 25
#define PRESSURE A0
#define MIN_OPEN_TIME 1000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(26, PIXPIN, NEO_GRB + NEO_KHZ800);
long setPressure = 0L;
bool charging = false;
volatile long openTime = 0L;

#ifdef SIMULATED
  volatile long simVal = 0L;
#endif

void setup() {
  Serial.begin(9600);
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);
  pinMode(VALVE, OUTPUT);
  digitalWrite(VALVE, LOW);
  pinMode(PRESSURE, INPUT);
  pinMode(TRIGGER, INPUT);
  attachInterrupt(digitalPinToInterrupt(TRIGGER), trigger, CHANGE);
  pixels.begin();
  pixels.show();
  #ifdef SIMULATED
    randomSeed(analogRead(PRESSURE));
  #endif
}

void loop() {
  //check for Serial commands and executes the first one of any that are available
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('.');
    if (cmd.startsWith("set")) {
      cmd.remove(0,4);
      setPressure = cmd.toInt() * 1000L;
    } else if (cmd == "request") {
      float fVal = readPressure() / 1000.0;
      int dp = 3;
      if (fVal >= 10)
        dp--;
      if (fVal >= 100)
        dp--;
      Serial.println(String(fVal, dp));
    }
  }
  //check tank pressure, start charging if below 90% of target pressure, or stop charging if at least 100% of target pressure
  if (readPressure() < 0.9 * setPressure and openTime == 0 and not charging) {
    digitalWrite(PUMP, HIGH);
    charging = true;
  }
  if (readPressure() >= setPressure and charging) {
    charging = false;
    digitalWrite(PUMP, LOW);
  }
  //if the trigger was released before the minimum time elapsed, this will close it instead
  if (openTime != 0 and millis() - openTime >= MIN_OPEN_TIME) {
    openTime = 0L;
    digitalWrite(VALVE, LOW);
  }
  #ifdef SIMULATED
    if (charging)
      simVal += random(1000);
  #endif
}

//read pressure from sensor
long readPressure() {
//  long pressureVal = (1500*(long)analogRead(PRESSURE)-153450)/8184; Where did this come from?
  #ifdef SIMULATED
    return simVal;
  #else
    long pressure = 1000 * (75 * (5 * (long)analogRead(PRESSURE) - 512)) / 2048;
    if (pressure < 0)
      pressure = 0L;
    return pressure;
  #endif
}

//respond to the trigger
void trigger() {
  if (digitalRead(TRIGGER) == LOW) {
    openTime = millis();
    digitalWrite(VALVE, HIGH);
    #ifdef SIMULATED
      simVal = 0L;
    #endif
  } else if (millis() - openTime >= MIN_OPEN_TIME) { //close valve on trigger release if greater than minimum open time
    openTime = 0L;
    digitalWrite(VALVE, LOW);
  }
}
