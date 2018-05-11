
#include "i2c_master_viperboard.h"

namespace Viper
{
    uint8_t Frequency2ViperFrequency(I2CFrequency_t frequency);
    
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
        uint8_t viper_frequency = Frequency2ViperFrequency(frequency);
        
        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xE1, 0x0000, 0x0000, &viper_frequency, 0x01, 1000u);
        if (bytes_transferred != 1)
        {
            return VIPER_TRANSACTION_FAILURE;
        }
        
        return VIPER_SUCCESS;
    }

    ViperResult_t I2CMasterViperboard::ScanConnectedDevices(bool* plist, uint8_t length)
    {
        int bytes_transferred = 0;
        uint8_t buffer[12];


        buffer[0] = 0x00;
        buffer[1] = 0x00;
        buffer[2] = 0x00;
        buffer[3] = 0x00;
        buffer[4] = 0x00;
        buffer[5] = 0x00;
        buffer[6] = 0x06;
        buffer[7] = 0x00;
        buffer[8] = 0x00;
        buffer[9] = 0x00;
        buffer[10] = 0x00;
        buffer[11] = 0xff;

        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xE2, 0x0000, 0x0000, buffer, 7, 1000u);

        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xE9, 0x0000, 0x0000, buffer, 12, 1000u);
        plist[0] = true;

        return VIPER_SUCCESS;
    }
    
    ViperResult_t I2CMasterViperboard::Write(uint8_t slave_address, uint8_t register_address, int16_t length, uint8_t* pBuffer)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t I2CMasterViperboard::Read(uint8_t slave_address, uint8_t register_address, int16_t length, uint8_t* pBuffer)
    {
        return VIPER_OTHER_ERROR;
    }

    uint8_t Frequency2ViperFrequency(I2CFrequency_t frequency)
    {
        uint8_t viper_frequency = 6u;
        
        switch(frequency)
        {
            case I2C_FREQUENCY_10KHZ:
            {
                viper_frequency = 7u;
                break;
            }
            case I2C_FREQUENCY_100KHZ:
            {
                viper_frequency = 6u;
                break;
            }
            case I2C_FREQUENCY_200KHZ:
            {
                viper_frequency = 5u;
                break;
            }
            case I2C_FREQUENCY_400KHZ:
            {
                viper_frequency = 4u;
                break;
            }
            case I2C_FREQUENCY_1MHZ:
            {
                viper_frequency = 3u;
                break;
            }
            case I2C_FREQUENCY_3MHZ:
            {
                viper_frequency = 2u;
                break;
            }
            case I2C_FREQUENCY_6MHZ:
            {
                viper_frequency = 1u;
                break;
            }
            default:
            {
                viper_frequency = 6u;
                break;
            }
        }
        
        return viper_frequency;
    }
}
