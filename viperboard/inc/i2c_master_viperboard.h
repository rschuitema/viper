
#include "vipertypes.h"
#include "ii2c_master.h"

namespace Viperboard
{
    
    class I2CMasterViperboard : public(II2C_Master)
    {
        int SetFrequency(int frequency)=0;
        int ScanConnectedDevices(unsigned char* plist, int length)=0;
        int Write(unsigned char slave_address, unsigned char register_address, int length, unsigned char* pBuffer)=0;
        int Read(unsigned char slave_address, unsigned char register_address, int length, unsigned char* pBuffer)=0;
    };
    
}