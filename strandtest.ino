
#include <Adafruit_NeoPXL8.h>

#define NUM_LEDS 96
// For the Feather RP2040 SCORPIO, use this list:
int8_t pins[8] = {16, 17, 18, 19, 20, 21, 22, 23};

// Here's the global constructor as explained near the start:
Adafruit_NeoPXL8HDR leds(NUM_LEDS, pins, NEO_RGB + NEO_KHZ800);

// See generate_lookup.py - normalized sqrt(sin(x)) for x in [0, 1]
uint8_t const sin_wave[256] PROGMEM =
// {0,3,6,9,12,15,18,21,25,28,31,34,37,40,43,46,49,53,56,59,62,65,68,71,74,77,80,83,86,89,92,95,97,100,103,106,109,112,115,117,120,123,126,128,131,134,136,139,142,144,147,149,152,154,157,159,162,164,167,169,171,174,176,178,180,183,185,187,189,191,193,195,197,199,201,203,205,207,209,210,212,214,215,217,219,220,222,223,225,226,228,229,230,232,233,234,236,237,238,239,240,241,242,243,244,245,246,246,247,248,249,249,250,250,251,251,252,252,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,254,254,253,253,253,252,252,251,251,250,250,249,249,248,247,246,246,245,244,243,242,241,240,239,238,237,236,234,233,232,230,229,228,226,225,223,222,220,219,217,215,214,212,210,209,207,205,203,201,199,197,195,193,191,189,187,185,183,180,178,176,174,171,169,167,164,162,159,157,154,152,149,147,144,142,139,136,134,131,128,126,123,120,117,115,112,109,106,103,100,97,95,92,89,86,83,80,77,74,71,68,65,62,59,56,53,49,46,43,40,37,34,31,28,25,21,18,15,12,9,6,3,0};
{0,28,40,49,56,63,69,74,79,84,89,93,97,101,105,109,112,116,119,122,125,128,131,134,137,140,143,145,148,150,153,155,158,160,162,164,167,169,171,173,175,177,179,181,183,185,186,188,190,192,193,195,197,198,200,201,203,204,206,207,209,210,212,213,214,216,217,218,219,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,238,239,240,241,241,242,243,244,244,245,245,246,247,247,248,248,249,249,250,250,250,251,251,252,252,252,252,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,253,253,253,253,252,252,252,252,251,251,250,250,250,249,249,248,248,247,247,246,245,245,244,244,243,242,241,241,240,239,238,238,237,236,235,234,233,232,231,230,229,228,227,226,225,224,223,222,221,219,218,217,216,214,213,212,210,209,207,206,204,203,201,200,198,197,195,193,192,190,188,186,185,183,181,179,177,175,173,171,169,167,164,162,160,158,155,153,150,148,145,143,140,137,134,131,128,125,122,119,116,112,109,105,101,97,93,89,84,79,74,69,63,56,49,40,28,0};
/*
*** FIRST TIME HERE? START W/THE NEOPXL8 (not HDR) strandtest INSTEAD! ***
That code explains and helps troubshoot wiring and NeoPixel color format.

Adafruit_NeoPXL8HDR is a subclass of Adafruit_NeoPXL8 that provides
16-bits-per-channel color, temporal dithering, and gamma correction.
This is a minimal demonstration of "HDR" vs regular NeoPXL8 to show the
extra setup required. It runs an animation loop similar to the non-HDR
strandtest example, with the color fills on startup removed...those are
for troubleshooting wiring and color order...work through that in the
easier non-HDR strandtest and then copy your findings here. You'll
find MUCH more documentation in that example.

Most important thing to note here is that different chips (RP2040, SAMD51)
require very different startup code. Animation loop is identical to NeoPXL8,
just the setup changes. SAMD21 is NOT supported; HDR is just too demanding.
SAMD51 requires the Adafruit_ZeroTimer library.
*/

// SETUP (VERY DIFFERENT from "regular" NeoPXL8) ---------------------------

// On RP2040, the refresh() function is called in a tight loop on the
// second core (via the loop1() function). The first core is then 100%
// free for animation logic in the regular loop() function.

void loop1()
{
  leds.refresh();
}

void setup()
{

  // In Adafruit_NeoPXL8, begin() normally doesn't take arguments. The HDR
  // version has some extra flags to select frame blending, added "depth"
  // for temporal dithering, and whether to double-buffer DMA pixel writes:
  if (!leds.begin(true, 4, true))
  {
    // If begin() returns false, either an invalid pin list was provided,
    // or requested too many pixels for available memory (HDR requires
    // INORDINATE RAM). Blink onboard LED to indicate startup problem.
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;)
      digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  // NeoPXL8HDR can provide automatic gamma-correction, but it's disabled
  // by default; pixel values correlate to linear duty cycle. This is so
  // existing NeoPixel or NeoPXL8 code brought over to NeoPXL8HDR looks the
  // same, no surprises. When reworking a project for HDR, it's best to
  // enable, animation code then doesn't need to process this. Gamma curve
  // (2.6 in this example) is set at the same time as global brightness.
  // Notice the brightness uses a 16-bit value (0-65535), not 8-bit
  // (0-255) as in NeoPixel and NeoPXL8:
  leds.setBrightness(65535 / 8, 2.6); // 1/8 max duty cycle, 2.6 gamma factor

  // The startup colors are REMOVED in this example. It's assumed at
  // this point (via non-HDR strandtest) that your code and hardware
  // are confirmed in sync, making these tests redundant.

  Serial.begin(9600);
}

// ANIMATION LOOP (similar to "regular" NeoPXL8) ---------------------------

// The loop() function is identical to the non-HDR example. Although
// NeoPXL8HDR can handle 16-bits-per-channel colors, we're using 8-bit
// here (packed into a 32-bit value) to show how existing NeoPixel or
// NeoPXL8 code can carry over directly. The library will expand these
// to 16 bits behind the scenes.
uint32_t start = 0;
#define SPEED_FACTOR 16
#define PULSE_LENGTH 256
void loop()
{
  uint32_t now = millis(); // Get time once at start of each frame

  if (start == 0) {
    start = now;
  }

  if (now - start < PULSE_LENGTH * SPEED_FACTOR) {
    uint16_t frame = (now - start) / SPEED_FACTOR;
    Serial.println(frame);
    uint8_t r = 0;
    for (int p = 0; p < NUM_LEDS; p++)
    {
      uint16_t hue = (frame + p) * PULSE_LENGTH;

      uint8_t value = sin_wave[(uint8_t)(frame + p)];

      if (frame + p < NUM_LEDS) {
        value = 0;
      } else if (frame + p > PULSE_LENGTH) {
        value = 0;
      }
      leds.setPixelColor(r * NUM_LEDS + p, leds.ColorHSV(hue, 255, value));
    }
    leds.show();
  } //else if (now - start > 20)
}

// Pixel strand colors, same code as non-HDR strandtest:
static uint8_t colors[8][3] = {
    255, 0, 0,   // Row 0: Red
    255, 160, 0, // Row 1: Orange
    255, 255, 0, // Row 2: Yellow
    0, 255, 0,   // Row 3: Green
    0, 255, 255, // Row 4: Cyan
    0, 0, 255,   // Row 5: Blue
    192, 0, 255, // Row 6: Purple
    255, 0, 255  // Row 7: Magenta
};

// This function changes very slightly from the non-HDR strandtest example.
// That code had to apply gamma correction on is own. It's not necessary
// with the HDR class once configured (see setBrightness() in setup()),
// we get that "free" now.
// uint32_t rain(uint32_t now, uint8_t row, int pixelNum) {
//   uint8_t frame = now / 4; // uint8_t rolls over for a 0-255 range
//   uint16_t b = 255;// 256 - ((frame - row * 32 + pixelNum * 256 / NUM_LEDS) & 0xFF);
//   return leds.Color((colors[row][0] * b) >> 8,
//                     (colors[row][1] * b) >> 8,
//                     (colors[row][2] * b) >> 8);
// }