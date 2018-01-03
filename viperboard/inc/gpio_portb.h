#ifndef _GPIO_PORTB_H_
#define _GPIO_PORTB_H_

#include <stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <libusb.h>
#pragma GCC diagnostic pop

#include "vipertypes.h"
#include "igpio_portb.h"

namespace Viper
{
    
    class GpioPortBViperboard : public IGPIO_PortB
    {
        
        public:
            GpioPortBViperboard(libusb_device_handle* usbdevicehandle);
            virtual ~GpioPortBViperboard(void);

            virtual ViperResult_t SetPortDirection(uint16_t value, uint16_t mask);
            virtual ViperResult_t GetPortDirection(uint16_t* value);
            virtual ViperResult_t WritePort(uint16_t value, uint16_t mask);
            virtual ViperResult_t ReadPort(uint16_t* value);
            
            virtual ViperResult_t SetBitDirection(uint8_t bit, bool direction);
            virtual ViperResult_t GetBitDirection(uint8_t bit, bool* direction);
            virtual ViperResult_t WriteBit(uint8_t bit, bool direction);
            virtual ViperResult_t ReadBit(uint8_t bit, bool* direction);

       private:
            libusb_device_handle* usbdevicehandle = nullptr;
    };
}

#endif
