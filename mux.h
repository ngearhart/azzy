#ifndef __MUX_H__
#define __MUX_H__

#define PCAADDR 0x70

/// @brief The Multiplexer I2C address as plugged into the PCA9546 board
enum MuxI2CAddress {    
    /// @brief Flowey external device
    FLOWEY,

    /// @brief The right arm; contains the right arm IMU
    RIGHT_ARM,
    
    /// @brief The left arm; contains the left arm IMU and the rotary encoder
    LEFT_ARM,
    
    /// @brief The center back; contains the whole body IMU and the haptic motor 
    CENTER,
};

void muxSelect(enum MuxI2CAddress address);

#endif
