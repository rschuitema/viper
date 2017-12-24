#ifndef _II2C_SLAVE_H_
#define _II2C_SLAVE_H_

#include "vipertypes.h"

namespace Viperboard
{
    
    class II2C_Slave {
    public:
        virtual ViperResult_t SetSlaveAddress(unsigned char slave_address)=0;
        virtual ViperResult_t Arm(void)=0;
        virtual ViperResult_t Disarm(void)=0;
        virtual ViperResult_t WriteBuffer(unsigned char buffer, int length, unsigned char* pBuffer)=0;
        virtual ViperResult_t ReadBuffer(unsigned char buffer, int length, unsigned char* pBuffer)=0;

    }
}

#endif