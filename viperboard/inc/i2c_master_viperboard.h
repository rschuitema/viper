#include <stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <libusb.h>
#pragma GCC diagnostic pop

#include "vipertypes.h"
#include "ii2c_master.h"

namespace Viper
{
    
    class I2CMasterViperboard : public II2C_Master
    {
        public:
            I2CMasterViperboard(libusb_device_handle* usbdevicehandle);
            virtual ~I2CMasterViperboard(void);

            ViperResult_t SetFrequency(I2CFrequency_t frequency);
            ViperResult_t ScanConnectedDevices(bool* plist, uint8_t length);
            ViperResult_t Write(uint8_t slave_address, uint8_t register_address, uint16_t length, uint8_t* pBuffer);
            ViperResult_t Read(uint8_t slave_address, uint8_t register_address, int16_t length, uint8_t* pBuffer);
            
        private:
            libusb_device_handle* usbdevicehandle = nullptr;

    };
    
}
