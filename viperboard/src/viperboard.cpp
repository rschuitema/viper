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
    
    // Detach kernel driver if it is active
    ViperResult_t Viperboard::DetachActiveKernelDriver(libusb_device_handle* devicehandle)
    {
        ViperResult_t result = VIPER_OTHER_ERROR;
        int value = LIBUSB_ERROR_OTHER;
        
        value = libusb_kernel_driver_active(devicehandle, 0);
        if (value == 1)
        {
            value = libusb_detach_kernel_driver(devicehandle, 0);   
        }
        
        switch(value)
        {
            case 0:
            {
                result = VIPER_SUCCESS;
                break;
            }
            case LIBUSB_ERROR_NO_DEVICE:
            {
                result = VIPER_HW_NOT_FOUND;
                break;
            }
            case LIBUSB_ERROR_NOT_FOUND:
            case LIBUSB_ERROR_INVALID_PARAM:
            case LIBUSB_ERROR_NOT_SUPPORTED:
            default:
            {   
                result = VIPER_OTHER_ERROR;
                break;
            }
        }
        
        return result;
        
    }
    
    // Opens the device for specific vid and pid
    ViperResult_t Viperboard::Open(void)
    {
        ViperResult_t result;
        
        usbdevicehandle = libusb_open_device_with_vid_pid(usbcontext, VIPERBOARD_VENDOR_ID, VIPERBOARD_PRODUCT_ID);
        if (usbdevicehandle)
        {
            result = DetachActiveKernelDriver(usbdevicehandle);
        }
        else
        {
            result = VIPER_HW_NOT_FOUND;
        }
        
        return result;
    }
}
