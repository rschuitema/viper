#ifndef _II2C_MASTER_H_
#define _II2C_MASTER_H_

#include <stdint.h>
#include "vipertypes.h"

namespace Viper
{
    typedef enum {
        I2C_FREQUENCY_10KHZ = 0,
        I2C_FREQUENCY_100KHZ,
        I2C_FREQUENCY_200KHZ,
        I2C_FREQUENCY_400KHZ,
        I2C_FREQUENCY_1MHZ,
        I2C_FREQUENCY_3MHZ,
        I2C_FREQUENCY_6MHZ
    } I2CFrequency_t;
    
    class II2C_Master {
        
    public:
        virtual ViperResult_t SetFrequency(I2CFrequency_t frequency)=0;
        virtual ViperResult_t ScanConnectedDevices(bool* plist, uint8_t length)=0;
        virtual ViperResult_t Write(uint8_t slave_address, uint8_t register_address, int16_t length, uint8_t* pBuffer)=0;
        virtual ViperResult_t Read(uint8_t slave_address, uint8_t register_address, int16_t length, uint8_t* pBuffer)=0;
        
    };
}

#endif
