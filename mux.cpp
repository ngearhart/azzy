#include "mux.h"
#include "Wire.h"

enum MuxI2CAddress currentAddress = MuxI2CAddress::FLOWEY;
enum MuxI2CAddress prevAddress = MuxI2CAddress::FLOWEY;

/// @brief Change the I2C Multiplexer to select a specific port as described in MuxI2CAddress enum
/// @param  address The address to switch to
void muxSelect(enum MuxI2CAddress address) {
  if (address == currentAddress) return;
  if (address > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << address);
  Wire.endTransmission();
  prevAddress = currentAddress;
  currentAddress = address;
}
