#include "leds.h"
#include <Adafruit_NeoPXL8.h>

// Smaller speed factor = faster pulses
#define SPEED_FACTOR 4
#define PULSE_LENGTH 256
#define NUM_LEDS 96

// Feather RP2040 SCORPIO pinout
int8_t pins[8] = {16, 17, 18, 19, 20, 21, 22, 23};

// Here's the global constructor as explained near the start:
Adafruit_NeoPXL8HDR leds(NUM_LEDS, pins, NEO_RGB + NEO_KHZ800);

// See generate_lookup.py - normalized sqrt(sin(x)) for x in [0, 1]
uint8_t const sin_wave[256] PROGMEM =
    {0, 28, 40, 49, 56, 63, 69, 74, 79, 84, 89, 93, 97, 101, 105, 109, 112, 116, 119, 122, 125, 128, 131, 134, 137, 140, 143, 145, 148, 150, 153, 155, 158, 160, 162, 164, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 186, 188, 190, 192, 193, 195, 197, 198, 200, 201, 203, 204, 206, 207, 209, 210, 212, 213, 214, 216, 217, 218, 219, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245, 245, 246, 247, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 252, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 252, 252, 252, 252, 251, 251, 250, 250, 250, 249, 249, 248, 248, 247, 247, 246, 245, 245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 219, 218, 217, 216, 214, 213, 212, 210, 209, 207, 206, 204, 203, 201, 200, 198, 197, 195, 193, 192, 190, 188, 186, 185, 183, 181, 179, 177, 175, 173, 171, 169, 167, 164, 162, 160, 158, 155, 153, 150, 148, 145, 143, 140, 137, 134, 131, 128, 125, 122, 119, 116, 112, 109, 105, 101, 97, 93, 89, 84, 79, 74, 69, 63, 56, 49, 40, 28, 0};

uint32_t timeStart = 0;
uint32_t timeEnd = 0;

void ledLoop1()
{
    leds.refresh();
}

void setupLeds()
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
}

void pulseLeds()
{
    uint32_t now = millis(); // Get time once at start of each frame
    Serial.print("Pulsing LEDs");
    if (now > timeEnd)
    {
        Serial.print(" with 0 start");
        // If we are past time end, then reset everything
        timeStart = now;
    }
    // Otherwise, just reset time end
    timeEnd = now + PULSE_LENGTH * SPEED_FACTOR;
    Serial.println("");
}

void updateLeds()
{
    uint32_t now = millis(); // Get time once at start of each frame

    if (now - timeStart < timeEnd)
    {
        uint32_t frame = (now - timeStart) / SPEED_FACTOR;
        uint32_t endFrame = (timeEnd - timeStart) / SPEED_FACTOR;
        uint8_t r = 0;
        for (int p = 0; p < NUM_LEDS; p++)
        {
            uint16_t hue = (frame + p) * PULSE_LENGTH;

            uint8_t value = 255; // = sin_wave[(uint8_t)(frame + p)];

            if (frame + p < NUM_LEDS)
            {
                value = 0; // Before the pulse
            }
            else if (frame + p > endFrame)
            {
                value = 0; // After the pulse
            }
            leds.setPixelColor(r * NUM_LEDS + p, leds.ColorHSV(hue, 255, value));
        }
        leds.show();
    }
}
