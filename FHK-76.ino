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

Ring ring = Ring(RINGPIN);
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXCOUNT, PIXPIN, NEO_RGB + NEO_KHZ800);
//Pixel leftPixel = Pixel(pixels, 0);
//Pixel rightPixel = Pixel(pixels, 1);
//long setPressure = 0L;
//bool charging = false;
//volatile int openTime = 0L;
//volatile bool valveOpen = false;
//int msgNum = 0;

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
//  cli();
//  TCCR2A |= (1 << WGM21); //Timer 2 in CTC mode
//  TCCR2B |= (1 << CS22) | (1 << CS20); // CLK/128 prescaler
//  OCR2A = 0xF4; //ISR for animation should trigger every 244 ticks
//  sei();
  Serial.begin(115200);
  Serial.println("ready");
}

void loop() {
  //check for Serial commands and executes the first one of any that are available
  if (Serial.available()) {
    char rawInput[50];
    int numChars = Serial.readBytesUntil(';', rawInput, sizeof(rawInput));
//    if (numChars == sizeof(rawInput))
//      Serial.println("Buffer overflow!");
    while (not ring.isReady()) {}
    String cmd[7];
    int index = 0;
    for (int n = 0; n < numChars; n++) {
      if (rawInput[n] == ' ') {
        index++;
      } else {
        cmd[index] += rawInput[n];
      }
    }
    if (cmd[0] == "set") {
//      setPressure = cmd[1].toInt() * 1000L;
      #ifdef DEBUG
        Serial.print("Pressure set to ");
        Serial.println(setPressure);
      #endif
    } else {
      Serial.println("ack");
      //Serial.println(cmd[0]);
      if (cmd[0] == "request") {
  //      float fVal = readPressure() / 1000.0;
  //      int dp = 3;
  //      if (fVal >= 10)
  //        dp--;
  //      if (fVal >= 100)
  //        dp--;
  //      Serial.println(String(fVal, dp));
      } else if (cmd[0] == "pixel") {
  //      processPixelCommand(cmd);
      } else if (cmd[0] == "ring") {
        processRingCommand(cmd);
      } else {
        String msg = "";
        for (int i = 0; i < 7; i++) {
          msg += cmd[1];
          if (i < 6)
            msg += " ";
        }
        Serial.println(msg);
      }
      #ifdef DEBUG
        if (cmd[0] =="pressure") {
          pressureValue = cmd[1].toFloat() * 1000L;
        }
      #endif
    }
    //Serial.println(++msgNum);
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
//  ring.animate();
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
void processRingCommand(String cmd[]) {
  if (cmd[1] == "change") {
    if (cmd[2] == "time") {
      ring.changeTime(cmd[3].toInt());
    } else if (cmd[2] == "hue") {
      ring.changeHue(cmd[3].toInt());
    } else if (cmd[2] == "step") {
      ring.changeStep(cmd[3].toInt());
    }
  } else {
    Mode mode = getMode(cmd[1]);
    int layoutNum = cmd[2].toInt();
    if (cmd[2].endsWith("a"))
      layoutNum = -layoutNum;
    switch (mode) {
      case Mode::STATIC:
        ring.newPattern(mode, layoutNum, getColor(cmd[3]), (uint16_t)cmd[4].toInt());
        break;
      case Mode::BREATHE:
        ring.newPattern(mode, layoutNum, getColor(cmd[3]), (uint16_t)cmd[4].toInt(), cmd[5].toInt());
        break;
      case Mode::RAINBOW_SPIN:
        ring.newPattern(mode, cmd[2].toInt(), cmd[3] == "clw");
        break;
      case Mode::SOLID_SPIN:
      case Mode::FADE_SPIN:
        ring.newPattern(mode, layoutNum, getColor(cmd[3]), (uint16_t)cmd[4].toInt(), cmd[5].toInt(), cmd[6] == "clw");
        break;
      default:
        String msg = "";
        for (int i = 0; i < 7; i++) {
          msg += cmd[1];
          if (i < 6)
            msg += " ";
        }
        Serial.println(msg);
    }
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
  } else if (modeStr == "spin") {
    return Mode::SOLID_SPIN;
  } else if (modeStr == "fade") {
    return Mode::FADE_SPIN;
  } else if (modeStr == "rainbow") {
    return Mode::RAINBOW_SPIN;
  }
}

RColor getColor(String cStr) {
  if (cStr == "single") {
    return RColor::SINGLE;    
  } else if (cStr == "rainbow") {
    return RColor::RAINBOW;
  } else if (cStr == "rgb") {
    return RColor::RGB;
  } else if (cStr == "ycm") {
    return RColor::YCM;
  } else if (cStr == "rygcbm") {
    return RColor::RYGCBM;
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

//Timer2 CTC ISR for animation
ISR(TIMER2_COMPA_vect) {
//  leftPixel.tick();
//  rightPixel.tick();
  ring.tick();
//  if (valveOpen)
//    openTime += 1;
}

//Timer2 CTC ISR for color steps
ISR(TIMER2_COMPB_vect) {
  ring.cStep();
}
