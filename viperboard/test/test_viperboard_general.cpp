#include <stdexcept>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "libusb_mock.hpp"
#include "viperboard.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::Throw;
using ::testing::Eq;
using ::testing::A;

using namespace Viper;

#define VIPERBOARD_VENDOR_ID  (0x2058)
#define VIPERBOARD_PRODUCT_ID (0x1005)

class ViperboardGenaralTest : public ::testing::Test
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

TEST_F(ViperboardGenaralTest, RevisionSuccess)
{
    uint16_t revision = 0;
    uint8_t revision_lsb = 0xCD;
    uint8_t revision_msb = 0xAB;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEA), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_msb), Return(1)));
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEB), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_lsb), Return(1)));

    revision = pViper->Revision();
    
    ASSERT_EQ(0xABCD, revision);
}

TEST_F(ViperboardGenaralTest, RevisionMsbNotReturnCorrectNrBytes)
{
    uint16_t revision = 0;
    uint8_t revision_lsb = 0xCD;
    uint8_t revision_msb = 0xAB;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEA), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_msb), Return(3)));

    EXPECT_THROW(revision = pViper->Revision(),std::runtime_error);
    
}

TEST_F(ViperboardGenaralTest, RevisionMsbUsbError)
{
    uint16_t revision = 0;
    uint8_t revision_lsb = 0xCD;
    uint8_t revision_msb = 0xAB;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEA), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_msb), Return(LIBUSB_ERROR_OTHER)));

    EXPECT_THROW(revision = pViper->Revision(),std::runtime_error);
    
}

TEST_F(ViperboardGenaralTest, RevisionLsbNotReturnCorrectNrBytes)
{
    uint16_t revision = 0;
    uint8_t revision_lsb = 0xCD;
    uint8_t revision_msb = 0xAB;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEA), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_msb), Return(1)));
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEB), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_lsb), Return(9)));

    EXPECT_THROW(revision = pViper->Revision(),std::runtime_error);
    
}

TEST_F(ViperboardGenaralTest, RevisionLsbUsbError)
{
    uint16_t revision = 0;
    uint8_t revision_lsb = 0xCD;
    uint8_t revision_msb = 0xAB;
    
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEA), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_msb), Return(1)));
    EXPECT_CALL(*pLibUsbMock, control_transfer(_, Eq(0xC0), Eq(0xEB), Eq(0x0000), Eq(0x0000), _, Eq(1u), Eq(1000u))).WillOnce(DoAll(SetArgPointee<5>(revision_lsb), Return(LIBUSB_ERROR_OTHER)));

    EXPECT_THROW(revision = pViper->Revision(),std::runtime_error);
    
}

