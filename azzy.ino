#include "acc.h"
#include "leds.h"
#include "mux.h"

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  Serial.println("Setup startin");
  setupAccelerometers();
  setupLeds();
  pulseAllLeds();
  // startLedTest();

  Serial.println("Setup complete");
}

void loop1() {
  ledLoop1();
}

int delayMs = 10;
uint8_t sleepCounter = 0;
void loop() {
  if (sleepCounter == 0) {
    readAccelerometers(MuxI2CAddress::LEFT_ARM, delayMs * 20, pulseLedsLeftArm);
    readAccelerometers(MuxI2CAddress::RIGHT_ARM, delayMs * 20, pulseLedsRightArm);
    readAccelerometers(MuxI2CAddress::CENTER, delayMs * 20, pulseAllLeds);
  }

  sleepCounter++;
  sleepCounter %= 20;
  updateLeds();
  delay(delayMs);
}
