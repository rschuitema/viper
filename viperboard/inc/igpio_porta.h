#ifndef _IGPIO_PORTA_H_
#define _IGPIO_PORTA_H_

#include <stdint.h>
#include "vipertypes.h"

namespace Viper
{
    
    class IGPIO_PortA 
    {
        
        public:
            virtual ViperResult_t SetContinuousMode(uint8_t bit, uint8_t clock, uint8_t t1, uint8_t t2)=0;
            virtual ViperResult_t SetPulseMode(uint8_t bit, uint8_t clock, uint8_t t1, uint8_t t2, bool invert)=0;
            virtual ViperResult_t SetPWMMode(uint8_t bit, uint8_t level, uint8_t clock)=0;
            virtual ViperResult_t SetDigitalOutputMode(uint8_t bit, bool value)=0;
            virtual ViperResult_t SetDigitalInputMode(uint8_t bit, uint8_t clock)=0;
    };
}

#endif
