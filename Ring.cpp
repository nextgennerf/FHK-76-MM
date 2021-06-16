#include "Ring.h"

//TODO: Rotation functions (start, stop, change period)

Ring::Ring(int numPix, int pin) {
  _np = Adafruit_NeoPixel(numPix, pin, NEO_GRB + NEO_KHZ800);
  _size = numPix;
//  _rotating = false;
  _colors[_size][3];
  for (int i = 0; i < _size; i++) {
    _colors[i][0] = 0;
    _colors[i][1] = 0;
    _colors[i][2] = 0;
  }
  _mode = Mode::STATIC;
}

void Ring::init(int brightness) {
  _np.begin();
  _np.setBrightness(brightness);
  _np.show();
}

void Ring::setColor(uint16_t h, uint8_t s, uint8_t v) {
  _uniform = true;
  _h = h;
  _s = s;
  _v = v;
  if (h == 0 && s == 0 && v == 0) {
    _np.clear();
  } else {
    _np.fill(_np.gamma32(_np.ColorHSV(_h, _s, _v)));
  }
  _np.show();
}

void Ring::setSingleColor(int index, uint16_t h, uint8_t s, uint8_t v) {
  _uniform = false;
  _colors[index][0] = h;
  _colors[index][1] = s;
  _colors[index][2] = v;
  _np.setPixelColor(index, _np.gamma32(_np.ColorHSV(h, s, v)));
  _np.show();
}

void Ring::animate() {
  if (_mode != Mode::STATIC && _stepCount >= _stepTarget) {
    _stepCount = 0;
    switch (_mode) {
      case Mode::BREATHE:
        _v += _factor;
        if (_factor == 0) {
          _factor = -1;
        } else if (_v == 255) {
          _factor = 0;
        } else if (_v == 0) {
          _factor = 1;
        }
        if (_uniform) {
          _np.fill(_np.gamma32(_np.ColorHSV(_h, _s, _v)));
        } else {
          for (int i = 0; i < _size; i++)
            _np.setPixelColor(i, _np.gamma32(_np.ColorHSV(_colors[i][0], _colors[i][1], _colors[i][2])));
        }
        break;
      default:
        break;
    }
    _np.show();
  }
//  if (!_uniform && _mode == Mode::CYCLE) {
//    //TODO: Figure out what rotating cycle should look like
//  } else {
//    returnVal = animate();
//  }
//  if (_rotating && _rotateCount >= _rotateTarget) {
//    if (_clockwise) {
//      int last[3] = {_colors[_length - 1]};
//      for (int i = _length - 1; i > 0; i--) {
//        _colors[i][0] = _colors[i - 1][0];
//        _colors[i][1] = _colors[i - 1][1];
//        _colors[i][2] = _colors[i - 1][2];
//      }
//      _colors[0][0] = last[0];
//      _colors[0][1] = last[1];
//      _colors[0][2] = last[2];
//    } else {
//      int first[3] = {_colors[0]};
//      for (int i = 0; i < _length - 1; i++) {
//        _colors[i][0] = _colors[i + 1][0];
//        _colors[i][1] = _colors[i + 1][1];
//        _colors[i][2] = _colors[i + 1][2];
//      }
//      _colors[_length - 1][0] = first[0];
//      _colors[_length - 1][1] = first[1];
//      _colors[_length - 1][2] = first[2];
//    }
//    _rotateCount = 0;
//    returnVal = true;
//  }
//  return returnVal;
}

void Ring::changeMode(Mode mode, int interval, Direction dir) {
  _stepTarget = interval;
  if (mode == Mode::STATIC) {
    cli();
    TIMSK2 &= ~(1 << OCIE2A); //Disable the timer interrupt
    sei();
    if (!_uniform) {
      switch (_mode) {
        case Mode::BREATHE:
          for (int i = 0; i < _size; i++)
            _colors[i][2] = _v;
        default:
          break;
      }
    }
  } else {
    switch (mode) {
      case Mode::BREATHE:
        if (!_uniform) {
          _v = 0;
          for (int i = 0; i < _size; i++) {
            if (_colors[i][2] != 0 && _colors[i][2] > _v)
              _v = _colors[i][2];
          }
        }
        if (_v == 255) {
          _factor = -1;
        } else {
          _factor = 1;
        }
        break;
//      case Mode::CYCLE:
//        _factor = 128;
//        _h = (_h / 128) * 128; // rounds _h down to a multiple of 128
//        _s = 255;
//        _v = 255;
//        if (!_rotating)
//          _uniform = true;
//        TIMSK2 |= 1 << OCIE2A; //Enable the timer interrupt
//        break;
      default:
        break;
    }
    cli();
    TIMSK2 |= 1 << OCIE2A; //Enable the timer interrupt
    sei();
  }
  _mode = mode;
}

void Ring::tick() {
  _stepCount++;
//  if (_rotating)
//    _rotateCount++;
}

//void Ring::updateColor() {
//  if (_uniform) {
//  } else if (_mode == Mode::BREATHE) {
//    for (int i = 0; i < _length; i++) {
//      _np.setPixelColor(_id + i, _np.gamma32(_np.ColorHSV(_colors[i][0], _colors[i][1], _v)));
//    }
//  } else {
//    for (int i = 0; i < _length; i++) {
//      _np.setPixelColor(_id + i, _np.gamma32(_np.ColorHSV(_colors[i][0], _colors[i][1], _colors[i][2])));
//    }
//  }
//}
