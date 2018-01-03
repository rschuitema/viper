#ifndef _IGPIO_PORTB_H_
#define _IGPIO_PORTB_H_

#include <stdint.h>
#include "vipertypes.h"

namespace Viper
{
    
    class IGPIO_PortB 
    {
        
        public:
            virtual ViperResult_t SetPortDirection(uint16_t value, uint16_t mask)=0;
            virtual ViperResult_t GetPortDirection(uint16_t* value)=0;
            virtual ViperResult_t WritePort(uint16_t value, uint16_t mask)=0;
            virtual ViperResult_t ReadPort(uint16_t* value)=0;
            
            virtual ViperResult_t SetBitDirection(uint8_t bit, bool direction)=0;
            virtual ViperResult_t GetBitDirection(uint8_t bit, bool* direction)=0;
            virtual ViperResult_t WriteBit(uint8_t bit, bool direction)=0;
            virtual ViperResult_t ReadBit(uint8_t bit, bool* direction)=0;

    };
}

#endif
