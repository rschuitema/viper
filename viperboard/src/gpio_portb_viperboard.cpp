
#include "gpio_portb.h"

namespace Viper
{
    
    GpioPortBViperboard::GpioPortBViperboard(libusb_device_handle* devicehandle)
    {
        usbdevicehandle = devicehandle;
    }
    
    GpioPortBViperboard::~GpioPortBViperboard()
    {
        
    }

    ViperResult_t GpioPortBViperboard::SetPortDirection(uint16_t value, uint16_t mask)
    {
        int bytes_transferred = 0;
	    uint8_t buffer[5];
	    
	    buffer[0] = 0x00;
	    buffer[1] = ((value & 0xFF00) >> 8);
	    buffer[2] = ((value & 0x00FF));
	    buffer[3] = ((mask & 0xFF00) >> 8);
	    buffer[4] = ((mask & 0x00FF));
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xDD, 0x0000, 0x0000, buffer, 5, 1000);
	    if (bytes_transferred !=5) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 
	    
        return VIPER_SUCCESS;
    }
    
    ViperResult_t GpioPortBViperboard::GetPortDirection(uint16_t* value)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t GpioPortBViperboard::WritePort(uint16_t value, uint16_t mask)
    {
        int bytes_transferred = 0;
	    uint8_t buffer[5];
	    
	    buffer[0] = 0x01;
	    buffer[1] = ((value & 0xFF00) >> 8);
	    buffer[2] = ((value & 0x00FF));
	    buffer[3] = ((mask & 0xFF00) >> 8);
	    buffer[4] = ((mask & 0x00FF));
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xDD, 0x0000, 0x0000, buffer, 5, 1000);
	    if (bytes_transferred !=5) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    } 
	    
        return VIPER_SUCCESS;
    }
    
    ViperResult_t GpioPortBViperboard::ReadPort(uint16_t* value)
    {
        int bytes_transferred = 0;
	    uint8_t buffer[5] = {0u};
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xDD, 0x0000, 0x0000, buffer, 5, 1000);
	    
        return VIPER_SUCCESS;
    }
    

    ViperResult_t GpioPortBViperboard::SetBitDirection(uint8_t bit, bool direction)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t GpioPortBViperboard::GetBitDirection(uint8_t bit, bool* direction)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t GpioPortBViperboard::WriteBit(uint8_t bit, bool direction)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t GpioPortBViperboard::ReadBit(uint8_t bit, bool* direction)
    {
        return VIPER_OTHER_ERROR;
    }
    

}
