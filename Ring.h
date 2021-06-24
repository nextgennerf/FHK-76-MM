#ifndef MODE
#define MODE
enum class Mode {STATIC, BREATHE, CYCLE, SOLID_SPIN, FADE_SPIN, RAINBOW_SPIN};
#endif

//Represents a ring of 24 GRB NeoPixels
#ifndef RING_H
#define RING_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define OFFSET 18 //shifts pixel locations to align position 0 with directly above the barrel

enum class RColor {SINGLE, RAINBOW, RGB, YCM, RYGCBM};

class Ring {
  public:
    Ring(int pin);
    void init(int brightness);
    void newPattern(Mode mode, int cycle, bool clw); //used for rainbow spin
    void newPattern(Mode mode, int num, RColor color, uint16_t colorArg, int cycle = 0, bool clw = false);
    void changeTime(int t);
    void changeHue(uint16_t h);
    void changeStep(int s);
    void animate();
    void tick();
    void cStep();
    bool isReady();
  private:
    int rotationTarget(int secs);
    uint16_t hue(uint32_t color);
    void setNewColors(int num, RColor color, uint16_t colorArg);
    void resetTimers(Mode mode, RColor color);
    Adafruit_NeoPixel _np;
    int _numPixels;
    Mode _mode;
    int _spacing;
    bool _alternate;
    RColor _colorType;
    uint32_t _rgbColor;
    uint16_t _h;
    uint8_t _v;
    int _deltaP;
    int _deltaV;
    volatile int _count;
    int _countTarget;
    volatile int _step;
    int _stepTarget;
    bool _clockwise;
    uint32_t _red;
    uint32_t _green;
    uint32_t _blue;
    uint32_t _cyan;
    uint32_t _magenta;
    uint32_t _yellow;
};

#endif
