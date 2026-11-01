#include "acc.h"
#include "leds.h"

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  setupAccelerometers();
  setupLeds();
  pulseLeds();
}

void loop1() {
  ledLoop1();
}

int delayMs = 10;
uint8_t sleepCounter = 0;
void loop() {
  if (sleepCounter == 0) {
    readAccelerometers(delayMs * 20, pulseLeds);
  }

  sleepCounter++;
  sleepCounter %= 20;
  updateLeds();
  delay(delayMs);
}
