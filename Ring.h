#ifndef MODE
#define MODE
enum class Mode {STATIC, BREATHE, CYCLE};
#endif

//Represents a ring of GRB NeoPixels
#ifndef RING_H
#define RING_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Ring {
  public:
    Ring(int numPix, int pin);
    void init(int brightness);
    void changeMode(Mode mode, int interval);
    void animate();
    void setColor(uint16_t h, uint8_t s, uint8_t v);
    void setSingleColor(int index, uint16_t h, uint8_t s, uint8_t v);
    void tick();
  private:
//    void updateColor();
    Adafruit_NeoPixel _np;
    int _size;
    bool _uniform;
    uint16_t _h;
    uint8_t _s;
    uint8_t _v;
    Mode _mode;
    int _factor;
    volatile int _stepCount;
    int _stepTarget;
//    bool _rotating;
//    volatile int _rotateCount;
//    int _rotateTarget;
//    bool _clockwise;
    int _colors[][3];
};

#endif
