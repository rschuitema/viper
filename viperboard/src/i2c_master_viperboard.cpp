
#include "i2c_master_viperboard.h"

namespace Viper
{
    I2CMasterViperboard::I2CMasterViperboard(libusb_device_handle* devicehandle)
    {
        usbdevicehandle = devicehandle;
    }
    
    I2CMasterViperboard::~I2CMasterViperboard()
    {
        
    }

    ViperResult_t I2CMasterViperboard::SetFrequency(I2CFrequency_t frequency)
    {
        int bytes_transferred = 0;
        uint8_t nano_frequency = 4;
        
        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xE1, 0x0000, 0x0000, &nano_frequency, 0x01, 1000u);
        if (bytes_transferred != 1)
        {
            return VIPER_TRANSACTION_FAILURE;
        }
        
        return VIPER_SUCCESS;
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
