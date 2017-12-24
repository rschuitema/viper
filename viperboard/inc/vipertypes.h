#ifndef _VIPER_TYPES_H_
#define _VIPER_TYPES_H_

typedef enum 
{
	VIPER_SUCCESS = 0,                  // Call was successful
	VIPER_XACTION_FAILURE = 1,          // There was an error in the USB transaction
	VIPER_HW_NOT_FOUND = 2,             // No hardware was found
	VIPER_BAD_PARAMETER = 3,            // Bad or out of range paramters
	VIPER_IIC_PROTOCOL_ERROR = 4        // There was an error discovered in the IIC transfer, for example an ACK error
} ViperResult_t;


#endif