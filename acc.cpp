
#include "acc.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

sensors_event_t prevEvent;
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setupAccelerometers() {
  Serial.println("LIS3DH test!");

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
  Serial.print("Performance mode set to: ");
  switch (lis.getPerformanceMode()) {
    case LIS3DH_MODE_NORMAL: Serial.println("Normal 10bit"); break;
    case LIS3DH_MODE_LOW_POWER: Serial.println("Low Power 8bit"); break;
    case LIS3DH_MODE_HIGH_RESOLUTION: Serial.println("High Resolution 12bit"); break;
  }

  lis.setDataRate(LIS3DH_DATARATE_10_HZ);
  Serial.print("Data rate set to: ");
  switch (lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("1.6 Khz Low Power"); break;
  }
}

void readAccelerometers(int delayMs, void (*callback)(void)) {
    sensors_event_t event;
    lis.getEvent(&event);

    int dx = 0;
    int dy = 0;
    int dz = 0;

    if (prevEvent.acceleration.x != 0) {
        dx = floor(abs(prevEvent.acceleration.x - event.acceleration.x) / delayMs * 1000);
        dy = floor(abs(prevEvent.acceleration.y - event.acceleration.y) / delayMs * 1000);
        dz = floor(abs(prevEvent.acceleration.z - event.acceleration.z) / delayMs * 1000);
    }
    prevEvent = event;

    if (dx > ACC_TRIGGER_THRESHOLD || dy > ACC_TRIGGER_THRESHOLD || dz > ACC_TRIGGER_THRESHOLD) {
        callback();
    }
}