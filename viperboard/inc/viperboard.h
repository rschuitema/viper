#ifndef _VIPERBOARD_H_
#define _VIPERBOARD_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <libusb.h>
#pragma GCC diagnostic pop

#include "vipertypes.h"

namespace Viper
{
    class II2C_Master;
    class II2C_Slave;

    class Viperboard {
        
    public:
        Viperboard(void);
        virtual ~Viperboard(void);

        ViperResult_t Open(void);
        ViperResult_t Close(void);
        
        void SetI2CMaster(II2C_Master* master);
        void SetI2CSlave(II2C_Slave* slave);
        
        ViperResult_t Revision(void);
        
    private:
        libusb_context* usbcontext = nullptr;
        libusb_device* usbdevice = nullptr;
        libusb_device_handle* usbdevicehandle = nullptr;

        II2C_Master* i2c_master = nullptr;
        II2C_Slave*  i2c_slave = nullptr;
        
    };
}

#endif
