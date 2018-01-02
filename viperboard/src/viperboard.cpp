#include <stdexcept>
#include "vipertypes.h"
#include "viperboard.h"
#include "i2c_master_viperboard.h"

namespace Viper
{
    const uint16_t VIPERBOARD_PRODUCT_ID = 0x1005;
    const uint16_t VIPERBOARD_VENDOR_ID  = 0x2058;
    
    const int BUFFER_SIZE = 512;
    
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
        unsigned char buffer[BUFFER_SIZE];
        int transferred = 0;
        
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
            
            if (LIBUSB_SUCCESS == result)
            {
                result = libusb_claim_interface(usbdevicehandle, 0);
            }
            
            
            for (int i = 0; (LIBUSB_SUCCESS == result) && (i < 2); i++)
            {
                result = libusb_bulk_transfer(usbdevicehandle, 0x86, buffer, BUFFER_SIZE, &transferred, 10);
            }
        }
        else
        {
            result = LIBUSB_ERROR_NO_DEVICE;
        }
        
        return UsbResult2ViperResult(result);
    }
    
    ViperResult_t Viperboard::Close(void)
    {
        int result;
        
        result = libusb_release_interface(usbdevicehandle, 0);
    
        if ((LIBUSB_SUCCESS == result) && (usbdevicehandle))
        {
            libusb_close(usbdevicehandle);
        }
            
        return UsbResult2ViperResult(result);
    }
    
    uint16_t Viperboard::Revision(void)
    {
        int bytes_transferred = 0;
        uint8_t revision_msb = 0u;
        uint8_t revision_lsb = 0u;
        uint16_t revision = 0u;
        
        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xEA, 0x0000, 0x0000, &revision_msb, 0x01, 1000u);
        if (bytes_transferred != 1)
        {
            throw std::runtime_error("did not get msb of revision number");
        }
        
        bytes_transferred = libusb_control_transfer(usbdevicehandle, 0xC0, 0xEB, 0x0000, 0x0000, &revision_lsb, 0x01, 1000u);
        if (bytes_transferred != 1)
        {
            throw std::runtime_error("did not get lsb of revision number");
        }
        
        revision = revision_msb;
        revision <<= 8;
        revision |= revision_lsb;        
        
        return revision;
    }

    II2C_Master* Viperboard::GetI2CMasterInterface(void)
    {
        if (nullptr == i2c_master)
        {
            i2c_master = new I2CMasterViperboard(usbdevicehandle);
        }
        
        return i2c_master;
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
