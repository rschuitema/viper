#include <stdexcept>
#include "vipertypes.h"
#include "viperboard.h"

namespace Viper
{
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

    ViperResult_t Viperboard::Open(void)
    {
        int result = 0;

        result = libusb_init(&usbcontext);
        
        return VIPER_SUCCESS;
    }
}
