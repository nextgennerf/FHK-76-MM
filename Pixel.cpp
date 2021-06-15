#include "Pixel.h"

Pixel::Pixel(Adafruit_NeoPixel np, int count) {
  _np = np;
  _count = count;
}

//void Pixel::setColor(uint16_t h, uint8_t s, uint8_t v) {
//  _h = h;
//  _s = s;
//  _v = v;
//  updateColor();
//  _np.show();
//}

//bool Pixel::animate() {
//  if (_mode != Mode::STATIC && _stepCount >= _stepTarget) {
//    switch (_mode) {
//      case Mode::BREATHE:
//        _v += _factor;
//        if (_factor == 0) { _factor = -1; }
//        else if (_v == 255) { _factor = 0; }
//        else if (_v == 0) { _factor == 1; }
//        break;
//      case Mode::CYCLE:
//        _h += 128;
//        break;
//      default:
//        break;
//    }
//    _stepCount = 0;
//    updateColor();
//    return true;
//  } else {
//    return false;
//  }
//}

//void Pixel::changeMode(Mode mode, int interval = 0) {
//  _mode = mode;
//  _stepTarget = interval;
//  switch (mode) {
//    case Mode::BREATHE:
//      if (_v == 255) {
//        _factor = -1;
//      } else {
//        _factor = 1;
//      }
//      break;
//      TIMSK2 |= 1 << OCIE2A; //Enable the timer interrupt
//    case Mode::CYCLE:
//      _factor = 128;
//      _h = (_h / 128) * 128; // rounds _h down to a multiple of 128
//      _s = 255;
//      _v = 255;
//      TIMSK2 |= 1 << OCIE2A; //Enable the timer interrupt
//      break;
//    default:
//      TIMSK2 &= ~(1 << OCIE2A); //Disable the timer interrupt
//      break;
//  }
//}

//void Pixel::tick() {
//  _stepCount++;
//}

//void Pixel::updateColor() {
//  _np.setPixelColor(_id, _np.gamma32(_np.ColorHSV(_h, _s, _v)));
//}
