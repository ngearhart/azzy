/*
 * This example shows how to read from a seesaw encoder module.
 * The available encoder API is:
 *      int32_t getEncoderPosition();
        int32_t getEncoderDelta();
        void enableEncoderInterrupt();
        void disableEncoderInterrupt();
 */
#include "rotary.h"
#include "mux.h"
#include "haptic.h"
#include "leds.h"

Adafruit_seesaw ss;
int32_t encoder_position;
int32_t modifier;

void setupRotary() {
  Serial.println("Looking for rotary");
  muxSelect(MuxI2CAddress::LEFT_ARM);

  if (! ss.begin(SEESAW_ADDR)) {
    Serial.println("Couldn't find rotary on default address");
    while(1) delay(10);
  }
  Serial.println("rotary started");
  uint32_t version = ((ss.getVersion() >> 16) & 0xFFFF);
  if (version  != 5740){
    Serial.print("Wrong firmware loaded? ");
    Serial.println(version);
    while(1) delay(10);
  }
  Serial.println("Found rotary 5740");

  ss.pinMode(SS_SWITCH_UP, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_DOWN, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_LEFT, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_RIGHT, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_SELECT, INPUT_PULLUP);

  // get starting position
  encoder_position = ss.getEncoderPosition();

//   Serial.println("Turning on interrupts");
//   ss.enableEncoderInterrupt();
//   ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH_UP, 1);

}

void readRotary() {
  muxSelect(MuxI2CAddress::LEFT_ARM);
  if (! ss.digitalRead(SS_SWITCH_UP)) {
    Serial.println("UP pressed!");
  }
  if (! ss.digitalRead(SS_SWITCH_DOWN)) {
    Serial.println("DOWN pressed!");
  }
  if (! ss.digitalRead(SS_SWITCH_SELECT)) {
    Serial.println("SELECT pressed!");
  }
  if (! ss.digitalRead(SS_SWITCH_LEFT)) {
    Serial.println("LEFT pressed!");
    setLedBrightness(UINT8_MAX);
    // playHapticEffect(HAPTIC_MEDIUM_CLICK_60);
  }
  if (! ss.digitalRead(SS_SWITCH_RIGHT)) {
    Serial.println("RIGHT pressed!");
    setLedBrightness(0);
    // playHapticEffect(HAPTIC_MEDIUM_CLICK_60);
  }

  int32_t new_position = ss.getEncoderPosition();
  // did we move around?
  if (encoder_position != new_position) {
    int8_t delta = new_position - encoder_position;
    encoder_position = new_position;      // and save for next round

    if (modifyLedBrightness(delta * 4)) {
      playHapticEffect(HAPTIC_MEDIUM_CLICK_60);
    } else {
      // Too bright or too dim
      playHapticEffect(HAPTIC_SHORT_DOUBLE_CLICK_STRONG);
    }
  }
}
