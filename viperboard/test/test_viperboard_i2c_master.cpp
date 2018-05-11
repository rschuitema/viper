#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "libusb_mock.hpp"
#include "viperboard.h"
#include "ii2c_master.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::Throw;
using ::testing::Eq;
using ::testing::A;
using ::testing::SaveArgPointee;

using namespace Viper;

#define VIPERBOARD_VENDOR_ID  (0x2058)
#define VIPERBOARD_PRODUCT_ID (0x1005)

class ViperboardI2CMasterTest : public ::testing::Test
{
    protected:
        libusb_context context;
        libusb_device_handle handle;
        Viperboard* pViper = nullptr;

    protected:
        virtual void SetUp()
        {
            pLibUsbMock = new LibUsbMock();

            EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS)));
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

TEST_F(ViperboardI2CMasterTest, GetI2CMasterInterfaceSuccess)
{
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    ASSERT_FALSE(nullptr == pI2CMaster);
   
}

TEST_F(ViperboardI2CMasterTest, GetI2CMasterInterfaceTwiceReturnsSameInterface)
{
    II2C_Master* pI2CMaster1 = pViper->GetI2CMasterInterface();
    II2C_Master* pI2CMaster2 = pViper->GetI2CMasterInterface();
    
    ASSERT_FALSE(nullptr == pI2CMaster1);
    ASSERT_FALSE(nullptr == pI2CMaster2);
    ASSERT_TRUE(pI2CMaster2 == pI2CMaster1);
   
}

TEST_F(ViperboardI2CMasterTest, SetFrequency400KHZSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    I2CFrequency_t frequency = I2C_FREQUENCY_400KHZ;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    uint8_t bus_frequency = 0u;

    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xE1), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SaveArgPointee<5>(&bus_frequency), Return(1)));
    
    result = pI2CMaster->SetFrequency(frequency);
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    ASSERT_EQ(4u, bus_frequency);
}

TEST_F(ViperboardI2CMasterTest, SetFrequency6MHZSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    I2CFrequency_t frequency = I2C_FREQUENCY_6MHZ;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    uint8_t bus_frequency = 0u;

    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xE1), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SaveArgPointee<5>(&bus_frequency), Return(1)));
    
    result = pI2CMaster->SetFrequency(frequency);
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    ASSERT_EQ(1u, bus_frequency);
}

TEST_F(ViperboardI2CMasterTest, SetFrequencyIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    I2CFrequency_t frequency = I2C_FREQUENCY_6MHZ;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    uint8_t bus_frequency = 0u;

    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0x40), Eq(0xE1), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SaveArgPointee<5>(&bus_frequency), Return(34)));
    
    result = pI2CMaster->SetFrequency(frequency);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

#define MAX_DEVICES (128)

TEST_F(ViperboardI2CMasterTest, ScanOneConnectedDeviceSuccess)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    bool deviceList[MAX_DEVICES] = {false};


    result = pI2CMaster->ScanConnectedDevices(deviceList, MAX_DEVICES);
    
    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

