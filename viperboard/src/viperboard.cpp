#include <stdexcept>
#include "vipertypes.h"
#include "viperboard.h"

namespace Viper
{
    const uint16_t VIPERBOARD_PRODUCT_ID = 0x1005;
    const uint16_t VIPERBOARD_VENDOR_ID  = 0x2058;
    
    static ViperResult_t UsbResult2ViperResult(int ubsresult);
    
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
        int result;
        
        usbdevicehandle = libusb_open_device_with_vid_pid(usbcontext, VIPERBOARD_VENDOR_ID, VIPERBOARD_PRODUCT_ID);
        if (usbdevicehandle)
        {
            result = libusb_kernel_driver_active(usbdevicehandle, 0);
            if (result == 1)
            {
                result = libusb_detach_kernel_driver(usbdevicehandle, 0);   
            }
            
            if (LIBUSB_SUCCESS == result)
            {
                result = libusb_set_configuration(usbdevicehandle, 1);
            }
        }
        else
        {
            result = LIBUSB_ERROR_NO_DEVICE;
        }
        
        return UsbResult2ViperResult(result);
    }
    
    ViperResult_t UsbResult2ViperResult(int usbresult)
    {
        ViperResult_t result = VIPER_OTHER_ERROR;
        
        switch(usbresult)
        {
            case LIBUSB_SUCCESS:
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
    
}
