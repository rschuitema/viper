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

            ViperResult_t SetContinuousMode(uint8_t bit, uint8_t clock, uint8_t t1, uint8_t t2);
            ViperResult_t SetPulseMode(uint8_t bit, uint8_t clock, uint8_t t1, uint8_t t2, bool invert);
            ViperResult_t SetPWMMode(uint8_t bit, uint8_t level, uint8_t clock);
            ViperResult_t SetDigitalOutputMode(uint8_t bit, bool value);
            ViperResult_t SetDigitalInputMode(uint8_t bit, uint8_t clock);
            ViperResult_t GetDigitalInput(uint8_t bit, bool* value);

        private:
            libusb_device_handle* usbdevicehandle = nullptr;
    };
}

#endif
