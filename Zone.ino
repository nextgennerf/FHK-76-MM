//A class wrapper for a NeoPixel "zone"

class Zone {
  int startPixel;
  int zoneLength;
  Adafruit_NeoPixel strip;

public:
  Zone(Adafruit_NeoPixel parent, int pixel, int len) {
    startPixel = pixel;
    zoneLength = len;
    strip = parent;
  }
};
