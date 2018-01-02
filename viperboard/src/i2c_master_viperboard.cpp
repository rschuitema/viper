
#include "i2c_master_viperboard.h"

namespace Viper
{
    I2CMasterViperboard::I2CMasterViperboard()
    {
        
    }
    
    I2CMasterViperboard::~I2CMasterViperboard()
    {
        
    }

    ViperResult_t I2CMasterViperboard::SetFrequency(I2CFrequency_t frequency)
    {
        return VIPER_OTHER_ERROR;
    }

    ViperResult_t I2CMasterViperboard::ScanConnectedDevices(uint8_t* plist, int16_t length)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t I2CMasterViperboard::Write(uint8_t slave_address, uint8_t register_address, int16_t length, uint8_t* pBuffer)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t I2CMasterViperboard::Read(uint8_t slave_address, uint8_t register_address, int16_t length, uint8_t* pBuffer)
    {
        return VIPER_OTHER_ERROR;
    }

}
