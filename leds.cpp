#include "leds.h"
#include "constants.h"
#include <Adafruit_NeoPXL8.h>

// Smaller speed factor = faster pulses
#define SPEED_FACTOR 4
#define PULSE_LENGTH 256
#define NUM_LEDS_PER_STRIP 96

int8_t LED_PINS[8] = { 
    LEFT_ARM_LED_PIN, RIGHT_ARM_LED_PIN, LEFT_SIDE_LED_PIN, RIGHT_SIDE_LED_PIN, 20, 21, 22, 23
};

// Up to 65535
// 65535 / 8 => roughly 1 watt per meter
// 65535 / 4 => roughly 2 watts per meter
// 65535 / 2 => roughly 4 watts per meter
// So for the whole cosplay - 4 meters * max brightness power = 16 watts
// 16 watts at 5V is 3.2A - need at least 3.2A peak current, probably 5A to be safe
#define LED_BRIGHTNESS 65535 / 2

// Here's the global constructor as explained near the start:
Adafruit_NeoPXL8HDR leds(NUM_LEDS_PER_STRIP, LED_PINS, NEO_RGB + NEO_KHZ800);

uint32_t timeStart[4] = { 0, 0, 0, 0 };
uint32_t timeEnd[4] = { 0, 0, 0, 0 };

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
    leds.setBrightness(LED_BRIGHTNESS, 2.6); // 1/8 max duty cycle, 2.6 gamma factor
}

void pulseLeds(enum LedPosition position)
{
    uint32_t now = millis(); // Get time once at start of each frame
    Serial.print("Pulsing LEDs - pos=");
    Serial.print(position);
    if (now > timeEnd[position])
    {
        Serial.print(" with 0 start");
        // If we are past time end, then reset everything
        timeStart[position] = now;
    }
    // Otherwise, just reset time end
    timeEnd[position] = now + PULSE_LENGTH * SPEED_FACTOR;
    Serial.println("");
}

void updateLeds(enum LedPosition position) {
    uint32_t now = millis(); // Use millis to ensure if there is a delay, the animation runs just as fast
    uint32_t timeStartForThisPos = timeStart[position];
    uint32_t timeEndForThisPos = timeEnd[position];
    if (now - timeStartForThisPos < timeEndForThisPos)
    {
        uint32_t frame = (now - timeStartForThisPos) / SPEED_FACTOR;
        uint32_t endFrame = (timeEndForThisPos - timeStartForThisPos) / SPEED_FACTOR;
        for (int p = 0; p < NUM_LEDS_PER_STRIP; p++)
        {
            uint16_t hue = (frame + p) * PULSE_LENGTH;

            uint8_t value = 255;

            if (frame + p < NUM_LEDS_PER_STRIP)
            {
                value = 0; // Before the pulse
            }
            else if (frame + p > endFrame)
            {
                value = 0; // After the pulse
            }
            leds.setPixelColor(position * NUM_LEDS_PER_STRIP + p, leds.ColorHSV(hue, 255, value));
        }
        leds.show();
    }
}

/// @brief Run every update loop to give all LEDs the correct state
void updateLeds()
{
    for (int position = 0; position < 4; position++) {
        updateLeds((LedPosition) position);
    }
}

/// @brief Run LED pulse constantly
void startLedTest() {
    pulseLeds(LedPosition::LEFT_ARM);
    pulseLeds(LedPosition::RIGHT_ARM);
    timeEnd[0] = UINT32_MAX;
    timeEnd[1] = UINT32_MAX;
}
