#include <Adafruit_NeoPixel.h>

#define minf(a,b) (((a)<(b))?(a):(b))
#define maxf(a,b) (((a)>(b))?(a):(b))

#define PIN 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(291, PIN, NEO_GRB + NEO_KHZ800); //(1+44+(41*6))

#define R_IN 33
#define G_IN 34
#define B_IN 35

int16_t adc_color[3] = {0, 0, 0};
uint8_t hue_color[3] = {0, 0, 0};

uint16_t wakeup = false;

uint8_t ledMode = 0;
uint8_t modes = 3;

uint8_t ledHigh;
uint8_t ledLow;
uint16_t ledHighLowTim = 0;
uint16_t ledHighLowTimMax = 3000;
uint8_t modeCounter;

uint32_t firstPixelHue;

byte neopix_gamma[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

struct rgbywwcw {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t y;
  uint8_t ww;
  uint8_t cw;
};

rgbywwcw led;
rgbywwcw led_f;

typedef struct {
  double h;       // angle in degrees
  double s;       // a fraction between 0 and 1
  double v;       // a fraction between 0 and 1
} hsv;

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show();
  strip.setBrightness(255);
  delay(500);
}

uint16_t i;

void loop() {
  adc_color[0] = (analogRead(R_IN));
  adc_color[1] = (analogRead(G_IN));
  adc_color[2] = (analogRead(B_IN));

  if (adc_color[0] >= 1500 && adc_color[1] >= 3500 && adc_color[2] >= 3500) {
    ledHigh = 1;
    if (ledLow == 1) {
      if (ledHighLowTim >= millis()) {
        modeCounter++;
      }
      ledLow = 0;
      ledHighLowTim = millis() + ledHighLowTimMax;
    }
  }

  if (adc_color[0] <= 1400 && adc_color[1] >= 3500 && adc_color[2] >= 3500) {
    ledLow = 1;
    if (ledHigh == 1) {
      if (ledHighLowTim >= millis()) {
        modeCounter++;
      }
      ledHigh = 0;
      ledHighLowTim = millis() + ledHighLowTimMax;
    }
  }

  Serial.print(adc_color[0]);
  Serial.print("\t");
  Serial.print(adc_color[1]);
  Serial.print("\t");
  Serial.print(adc_color[2]);
  Serial.print("\t");
  Serial.print(ledHigh);
  Serial.print("\t");
  Serial.print(ledLow);
  Serial.print("\t");
  Serial.print(modeCounter);
  Serial.print("\t");
  Serial.print(millis());
  Serial.print("\t");
  Serial.println(ledHighLowTim);

  if (modeCounter >= 2) {
    strip.setBrightness(255);
    strip.fill(0);

    modeCounter = 0;
    ledMode++;
    if (ledMode >= modes) {
      ledMode = 0;
    }
  }

  if (modeCounter != 0 && ledHighLowTim <= millis()) {
    modeCounter = 0;
  }

  if (adc_color[0] >= 4094 && adc_color[1] >= 4094 && adc_color[2] >= 4094) {
    hue_color[0] = 0;
    hue_color[1] = 0;
    hue_color[2] = 0;
    wakeup = 50;
    setColor(rgbywwcw{0, 0, 0, 0, 0, 0});
    delay(50);
  } else {
    hue_color[0] = map(adc_color[0], 0, 4095, wakeup, 0);
    hue_color[1] = map(adc_color[1], 0, 4095, wakeup, 0);
    hue_color[2] = map(adc_color[2], 0, 4095, wakeup, 0);
    wakeup++;
    if (wakeup > 255) {
      wakeup = 255;
    }
    setColor(_f(rgb_to_rgbw(hsv_to_rgby_naive(rgb2hsv(hue_color[0] / 255.0, hue_color[1] / 255.0, hue_color[2] / 255.0)))));
    delay(1);
  }
  delay(10);
}

hsv test(uint16_t i) {
  hsv _hsv;
  _hsv.s = 1.0;
  _hsv.v = 1.0;
  _hsv.h = i;
  return _hsv;
}

void setColor(rgbywwcw in) {
  uint32_t col = 0;
  uint32_t white = 0;

  // LED 0 BLACK; 41 LED per Panel, 6 Panel; 44 LED for the base

  col = strip.Color(neopix_gamma[in.r], neopix_gamma[in.g], neopix_gamma[in.b]);
  white =  strip.Color(neopix_gamma[in.y], neopix_gamma[in.cw], neopix_gamma[in.ww]);

  if (ledMode == 0) {
    strip.fill(col);

    for (uint16_t i = (41 * 6) + 1; i < strip.numPixels(); i += 2) {
      strip.setPixelColor(i, col);
    }

    for (uint16_t i = (41 * 6) + 2; i < strip.numPixels(); i += 2) {
      strip.setPixelColor(i, white);
    }

  } else if (ledMode == 1) {
    uint8_t nLED = 20;
    float sA = PI * 2 / nLED;
    float piDivFive[nLED] = {0, sA * 1, sA * 2, sA * 3, sA * 4, sA * 5, sA * 6, sA * 7, sA * 8, sA * 9, sA * 10, sA * 11, sA * 12, sA * 13, sA * 14, sA * 15, sA * 16, sA * 17, sA * 18, sA * 19};
    for (uint8_t j = 0; j < nLED; j++) {
      float mult = (sin((millis() / 10000.0) + piDivFive[j]) * (nLED - 1)) - (nLED - 2);
      if (mult <= 0) mult = 0;
      if (mult >= 1) mult = 1;
      uint32_t effectCol = strip.Color(neopix_gamma[in.r] * mult, neopix_gamma[in.g] * mult, neopix_gamma[in.b] * mult);
      for (uint16_t i = 1 + j; i < (41 * 6); i += nLED) {
        strip.setPixelColor(i, effectCol);
      }
    }

    for (uint16_t i = (41 * 6) + 2; i < strip.numPixels(); i += 2) {
      strip.setPixelColor(i, white);
    }

  } else if (ledMode == 2) {
    strip.setBrightness(neopix_gamma[in.ww]);
    firstPixelHue += 64;
    if (firstPixelHue >= 5 * 65536) firstPixelHue = 0;
    strip.rainbow(firstPixelHue);
  }

  strip.setPixelColor(0, (0, 0, 0));
  strip.show();
}

#define FILT_COEF 0.1
rgbywwcw _f(rgbywwcw in) {
  led_f.r = (led_f.r * (1.0 - FILT_COEF)) + (in.r * FILT_COEF);
  led_f.g = (led_f.g * (1.0 - FILT_COEF)) + (in.g * FILT_COEF);
  led_f.b = (led_f.b * (1.0 - FILT_COEF)) + (in.b * FILT_COEF);
  led_f.y = (led_f.y * (1.0 - FILT_COEF)) + (in.y * FILT_COEF);
  led_f.ww = (led_f.ww * (1.0 - FILT_COEF)) + (in.ww * FILT_COEF);
  led_f.cw = (led_f.cw * (1.0 - FILT_COEF)) + (in.cw * FILT_COEF);
  return led_f;
}


rgbywwcw calculateColorTemp(rgbywwcw in) {
  rgbywwcw out = in;
  float temp = (in.b - in.y);
  out.ww = min(uint8_t(min(int(out.ww + temp), 255)), out.ww);
  out.cw = min(uint8_t(min(int(out.cw - temp), 255)), out.cw);
  return out;
}

rgbywwcw rgb_to_rgbywwcw_struct(uint8_t r, uint8_t g, uint8_t b) {
  rgbywwcw out = {r, g, b, 0, 0, 0};
  return out;
}

rgbywwcw rgb_to_rgbw(rgbywwcw in) {
  rgbywwcw out;
  //Get the maximum between R, G, and B
  double tM = max(in.r, max(in.g, in.b));
  uint32_t ret = 0;

  //If the maximum value is 0, immediately return pure black.
  if (tM == 0) {
    out = {0, 0, 0, 0, 0, 0};
    return out;
  }

  //This section serves to figure out what the color with 100% hue is
  double multiplier = 255.0f / tM;
  double hR = in.r * multiplier;
  double hG = in.g * multiplier;
  double hB = in.b * multiplier;

  //This calculates the Whiteness (not strictly speaking Luminance) of the color
  double M = max(hR, max(hG, hB));
  double m = min(hR, min(hG, hB));
  double Luminance = ((M + m) / 2.0f - 127.5f) * (255.0f / 127.5f) / multiplier;

  //Calculate the output values
  int Wo = uint32_t(Luminance);
  int Bo = uint32_t(in.b - Luminance);
  int Ro = uint32_t(in.r - Luminance);
  int Go = uint32_t(in.g - Luminance);

  //Trim them so that they are all between 0 and 255
  if (Wo < 0) Wo = 0;
  if (Bo < 0) Bo = 0;
  if (Ro < 0) Ro = 0;
  if (Go < 0) Go = 0;
  if (Wo > 255) Wo = 255;
  if (Bo > 255) Bo = 255;
  if (Ro > 255) Ro = 255;
  if (Go > 255) Go = 255;

  out.r = Ro;
  out.g = Go;
  out.b = Bo;
  out.y = in.y;
  out.ww = Wo;
  out.cw = Wo / 2;

  return out;
}

hsv rgb2hsv(double r, double g, double b) {
  hsv         out;
  double      min, max, delta;

  min = r < g ? r : g;
  min = min  < b ? min  : b;

  max = r > g ? r : g;
  max = max  > b ? max  : b;

  out.v = max;                                // v
  delta = max - min;
  if (delta < 0.00001)
  {
    out.s = 0;
    out.h = 0; // undefined, maybe nan?
    return out;
  }
  if ( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max);                  // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, h is undefined
    out.s = 0.0;
    out.h = NAN;                            // its now undefined
    return out;
  }
  if ( r >= max )                          // > is bogus, just keeps compilor happy
    out.h = ( g - b ) / delta;        // between yellow & magenta
  else if ( g >= max )
    out.h = 2.0 + ( b - r ) / delta;  // between cyan & yellow
  else
    out.h = 4.0 + ( r - g ) / delta;  // between magenta & cyan

  out.h *= 60.0;                              // degrees

  if ( out.h < 0.0 )
    out.h += 360.0;

  return out;
}

rgbywwcw hsv_to_rgby_naive(hsv hsv) {
  rgbywwcw _tmpmdl;
  memset(&_tmpmdl, 0, sizeof(_tmpmdl));
  return hsv_to_rgby_naive(hsv.h / 360.0, hsv.s, hsv.v, _tmpmdl);
}

rgbywwcw hsv_to_rgby_naive(double h, double s, double v) {
  rgbywwcw _tmpmdl;
  memset(&_tmpmdl, 0, sizeof(_tmpmdl));
  return hsv_to_rgby_naive(h, s, v, _tmpmdl);
}


rgbywwcw hsv_to_rgby_naive(double h, double s, double v, rgbywwcw colorModel) {
  if (s == 0.0) {
    v = v * 255;

    colorModel =  (struct rgbywwcw) {
      v, v, v, v, colorModel.ww, colorModel.cw
    };
  }

  int i = int(h * 8.0);

  double f = (h * 8.0) - i;
  double b = v * (1.0 - s);
  double d = v * (1.0 - s * f);
  double u = v * (1.0 - s * (1.0 - f));
  i = i % 8;

  d = d * 255;
  b = b * 255;
  v = v * 255;
  u = u * 255;

  if (i == 0)colorModel =  (struct rgbywwcw) {
    v, b, b, u, colorModel.ww, colorModel.cw
  };

  if (i == 1)colorModel =  (struct rgbywwcw) {
    d, b, b, v, colorModel.ww, colorModel.cw
  };

  if (i == 2)colorModel =  (struct rgbywwcw) {
    b, u, b, v, colorModel.ww, colorModel.cw
  };

  if (i == 3)colorModel =  (struct rgbywwcw) {
    b, v, b, d, colorModel.ww, colorModel.cw
  };

  if (i == 4)colorModel =  (struct rgbywwcw) {
    b, v, u, b, colorModel.ww, colorModel.cw
  };

  if (i == 5)colorModel =  (struct rgbywwcw) {
    b, d, v, b, colorModel.ww, colorModel.cw
  };

  if (i == 6)colorModel =  (struct rgbywwcw) {
    u, b, v, b, colorModel.ww, colorModel.cw
  };

  if (i == 7)colorModel =  (struct rgbywwcw) {
    v, b, d, b, colorModel.ww, colorModel.cw
  };
  return colorModel;
}
