#include "haptic.h"

Adafruit_DRV2605 drv;

void setupHaptic()
{
    muxSelect(MuxI2CAddress::CENTER);
    bool started = false;
    for (uint8_t i = 0; i < 10; i++)
    {
        if (drv.begin())
        {
            started = true;
            break;
        }
        Serial.println("Could not find Haptic DRV2605 (retrying)");
        delay(200);
    }
    if (!started)
    {
        while (1)
            delay(10);
    }

    drv.selectLibrary(1);

    // I2C trigger by sending 'go' command
    // default, internal trigger when sending GO command
    drv.setMode(DRV2605_MODE_INTTRIG);
}

void playHapticEffect(uint8_t effect)
{
    muxSelect(MuxI2CAddress::CENTER);
    // set the effect to play
    drv.setWaveform(0, effect); // play effect
    drv.setWaveform(1, 0);      // end waveform

    // play the effect!
    drv.go();
}
