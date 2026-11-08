#ifndef __HAPTIC_H__
#define __HAPTIC_H__

#include <Wire.h>
#include "Adafruit_DRV2605.h"
#include "mux.h"

// https://learn.adafruit.com/assets/72593
#define HAPTIC_MEDIUM_CLICK_100 21
#define HAPTIC_MEDIUM_CLICK_60 23
#define HAPTIC_SHORT_DOUBLE_CLICK_STRONG 27

void setupHaptic();
void playHapticEffect(uint8_t effect);

#endif