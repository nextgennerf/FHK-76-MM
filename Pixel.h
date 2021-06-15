#ifndef MODE
#define MODE
enum class Mode {STATIC, BREATHE, CYCLE};
#endif

//Represents a single RGB NeoPixel
#ifndef PIXEL_H
#define PIXEL_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Pixel {
  public:
    Pixel(Adafruit_NeoPixel np, int count);
//    virtual void setColor(uint16_t h, uint8_t s, uint8_t v);
//    bool animate();
//    virtual void changeMode(Mode mode, int interval);
//    virtual void tick();
  private:
//    virtual void updateColor();
    Adafruit_NeoPixel _np;
    int _count;
//    uint16_t _h;
//    uint8_t _s;
//    uint8_t _v;
//    int _stepTarget;
//    Mode _mode;
//    uint8_t _factor;
//    volatile int _stepCount;
};

#endif
