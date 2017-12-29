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
        ViperResult_t result;
        
        usbdevicehandle = libusb_open_device_with_vid_pid(usbcontext, VIPERBOARD_VENDOR_ID, VIPERBOARD_PRODUCT_ID);
        if (usbdevicehandle)
        {
            int value = LIBUSB_ERROR_OTHER;
            value = libusb_kernel_driver_active(usbdevicehandle, 0);
            
            switch(value)
            {
                case 0:
                {
                    result = VIPER_SUCCESS;
                    break;
                }
                case 1:
                {
                    int detached = libusb_detach_kernel_driver(usbdevicehandle, 0);
                    if (detached == 0)
                    {
                        result = VIPER_SUCCESS;
                    }
                    else
                    {
                        result = VIPER_HW_NOT_FOUND;
                    }
                    break;
                }
                case LIBUSB_ERROR_NO_DEVICE:
                {
                    result = VIPER_HW_NOT_FOUND;
                    break;
                }
                default:
                {   
                    result = VIPER_OTHER_ERROR;
                    break;
                }
            }
            
        }
        else
        {
            result = VIPER_HW_NOT_FOUND;
        }
        
        return result;
    }
}
