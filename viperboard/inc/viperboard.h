#ifndef _VIPERBOARD_H_
#define _VIPERBOARD_H_

#include "vipertypes.h"

namespace Viperboard
{

    class Viperboard {
        
    public:
        Viperboard(void);
        virtual ~Viperboard(void);

        ViperResult_t Open(void);
        ViperResult_t Close(void);
        
        void SetI2CMaster(I2C_Master* master);
        void SetI2CSlave(I2C_Slave* slave);
        
        ViperResult_t Revision(void);
        
    private:
        I2C_Master* i2c_master = NULL;
        I2C_Slave*  i2c_slave = NULL;
        
    };
}

#endif