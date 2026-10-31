#pragma once

#define ACC_TRIGGER_THRESHOLD 20

void setupAccelerometers();
void readAccelerometers(int delayMs, void (*callback)(void));
