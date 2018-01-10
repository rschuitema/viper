
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
        int bytes_transferred = 0;
	    uint8_t buffer[5];
	    
        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xDD, 0x0000, 0x0000, buffer, 5, 1000);
        
	    if (bytes_transferred == 5) 
	    {
            *value = buffer[3];
            *value <<= 8u;
            *value |= buffer[4];
	        
	        return VIPER_SUCCESS;
	    } 
	    
        return VIPER_TRANSACTION_FAILURE;
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
        
	    if (bytes_transferred == 5) 
	    {
            *value = buffer[1];
            *value <<= 8u;
            *value |= buffer[2];
	        
	        return VIPER_SUCCESS;
	    } 
	    
        return VIPER_TRANSACTION_FAILURE;
    }
    

    ViperResult_t GpioPortBViperboard::SetBitDirection(uint8_t bit, bool direction)
    {
        int bytes_transferred = 0;
	    uint8_t buffer[7];
	    
	    if (bit >15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
	    
	    buffer[0] = 0x02;
	    buffer[1] = 0x00;
	    buffer[2] = 0x00;
	    buffer[3] = 0x00;
	    buffer[4] = 0x00;
	    buffer[5] = bit;
	    buffer[6] = direction ? 0x01 : 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xDD, 0x0000, 0x0000, buffer, 7, 1000);
        
	    if (bytes_transferred != 7) 
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    }
        
        return VIPER_SUCCESS;
    }
    
    ViperResult_t GpioPortBViperboard::GetBitDirection(uint8_t bit, bool* direction)
    {
        int bytes_transferred = 0;
	    uint8_t buffer[5];
	    uint16_t value = 0u;
	    uint16_t mask = 1u;
	    
	    if ((bit > 15u) || (!direction))
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
	    
        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xDD, 0x0000, 0x0000, buffer, 5, 1000);
        
	    if (bytes_transferred == 5) 
	    {
            value = buffer[3];
            value <<= 8u;
            value |= buffer[4];
            
            mask <<= bit;
            *direction = value & mask ? true: false;
        
            return VIPER_SUCCESS;
        }
        
        return VIPER_TRANSACTION_FAILURE;
    }
    
    ViperResult_t GpioPortBViperboard::WriteBit(uint8_t bit, bool direction)
    {
        int bytes_transferred = 0;
	    uint8_t buffer[7];
	    
	    if (bit > 15)
	    {
	        return VIPER_INVALID_PARAMETER;
	    }
	    
	    
	    buffer[0] = 0x03;
	    buffer[1] = 0x00;
	    buffer[2] = 0x00;
	    buffer[3] = 0x00;
	    buffer[4] = 0x00;
	    buffer[5] = bit;
	    buffer[6] = direction ? 0x01 : 0x00;
	    
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0x40, 0xDD, 0x0000, 0x0000, buffer, 7, 1000);
	    
	    if (bytes_transferred != 7)
	    {
	        return VIPER_TRANSACTION_FAILURE;
	    }
	    
	    return VIPER_SUCCESS;
      }
    
    ViperResult_t GpioPortBViperboard::ReadBit(uint8_t bit, bool* bit_value)
    {
        int bytes_transferred = 0;
	    uint8_t buffer[5];
	    uint16_t value = 0u;
	    uint16_t mask = 1u;
        
	    bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xDD, 0x0000, 0x0000, buffer, 5, 1000);
	    
	    if (bytes_transferred == 5)
	    {
            value = buffer[1];
            value <<= 8u;
            value |= buffer[2];
            
            mask <<= bit;
            *bit_value = value & mask ? true: false;
	        
            return VIPER_SUCCESS;
        }
        
        return VIPER_TRANSACTION_FAILURE;
    }
    

}
