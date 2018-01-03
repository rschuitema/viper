
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
        return VIPER_SUCCESS;
    }
    
    ViperResult_t GpioPortBViperboard::GetPortDirection(uint16_t* value)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t GpioPortBViperboard::WritePort(uint16_t value, uint16_t mask)
    {
        return VIPER_OTHER_ERROR;
    }
    
    ViperResult_t GpioPortBViperboard::ReadPort(uint16_t* value)
    {
        return VIPER_OTHER_ERROR;
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
