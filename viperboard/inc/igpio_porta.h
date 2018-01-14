#ifndef _IGPIO_PORTA_H_
#define _IGPIO_PORTA_H_

#include <stdint.h>
#include "vipertypes.h"

namespace Viper
{
    
    class IGPIO_PortA 
    {
        
        public:
            virtual ViperResult_t SetPWMMode(uint8_t bit, uint16_t level, uint16_t clock)=0;
    };
}

#endif
