#ifndef _GPIO_PORTA_H_
#define _GPIO_PORTA_H_

#include <stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <libusb.h>
#pragma GCC diagnostic pop

#include "vipertypes.h"
#include "igpio_porta.h"

namespace Viper
{
    
    class GpioPortAViperboard : public IGPIO_PortA
    {
        
        public:
            GpioPortAViperboard(libusb_device_handle* usbdevicehandle);
            virtual ~GpioPortAViperboard(void);
            ViperResult_t SetPWMMode(uint8_t bit, uint8_t level, uint8_t clock);

        private:
            libusb_device_handle* usbdevicehandle = nullptr;
    };
}

#endif