
#include "gpio_porta.h"

namespace Viper
{
    
    GpioPortAViperboard::GpioPortAViperboard(libusb_device_handle* devicehandle)
    {
        usbdevicehandle = devicehandle;
    }
    
    GpioPortAViperboard::~GpioPortAViperboard()
    {
        
    }

	ViperResult_t GpioPortAViperboard::SetPWMMode(uint8_t bit, uint8_t level, uint8_t clock)
	{
        int bytes_transferred = 0;
	    uint8_t buffer[11];

	    if (bit >15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
    
	    buffer[0] = 0x02;
	    buffer[1] = clock;
	    buffer[2] = bit;
	    buffer[3] = 0x00;
	    buffer[4] = 0x00;
	    buffer[5] = 0x00;
	    buffer[6] = level;
	    buffer[7] = 0x00;
	    buffer[8] = 0x00;
	    buffer[9] = 0x00;
	    buffer[10] = 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 
	    
        return VIPER_SUCCESS;
	}

	ViperResult_t GpioPortAViperboard::SetContinuousMode(uint8_t bit, uint8_t clock, uint8_t t1, uint8_t t2)
	{
        int bytes_transferred = 0;
	    uint8_t buffer[11];

	    if (bit >15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
    
	    buffer[0] = 0x00;
	    buffer[1] = clock;
	    buffer[2] = bit;
	    buffer[3] = t1;
	    buffer[4] = t2;
	    buffer[5] = 0x00;
	    buffer[6] = 0x00;
	    buffer[7] = 0x00;
	    buffer[8] = 0x00;
	    buffer[9] = 0x00;
	    buffer[10] = 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 

        return VIPER_SUCCESS;
	}

	ViperResult_t GpioPortAViperboard::SetPulseMode(uint8_t bit, uint8_t clock, uint8_t t1, uint8_t t2, bool invert)
	{
        int bytes_transferred = 0;
	    uint8_t buffer[11];

	    if (bit >15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
    
	    buffer[0] = 0x01;
	    buffer[1] = clock;
	    buffer[2] = bit;
	    buffer[3] = t1;
	    buffer[4] = t2;
	    buffer[5] = invert ? 0x01 : 0x00;
	    buffer[6] = 0x00;
	    buffer[7] = 0x00;
	    buffer[8] = 0x00;
	    buffer[9] = 0x00;
	    buffer[10] = 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 

        return VIPER_SUCCESS;
	}

	ViperResult_t GpioPortAViperboard::SetDigitalOutputMode(uint8_t bit, bool value)
	{
        int bytes_transferred = 0;
	    uint8_t buffer[11];

	    if (bit >15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
    
	    buffer[0] = 0x03;
	    buffer[1] = 0x00;
	    buffer[2] = bit;
	    buffer[3] = 0x00;
	    buffer[4] = 0x00;
	    buffer[5] = 0x00;
	    buffer[6] = 0x00;
	    buffer[7] = value ? 0x01 : 0x00;
	    buffer[8] = 0x00;
	    buffer[9] = 0x00;
	    buffer[10] = 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 

        return VIPER_SUCCESS;
	}

	ViperResult_t GpioPortAViperboard::SetDigitalInputMode(uint8_t bit, uint8_t clock)
	{
        int bytes_transferred = 0;
	    uint8_t buffer[11];

	    if (bit >15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
    
	    buffer[0] = 0x04;
	    buffer[1] = clock;
	    buffer[2] = bit;
	    buffer[3] = 0x00;
	    buffer[4] = 0x00;
	    buffer[5] = 0x00;
	    buffer[6] = 0x00;
	    buffer[7] = 0x00;
	    buffer[8] = 0x00;
	    buffer[9] = 0x00;
	    buffer[10] = 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 

            return VIPER_SUCCESS;
	}

	ViperResult_t GpioPortAViperboard::GetDigitalInput(uint8_t bit, bool* value)
	{
            int bytes_transferred = 0;
	    uint8_t buffer[11];

	    if (bit >15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
    
	    buffer[0] = 0x06;
	    buffer[1] = 0x00;
	    buffer[2] = bit;
	    buffer[3] = 0x00;
	    buffer[4] = 0x00;
	    buffer[5] = 0x00;
	    buffer[6] = 0x00;
	    buffer[7] = 0x00;
	    buffer[8] = 0x00;
	    buffer[9] = 0x00;
	    buffer[10] = 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 

	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 

            *value = (buffer[9] == 0x01) ? true: false;
            
            return VIPER_SUCCESS;
	}

	ViperResult_t GpioPortAViperboard::SetInterruptInputMode(uint8_t bit, bool risefall, uint8_t clock)
	{
            int bytes_transferred = 0;
	    uint8_t buffer[11];

	    buffer[0] = 0x05;
	    buffer[1] = clock;
	    buffer[2] = bit;
	    buffer[3] = 0x00;
	    buffer[4] = 0x00;
	    buffer[5] = 0x00;
	    buffer[6] = 0x00;
	    buffer[7] = 0x00;
	    buffer[8] = risefall ? 0x01 : 0x00;
	    buffer[9] = 0x00;
	    buffer[10] = 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xED, 0x0000, 0x0000, buffer, 11, 1000);

	    if (bytes_transferred !=11) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 

            return VIPER_SUCCESS;
        }
}

	
