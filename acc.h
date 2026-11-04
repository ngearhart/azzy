#ifndef __ACC_H__
#define __ACC_H__

#include "mux.h"

void setupAccelerometers();
void readAccelerometers(enum MuxI2CAddress address, int delayMs, void (*callback)(void));

#endif
