
#include "vipertypes.h"
#include "ii2c_master.h"

namespace Viperboard
{
    
    class I2CMasterViperboard : public II2C_Master
    {
        I2CMasterViperboard(void);
        ~I2CMasterViperboard(void);

        ViperResult_t SetFrequency(int frequency);
        ViperResult_t ScanConnectedDevices(unsigned char* plist, int length);
        ViperResult_t Write(unsigned char slave_address, unsigned char register_address, int length, unsigned char* pBuffer);
        ViperResult_t Read(unsigned char slave_address, unsigned char register_address, int length, unsigned char* pBuffer);
    };
    
}
