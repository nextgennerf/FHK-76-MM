// Peripheral control board (Adafruit Metro Mini) for the FHK-76

/* MISSING METHODS
 * change LED color
 * LED animations (rotate for front ring, cycle for front ring and sides)
 * Serial commands for changing LED colors, and changing LED animations
 */
 
#include "Ring.h"
//#include "Pixel.h"

//#define SIMULATED
//#define DEBUG

//#define TRIGGER 2
//#define PUMP 3
//#define VALVE 4
#define RINGPIN 6
#define RINGSIZE 24
#define PIXPIN 7
#define PIXCOUNT 2
#define LEFT 0
#define RIGHT 1
#define BRIGHTNESS 50
//#define PRESSURE A0
//#define MIN_OPEN_TIME 512 //equivalent to 1 sec

Ring ring = Ring(RINGSIZE, RINGPIN);
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXCOUNT, PIXPIN, NEO_RGB + NEO_KHZ800);
//Pixel leftPixel = Pixel(pixels, 0);
//Pixel rightPixel = Pixel(pixels, 1);
//long setPressure = 0L;
//bool charging = false;
//volatile int openTime = 0L;
//volatile bool valveOpen = false;

#ifdef SIMULATED
  #define SIM_DELAY 5
  volatile long simVal = 0L;
  long lastChange = 0L;
#endif

#ifdef DEBUG
  volatile long pressureValue = 0L;
#endif

void setup() {
//  pinMode(PUMP, OUTPUT);
//  digitalWrite(PUMP, LOW);
//  pinMode(VALVE, OUTPUT);
//  digitalWrite(VALVE, LOW);
//  pinMode(PRESSURE, INPUT);
//  pinMode(TRIGGER, INPUT);
//  attachInterrupt(digitalPinToInterrupt(TRIGGER), trigger, CHANGE);
  ring.init(BRIGHTNESS);
//  pixels.begin();
//  pixels.clear();
//  pixels.show();
//  pixels.setBrightness(BRIGHTNESS);
  #ifdef SIMULATED
    randomSeed(analogRead(PRESSURE));
  #endif
  cli();
  TCCR2A |= (1 << WGM21); //Timer 2 in CTC mode
  TCCR2B |= (1 << CS22) | (1 << CS20); // CLK/128 prescaler
  OCR2A = 0xF4; //ISR should trigger every 244 ticks
  sei();
  Serial.begin(9600);
  Serial.println("ready");
}

void loop() {
  //check for Serial commands and executes the first one of any that are available
  if (Serial.available()) {
    char rawInput[50];
    int numChars = Serial.readBytesUntil(';', rawInput, sizeof(rawInput));
    String cmd;
    for (int n = 0; n < numChars; n++) {
      cmd += rawInput[n];
    }
    if (cmd.startsWith("set")) {
//      cmd.remove(0,4);
//      setPressure = cmd.toInt() * 1000L;
      #ifdef DEBUG
        Serial.print("Pressure set to ");
        Serial.println(setPressure);
      #endif
    } else if (cmd == "request") {
//      float fVal = readPressure() / 1000.0;
//      int dp = 3;
//      if (fVal >= 10)
//        dp--;
//      if (fVal >= 100)
//        dp--;
//      Serial.println(String(fVal, dp));
    } else if (cmd.startsWith("pixel")) {
//      processPixelCommand(cmd.substring(6));
    } else if (cmd.startsWith("ring")) {
      processRingCommand(cmd.substring(5));
    }
    #ifdef DEBUG
      if (cmd.startsWith("pressure")) {
        pressureValue = cmd.substring(9).toFloat() * 1000L;
      }
    #endif
  }
//  if (readPressure() < 0.9 * setPressure and openTime == 0 and not charging) {
//    //check tank pressure, start charging if below 90% of target pressure
//    digitalWrite(PUMP, HIGH);
//    charging = true;
//  }
//  if (readPressure() >= setPressure and charging) {
//    //check tank pressure, stop charging if at least 100% of target pressure
//    charging = false;
//    digitalWrite(PUMP, LOW);
//  }
//  if (openTime != 0 and millis() - openTime >= MIN_OPEN_TIME) {
//    //if the trigger was released before the minimum time elapsed, this will close it instead
//    digitalWrite(VALVE, LOW);
//    valveOpen = false;
//    openTime = 0;
//  }
//  if (leftPixel.animate() || rightPixel.animate()) {
//    //check if the singlee NeoPixels need to be updated
//    println("Doin' pixel stuff!");
//    pixels.show();
//  }
  ring.animate();
  #ifdef SIMULATED
    long timeNow = millis();
    if (charging and (lastChange + SIM_DELAY <= timeNow)) {
      lastChange = timeNow;
      simVal += random(10);
    }
  #endif
}

//Process a Serial instruction related to a single NeoPixel
//void processPixelCommand(String cmd) {
//  int ws = cmd.indexOf(" ");
//  int id = cmd.substring(0, ws).toInt();
//  cmd.remove(0, ws + 1);
//  ws = cmd.indexOf(" ");
//  if (isDigit(cmd.charAt(0))) {
//    int h = cmd.substring(0, ws).toInt();
//    cmd.remove(0, ws + 1);
//    ws = cmd.indexOf(" ");
//    int s = cmd.substring(0, ws).toInt();
//    int v = cmd.substring(ws + 1).toInt();
//    switch (id) {
//      case LEFTPIX:
//        leftPixel.setColor(h, s, v);
//        break;
//      case RIGHTPIX:
//        rightPixel.setColor(h, s, v);
//        break;
//      default:
//        pixelRing.setSingleColor(id, h, s, v);
//        break;
//    }
//  } else {
//    int interval = 0;
//    Mode mode;
//    if (cmd == "static") {
//      mode = getMode(cmd);
//    } else {
//      mode = getMode(cmd.substring(0, ws));
//      interval = cmd.substring(ws + 1).toInt();
//    }
//    switch (id) {
//      case LEFTPIX:
//        leftPixel.changeMode(mode, interval);
//        break;
//      case RIGHTPIX:
//        rightPixel.changeMode(mode, interval);
//        break;
//      default:
//        //TODO (ring)
//        break;
//    }
//  }
//}

//Process a Serial instruction related to the NeoPixel ring
void processRingCommand(String cmd) {
  int ws = cmd.indexOf(" ");
  if (isDigit(cmd.charAt(0)) || cmd.startsWith("p")) {
    bool single = cmd.startsWith("p");
    int index;
    if (single) {
      index = cmd.substring(1, ws).toInt();
      cmd.remove(0, ws + 1);
      ws = cmd.indexOf(" ");
    }
    int h = cmd.substring(0, ws).toInt();
    cmd.remove(0, ws + 1);
    ws = cmd.indexOf(" ");
    int s = cmd.substring(0, ws).toInt();
    int v = cmd.substring(ws + 1).toInt();
    if (single) {
      ring.setSingleColor(index, h, s, v);
    } else {
      ring.setColor(h, s, v);
    }
  } else if (ws == -1) { //static is the only command with no arguments and therefore no whitespace
      ring.changeMode(Mode::STATIC, 0, Direction::NONE);
  } else {
    Mode mode = getMode(cmd.substring(0, ws));
    int lws = cmd.lastIndexOf(" ");
    int interval = cmd.substring(lws + 1).toInt();
    Direction dir = Direction::NONE;
    if (ws != lws)
      dir = getDirection(cmd.substring(ws + 1, lws));
    ring.changeMode(mode, interval, dir);
  }
}

////read pressure from sensor
//long readPressure() {
////  long pressureVal = (1500*(long)analogRead(PRESSURE)-153450)/8184; Where did this come from?
//  #ifdef SIMULATED
//    return simVal;
//  #else
//    #ifdef DEBUG
//      return pressureValue;
//    #else
//      long pressure = 1000 * (75 * (5 * (long)analogRead(PRESSURE) - 512)) / 2048;
//      if (pressure < 0)
//        pressure = 0L;
//      return pressure;
//    #endif
//  #endif
//}

Mode getMode(String modeStr) {
  if (modeStr == "static") {
    return Mode::STATIC;    
  } else if (modeStr == "breathe") {
    return Mode::BREATHE;
  } else if (modeStr == "cycle") {
    return Mode::CYCLE;
  } else if (modeStr == "rotate") {
    return Mode::ROTATE;
  } else {
    return Mode::NONE;
  }
}

Direction getDirection(String dirStr) {
  if (dirStr == "clw") {
    return Direction::CLOCKWISE;    
  } else if (dirStr == "ccw") {
    return Direction::COUNTERCLOCKWISE;
  } else if (dirStr == "off") {
    return Direction::OFF;
  } else {
    return Direction::NONE;
  }
}

//ISR that responds to the trigger
//void trigger() {
//  if (digitalRead(TRIGGER) == LOW) {
//    digitalWrite(VALVE, HIGH);
//    valveOpen = true;
//    #ifdef SIMULATED
//      simVal = 0L;
//    #endif
//    #ifdef DEBUG
//      pressureValue = 0L;
//    #endif
//  } else if (openTime >= MIN_OPEN_TIME) { //close valve on trigger release if greater than minimum open time
//    digitalWrite(VALVE, LOW);
//    valveOpen = false;
//    openTime = 0;
//  }
//}

//Timer2 CTC ISR
ISR(TIMER2_COMPA_vect) {
//  leftPixel.tick();
//  rightPixel.tick();
  ring.tick();
//  if (valveOpen)
//    openTime += 1;
}
