#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "libusb_mock.hpp"
#include "viperboard.h"
#include "igpio_porta.h"

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

class ViperboardGpioATest : public ::testing::Test
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

TEST_F(ViperboardGpioATest, GetGpioInterfaceSuccess)
{
    IGPIO_PortA* pGpio = pViper->GetGpioPortAInterface();
    ASSERT_FALSE(nullptr == pGpio);
   
}

TEST_F(ViperboardGpioATest, GetGpioInterfaceTwiceReturnsSameInterface)
{
    IGPIO_PortA* pGpio1 = pViper->GetGpioPortAInterface();
    IGPIO_PortA* pGpio2 = pViper->GetGpioPortAInterface();
    
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


TEST_F(ViperboardGpioATest, SetPWMModeSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortA* pGpio = pViper->GetGpioPortAInterface();
    uint8_t bit = 4;
    uint8_t level = 55;
    uint8_t clock = 0x73;
    uint16_t length = 11;
    uint8_t data[50] = {0xAA};

    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xED), Eq(0x0000), Eq(0x0000), _, Eq(length), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(length)));

    result = pGpio->SetPWMMode(bit, level, clock);

    ASSERT_EQ(0x02, data[0]);
    ASSERT_EQ(clock, data[1]);
    ASSERT_EQ(bit, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(level, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x00, data[9]);
    ASSERT_EQ(0x00, data[10]);
    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardGpioATest, SetPWMModeIncorrectNrBytesTransactionfailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortA* pGpio = pViper->GetGpioPortAInterface();
    uint8_t bit = 4;
    uint8_t level = 55;
    uint8_t clock = 0x73;
    uint16_t length = 11;
    uint8_t data[50] = {0xAA};

    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xED), Eq(0x0000), Eq(0x0000), _, Eq(length), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(2)));

    result = pGpio->SetPWMMode(bit, level, clock);

    ASSERT_EQ(0x02, data[0]);
    ASSERT_EQ(clock, data[1]);
    ASSERT_EQ(bit, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(level, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x00, data[9]);
    ASSERT_EQ(0x00, data[10]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

TEST_F(ViperboardGpioATest, SetPWMModeLibusbErrorTransactionfailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    IGPIO_PortA* pGpio = pViper->GetGpioPortAInterface();
    uint8_t bit = 4;
    uint8_t level = 55;
    uint8_t clock = 0x73;
    uint16_t length = 11;
    uint8_t data[50] = {0xAA};

    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xED), Eq(0x0000), Eq(0x0000), _, Eq(length), Eq(1000u))).WillOnce(DoAll(WithArg<5>(SaveArrayPointee(data, length)), Return(LIBUSB_ERROR_NO_DEVICE)));

    result = pGpio->SetPWMMode(bit, level, clock);

    ASSERT_EQ(0x02, data[0]);
    ASSERT_EQ(clock, data[1]);
    ASSERT_EQ(bit, data[2]);
    ASSERT_EQ(0x00, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(level, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x00, data[9]);
    ASSERT_EQ(0x00, data[10]);
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}