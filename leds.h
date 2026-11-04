#pragma once

#include <Adafruit_NeoPXL8.h>


enum LedPosition {
    LEFT_ARM,
    RIGHT_ARM,
    LEFT_SIDE,
    RIGHT_SIDE
};

void ledLoop1();
void setupLeds();
void pulseLeds(enum LedPosition position);
void updateLeds();
void startLedTest();
