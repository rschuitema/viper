
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

		return VIPER_SUCCESS;
	}
}
