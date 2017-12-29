#include <stdexcept>
#include "vipertypes.h"
#include "viperboard.h"

namespace Viper
{
    const uint16_t VIPERBOARD_PRODUCT_ID = 0x1005;
    const uint16_t VIPERBOARD_VENDOR_ID  = 0x2058;
    
    Viperboard::Viperboard()
    {
        int result = LIBUSB_SUCCESS;
        
        if (nullptr == usbcontext)
        {
            result = libusb_init(&usbcontext);
        }

        if (LIBUSB_SUCCESS != result)
        {
            throw std::runtime_error("failed to initialize libusb");
        }
        else if (nullptr == usbcontext)
        {
            throw std::runtime_error("libusb return a null pointer");
        }

    }
    
    Viperboard::~Viperboard()
    {
        try
        {
            if (nullptr != usbcontext)
            {
                libusb_exit(usbcontext);
            }
        }
        catch(...)
        {
            // never throw from a destructor
        }
    }
    
    // Opens the device for specific vid and pid
    ViperResult_t Viperboard::Open(void)
    {
        usbdevicehandle = libusb_open_device_with_vid_pid(usbcontext, VIPERBOARD_VENDOR_ID, VIPERBOARD_PRODUCT_ID);
        if (usbdevicehandle)
        {
            return VIPER_SUCCESS;
        }
        else
        {
            return VIPER_HW_NOT_FOUND;
        }
    }
}
