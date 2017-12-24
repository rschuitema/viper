#ifndef _II2C_MASTER_H_
#define _II2C_MASTER_H_

#include "vipertypes.h"

namespace Viperboard
{
    
    class II2C_Master {
        
    public:
        virtual ViperResult_t SetFrequency(int frequency)=0;
        virtual ViperResult_t ScanConnectedDevices(unsigned char* plist, int length)=0;
        virtual ViperResult_t Write(unsigned char slave_address, unsigned char register_address, int length, unsigned char* pBuffer)=0;
        virtual ViperResult_t Read(unsigned char slave_address, unsigned char register_address, int length, unsigned char* pBuffer)=0;
        
    }
}

#endif