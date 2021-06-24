#include "Ring.h"

//TODO: Rotation functions (start, stop, change period)

Ring::Ring(int pin) {
  _np = Adafruit_NeoPixel(24, pin, NEO_GRB + NEO_KHZ800);

  uint32_t _red = _np.Color(255, 0, 0);
  uint32_t _green = _np.Color(0, 255, 0);
  uint32_t _blue = _np.Color(0, 0, 255);
  uint32_t _cyan = _np.Color(0, 255, 255);
  uint32_t _magenta = _np.Color(255, 0, 255);
  uint32_t _yellow = _np.Color(255, 255, 0);
}

void Ring::init(int brightness) {
  _np.begin();
  _np.clear();
  _np.setBrightness(brightness);
  _np.show();
}

bool Ring::isReady() {
  return _np.canShow();
}

void Ring::animate() {
//  bool change = false;
//  Serial.println("baz");
//  if (_mode != Mode::STATIC && _count >= _countTarget) {
//    _np.clear();
//    if (_mode == Mode::BREATHE) {
//      if (_deltaV == 0) {
//        _deltaV = -1;
//      } else if (_v == 255) {
//        _deltaV = 0;
//      } else if (_v == 0) {
//        _deltaV = 1;
//      }
//      _v += _deltaV;
//    } else {
//      _deltaP++;
//      _deltaP %= 24;
//    }
//    _count = 0;
//    change = true;
//  }
//  if (_colorType != RColor::SINGLE && _step >= _stepTarget) {
//    if (!change)
//      _np.clear();
//    if (_colorType == RColor::RAINBOW) {
//      _h += 256;
//      _rgbColor = _np.ColorHSV(_h, 255, _np.gamma8(_v));
//    } else {
//      bool six = _colorType == RColor::RYGCBM;
//      if (_rgbColor == _red) {
//        if (six) { _rgbColor = _yellow; } else { _rgbColor = _green; }
//      } else if (_rgbColor == _yellow) {
//        if (six) { _rgbColor = _green; } else { _rgbColor = _cyan; }
//      } else if (_rgbColor == _green) {
//        if (six) { _rgbColor = _cyan; } else { _rgbColor = _blue; }
//      } else if (_rgbColor == _cyan) {
//        if (six) { _rgbColor = _blue; } else { _rgbColor = _magenta; }
//      } else if (_rgbColor == _blue) {
//        if (six) { _rgbColor = _magenta; } else { _rgbColor = _red; }
//      } else if (_rgbColor == _magenta) {
//        if (six) { _rgbColor = _red; } else { _rgbColor = _yellow; }
//      }
//    }
//    _step = 0;
//    change = true;
//  }
//  if (change) {
//    int ind;
//    for (int i = 0; i < 24; i += _spacing) {
//      ind = i + OFFSET + _deltaP;
//      if (_alternate)
//        ind += _spacing / 2;
//      _np.setPixelColor(ind % 24, _rgbColor);
//    }
//    _np.show();
//  }
}

void Ring::newPattern(Mode mode, int cycle, bool clw) { //used for rainbow spin
//  if (mode == Mode::RAINBOW_SPIN) {
//    _mode = mode;
//    _deltaP = 0;
//    _clockwise = clw;
//    _numPixels = 24;
//    _spacing = 1;
//    _alternate = false;
//    _colorType = RColor::RAINBOW;
//    _v = 255;
//    _np.clear();
//    for (int i = 0; i < 24; i++) {
//      if (clw) {
//        _np.setPixelColor((i + OFFSET) % 24, _np.ColorHSV(i * 65536 / 24, 255, _v));
//      } else {
//        _np.setPixelColor((i + OFFSET) % 24, _np.ColorHSV((24 - i) * 65536 / 24, 255, _v));
//      }
//    }
//    _np.show();
//    _count = 0;
//    _countTarget = rotationTarget(cycle);
//    resetTimers(mode, _colorType);
//  }
}

void Ring::newPattern(Mode mode, int num, RColor color, uint16_t colorArg, int cycle = 0, bool clw = false) {
  _mode = mode;
  _deltaP = 0;
  _clockwise = clw;
  _np.clear();
  setNewColors(num, color, colorArg);
//  if (mode == Mode::FADE_SPIN)
//    for (int i = 0; i < num; i++)
//      for (int j = 1; j < _spacing; j++) {
//        if (clw) {
//          _np.setPixelColor((i * _spacing + j + OFFSET) % 24, \
//                            _np.ColorHSV(_h, 255, _np.gamma8(255 * (j - 1) / (_spacing - 1))));
//        } else {
//          _np.setPixelColor((i * _spacing + j + OFFSET) % 24, \
//                            _np.ColorHSV(_h, 255, _np.gamma8(255 * (1 - j / (_spacing - 1)))));
//        }
//      }
  _np.show();
//  _count = 0;
//  switch (mode) {
//    case Mode::STATIC:
//      _countTarget = 0;
//      break;
//    case Mode::BREATHE:
//      _countTarget = cycle;
//      break;
//    default:
//      _countTarget = rotationTarget(cycle);
//  }
//  resetTimers(mode, color);
}

void Ring::setNewColors(int num, RColor color, uint16_t colorArg) {
  _numPixels = num;
  _spacing = 24 / abs(num);
  _alternate = num < 0;
  _colorType = color;
  _v = 255;
  _deltaV = 0;
  if (color == RColor::SINGLE) {
    _h = colorArg;
    _rgbColor = _np.ColorHSV(_h, 255, 255);
  } else {
//    _stepTarget = colorArg * 10;
//    if (color == RColor::RAINBOW)
//      _stepTarget /= 5;
//    _rgbColor = _red;
//    if (color == RColor::YCM)
//      _rgbColor = _yellow;
//    _h = hue(_rgbColor);
  }
  int ind;
  for (int i = 0; i < 24; i += _spacing) {
    ind = i + OFFSET;
    if (_alternate)
      ind += _spacing / 2;
    _np.setPixelColor(ind % 24, _rgbColor);
  }
}

void Ring::resetTimers(Mode mode, RColor color) {
  cli();
  if (mode == Mode::STATIC) {
    TIMSK2 &= ~(1 << OCIE2A); //Disable the animation timer interrupt
  } else {
    _count = 0;
    TIMSK2 |= 1 << OCIE2A; //Enable the animation timer interrupt
  }
  if (color == RColor::SINGLE || mode == Mode::RAINBOW_SPIN) {
    TIMSK2 &= ~(1 << OCIE2B); //Disable the step timer interrupt
  } else {
    _step = 0;
    OCR2B = 0x7D; // triggers the ISR every 125 ticks
    if (color == RColor::RAINBOW)
      OCR2B = 0xF4; // 244 is the tick trigger target for rainbow color cycle only
    TIMSK2 |= 1 << OCIE2B; //Enable the step timer interrupt
  }
  sei();
}

int Ring::rotationTarget(int secs) {
  switch (secs) {
    case 5: return 107;
    case 6: return 128;
    case 7: return 149;
    case 8: return 171;
    case 9: return 192;
    case 10: return 213;
    case 11: return 235;
    case 12: return 256;
    case 13: return 277;
    case 14: return 299;
    case 15: return 320;
    case 16: return 342;
    case 17: return 363;
    case 18: return 384;
    case 19: return 406;
    case 20: return 427;
    case 21: return 448;
    case 22: return 470;
    case 23: return 491;
    case 24: return 512;
    case 25: return 534;
    case 26: return 555;
    case 27: return 576;
    case 28: return 598;
    case 29: return 610;
    case 30: return 640;
    default: return 0;
  }
}

uint16_t Ring::hue(uint32_t color) {
  uint16_t hd;
  if (color == _red) { hd = 0; }
  else if (color == _yellow) { hd = 60; }
  else if (color == _green) { hd = 120; }
  else if (color == _cyan) { hd = 180; }
  else if (color == _blue) { hd = 240; }
  else if (color == _magenta) { hd = 300; }
  return hd * 65536L / 360L;
}

void Ring::changeTime(int t) {
  if (_mode == Mode::BREATHE) {
    _countTarget = t;
  } else {
    _countTarget = rotationTarget(t);
  }
}

void Ring::changeHue(uint16_t h) {
  _h = h;
  _rgbColor = _np.ColorHSV(h, 255, _np.gamma8(_v));
  if (_mode == Mode::STATIC) { // animate() won't change anything so this needs to happen here
    _np.clear();
    Serial.println("bar");
    int ind;
    for (int i = 0; i < 24; i += _spacing) {
      ind = i + OFFSET + _deltaP;
      if (_alternate)
        ind += _spacing / 2;
      _np.setPixelColor(ind % 24, _rgbColor);
    }
    _np.show();
  }
}

void Ring::changeStep(int s) {
  _stepTarget = s * 10;
}

void Ring::tick() {
  _count++;
}

void Ring::cStep() {
  _step++;
}
