#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "libusb_mock.hpp"
#include "viperboard.h"
#include "igpio_portb.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::Throw;
using ::testing::Eq;
using ::testing::A;
using ::testing::SaveArgPointee;
using ::testing::WithArg;
using ::testing::DoAll;
using ::testing::SetArrayArgument;

using namespace Viper;

#define VIPERBOARD_VENDOR_ID  (0x2058)
#define VIPERBOARD_PRODUCT_ID (0x1005)

class ViperboardGpioBTest : public ::testing::Test
{
    protected:
        libusb_context context;
        libusb_device_handle handle;
        Viperboard* pViper = nullptr;

    protected:
        virtual void SetUp()
        {
            pLibUsbMock = new LibUsbMock();

            EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce( DoAll( SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS) ) );
            pViper = new Viperboard();

            EXPECT_FALSE(nullptr == pViper);

            EXPECT_CALL(*pLibUsbMock, open_device_with_vid_pid(_, Eq(VIPERBOARD_VENDOR_ID), Eq(VIPERBOARD_PRODUCT_ID))).WillOnce(Return(&handle));
            EXPECT_CALL(*pLibUsbMock, kernel_driver_active(_, Eq(0))).WillOnce(Return(0));
            EXPECT_CALL(*pLibUsbMock, set_configuration(_, Eq(1))).WillOnce(Return(0));
            EXPECT_CALL(*pLibUsbMock, claim_interface(_,Eq(0))).WillOnce(Return(0));
            EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, 0x86, _, 512, _, 10)).Times(2).WillRepeatedly(Return(0));

            ASSERT_EQ(VIPER_SUCCESS, pViper->Open());
        }
        
        virtual void TearDown()
        {
            EXPECT_CALL(*pLibUsbMock, release_interface(_, Eq(0))).WillOnce(Return(0));
            EXPECT_CALL(*pLibUsbMock, close(_));
            EXPECT_CALL(*pLibUsbMock, exit(_));

            ASSERT_EQ(VIPER_SUCCESS, pViper->Close());

            delete pViper;
            delete pLibUsbMock;
        }
        
};

// The testcases can asume the viperboard is opened and closed successfully in the setup and teardown
// This will put more focus on the intention of the actual testcases

TEST_F(ViperboardGpioBTest, GetGpioInterfaceSuccess)
{
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    ASSERT_FALSE(nullptr == pGpio);
   
}

TEST_F(ViperboardGpioBTest, GetGpioInterfaceTwiceReturnsSameInterface)
{
    IGPIO_PortB* pGpio1 = pViper->GetGpioPortBInterface();
    IGPIO_PortB* pGpio2 = pViper->GetGpioPortBInterface();
    
    ASSERT_FALSE(nullptr == pGpio1);
    ASSERT_FALSE(nullptr == pGpio2);
    ASSERT_TRUE(pGpio2 == pGpio1);
   
}

ACTION_P2(SaveArrayPointee, pointer, length)
{
    for (int i = 0; i < length; i++)
    {
        pointer[i] = arg0[i];
    }
}


TEST_F(ViperboardGpioBTest, SetPortDirectionIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 5;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(7)));
    
    result = pGpio->SetPortDirection(0x0000, 0xFFFF);
    
    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0xFF, data[3]);
    ASSERT_EQ(0xFF, data[4]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

TEST_F(ViperboardGpioBTest, SetPortDirectionLibusbErrorTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 5;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(LIBUSB_ERROR_NO_DEVICE)));
    
    result = pGpio->SetPortDirection(0x0000, 0xFFFF);
    
    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0xFF, data[3]);
    ASSERT_EQ(0xFF, data[4]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

TEST_F(ViperboardGpioBTest, SetPortDirectionSomeBitsToInputSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 5;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(5)));
    
    result = pGpio->SetPortDirection(0x34AE, 0xFFFF);
    
    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x34, data[1]);
    ASSERT_EQ(0xAE, data[2]);
    ASSERT_EQ(0xFF, data[3]);
    ASSERT_EQ(0xFF, data[4]);
    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardGpioBTest, SetPortDirectionOnlyAllowedBitsToInputSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 5;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(5)));
    
    result = pGpio->SetPortDirection(0x0000, 0xF05A);
    
    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0xF0, data[3]);
    ASSERT_EQ(0x5A, data[4]);
    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardGpioBTest, GetPortDirectionSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xDE, 0xAD};
    uint16_t port_direction = 0u;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+5), Return(5)));
    
    result = pGpio->GetPortDirection(&port_direction);
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    ASSERT_EQ(0xDEAD, port_direction);
}

TEST_F(ViperboardGpioBTest, GetPortDirectionIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xDE, 0xAD};
    uint16_t port_direction = 0u;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+5), Return(88)));
    
    result = pGpio->GetPortDirection(&port_direction);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
    ASSERT_EQ(0x0u, port_direction);
}

TEST_F(ViperboardGpioBTest, GetPortDirectionLibusbErrorTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xDE, 0xAD};
    uint16_t port_direction = 0u;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+5), Return(LIBUSB_ERROR_NO_DEVICE)));
    
    result = pGpio->GetPortDirection(&port_direction);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
    ASSERT_EQ(0x0u, port_direction);
}

TEST_F(ViperboardGpioBTest, WritePortSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 5;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(5)));
    
    result = pGpio->WritePort(0x0000, 0xF05A);
    
    ASSERT_EQ(0x01, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0xF0, data[3]);
    ASSERT_EQ(0x5A, data[4]);
    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardGpioBTest, WritePortIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 5;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(9)));
    
    result = pGpio->WritePort(0x0000, 0xF05A);
    
    ASSERT_EQ(0x01, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0xF0, data[3]);
    ASSERT_EQ(0x5A, data[4]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

TEST_F(ViperboardGpioBTest, WritePortLibusbErrorTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 5;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(LIBUSB_ERROR_NO_DEVICE)));
    
    result = pGpio->WritePort(0x0000, 0xF05A);
    
    ASSERT_EQ(0x01, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0xF0, data[3]);
    ASSERT_EQ(0x5A, data[4]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

TEST_F(ViperboardGpioBTest, ReadPortSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[3] = {0xFF, 0xAB, 0xCD};
    int16_t length = 5;
    uint16_t port_value = 0u;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+3), Return(5)));
    
    result = pGpio->ReadPort(&port_value);
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    ASSERT_EQ(0xABCD, port_value);
}

TEST_F(ViperboardGpioBTest, ReadPortIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[3] = {0xFF, 0xAB, 0xCD};
    int16_t length = 5;
    uint16_t port_value = 0u;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+3), Return(3)));
    
    result = pGpio->ReadPort(&port_value);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
    ASSERT_EQ(0u, port_value);
}

TEST_F(ViperboardGpioBTest, ReadPortLibusbErrorTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[3] = {0xFF, 0xAB, 0xCD};
    int16_t length = 5;
    uint16_t port_value = 0u;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+3), Return(LIBUSB_ERROR_NO_DEVICE)));
    
    result = pGpio->ReadPort(&port_value);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
    ASSERT_EQ(0u, port_value);
}

TEST_F(ViperboardGpioBTest, SetBitDirectionToInputSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 7;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(7u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(7)));
    
    result = pGpio->SetBitDirection(7, false);
    
    ASSERT_EQ(0x02, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x07, data[5]);
    ASSERT_EQ(0x00, data[6]);
    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardGpioBTest, SetBitDirectionToOutputSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 7;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(7u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(7)));
    
    result = pGpio->SetBitDirection(7, true);
    
    ASSERT_EQ(0x02, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x07, data[5]);
    ASSERT_EQ(0x01, data[6]);
    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardGpioBTest, SetBitDirectionIncorrectBitInvalidParameter)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();

    result = pGpio->SetBitDirection(16, true);
    
    ASSERT_EQ(VIPER_INVALID_PARAMETER, result);
}

TEST_F(ViperboardGpioBTest, SetBitDirectionIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 7;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(7u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(8)));
    
    result = pGpio->SetBitDirection(7, true);
    
    ASSERT_EQ(0x02, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x07, data[5]);
    ASSERT_EQ(0x01, data[6]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

TEST_F(ViperboardGpioBTest, SetBitDirectionLibusbErrorTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 7;

    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(7u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(LIBUSB_ERROR_NO_DEVICE)));
    
    result = pGpio->SetBitDirection(7, true);
    
    ASSERT_EQ(0x02, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x07, data[5]);
    ASSERT_EQ(0x01, data[6]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

TEST_F(ViperboardGpioBTest, GetBitDirectionInputSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xEF};
    bool bit_direction = true;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+5), Return(5)));
    
    result = pGpio->GetBitDirection(4, &bit_direction);
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    ASSERT_FALSE(bit_direction);
}

TEST_F(ViperboardGpioBTest, GetBitDirectionLibusbErrorTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xEF};
    bool bit_direction = true;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+5), Return(LIBUSB_ERROR_NO_DEVICE)));
    
    result = pGpio->GetBitDirection(4, &bit_direction);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
    ASSERT_TRUE(bit_direction);
}

TEST_F(ViperboardGpioBTest, GetBitDirectionIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xEF};
    bool bit_direction = true;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(5u), Eq(1000u))).WillOnce(DoAll(SetArrayArgument<5>(data, data+5), Return(67)));
    
    result = pGpio->GetBitDirection(4, &bit_direction);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
    ASSERT_TRUE(bit_direction);
}

TEST_F(ViperboardGpioBTest, GetBitDirectionIncorrectBitInvalidParameter)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xEF};
    bool bit_direction = true;
    
    result = pGpio->GetBitDirection(66, &bit_direction);
    
    ASSERT_EQ(VIPER_INVALID_PARAMETER, result);
    ASSERT_TRUE(bit_direction);
}


TEST_F(ViperboardGpioBTest, GetBitDirectionNullptrInvalidParameter)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xEF};
    bool bit_direction = true;
    
    result = pGpio->GetBitDirection(4, nullptr);
    
    ASSERT_EQ(VIPER_INVALID_PARAMETER, result);
    ASSERT_TRUE(bit_direction);
}

TEST_F(ViperboardGpioBTest, WriteBitHighSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortB* pGpio = pViper->GetGpioPortBInterface();
    uint8_t data[50] = {0xAA};
    int16_t length = 7;

    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xDD), Eq(0x0000), Eq(0x0000), _, Eq(7u), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(7)));
    
    result = pGpio->WriteBit(9, true);
    
    ASSERT_EQ(0x03, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x00, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x09, data[5]);
    ASSERT_EQ(0x01, data[6]);
    ASSERT_EQ(VIPER_SUCCESS, result);
}


