
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

	ViperResult_t GpioPortAViperboard::SetPWMMode(uint8_t bit, uint16_t level, uint16_t clock)
	{
		return VIPER_OTHER_ERROR;
	}
}

