#ifndef __LEDS_H__
#define __LEDS_H__

#include <Adafruit_NeoPXL8.h>


enum LedPosition {
    LED_LEFT_ARM,
    LED_RIGHT_ARM,
    LED_LEFT_SIDE,
    LED_RIGHT_SIDE
};

void ledLoop1();
void setupLeds();
void setLedBrightness(uint8_t brightness);
bool modifyLedBrightness(int16_t change);
void pulseLeds(enum LedPosition position);
void updateLeds();
void startLedTest();

void pulseLedsLeftArm();
void pulseLedsRightArm();
void pulseLedsLeftSide();
void pulseLedsRightSide();
void pulseAllLeds();

#endif
