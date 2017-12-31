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

TEST(ViperboardTest, ConstructionDesctuctionSuccess)
{
    libusb_context context;
    pLibUsbMock = new LibUsbMock();
    
    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS)));
    EXPECT_CALL(*pLibUsbMock, exit(_));


    Viperboard* pViper = new Viperboard();
    EXPECT_FALSE(nullptr == pViper);

    delete pViper;
    delete pLibUsbMock;
}

TEST(ViperboardTest, ConstructionThrowExceptionNullPointer)
{
    Viperboard* pViper = nullptr;
    pLibUsbMock = new LibUsbMock();
    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(Return(LIBUSB_SUCCESS));

    EXPECT_THROW(pViper = new Viperboard(), std::runtime_error);
    EXPECT_TRUE(nullptr == pViper);

    delete pViper;
    delete pLibUsbMock;
}

TEST(ViperboardTest, ConstructionThrowExceptionFailed)
{
    Viperboard* pViper = nullptr;
    pLibUsbMock = new LibUsbMock();
    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(Return(LIBUSB_ERROR_OTHER));

    EXPECT_THROW(pViper = new Viperboard(), std::runtime_error);
    EXPECT_TRUE(nullptr == pViper);

    delete pViper;
    delete pLibUsbMock;
}

TEST(ViperboardTest, DestructionDoesNotThrowException)
{
    libusb_context context;
    Viperboard* pViper = nullptr;
    pLibUsbMock = new LibUsbMock();

    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS)));
    EXPECT_CALL(*pLibUsbMock, exit(_)).WillOnce(Throw(std::runtime_error("Error")));

    pViper = new Viperboard();
    EXPECT_FALSE(nullptr == pViper);

    EXPECT_NO_THROW({delete pViper;});

    delete pLibUsbMock;
}


TEST(ViperboardTest, OpenKernelDriverNotActiveSuccess)
{
    libusb_context context;
    libusb_device_handle handle;
    Viperboard* pViper = nullptr;
    ViperResult_t result = VIPER_TRANSACTION_FAILURE;
    pLibUsbMock = new LibUsbMock();

    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS)));
    pViper = new Viperboard();

    EXPECT_CALL(*pLibUsbMock, open_device_with_vid_pid(_, Eq(VIPERBOARD_VENDOR_ID), Eq(VIPERBOARD_PRODUCT_ID))).WillOnce(Return(&handle));
    EXPECT_CALL(*pLibUsbMock, kernel_driver_active(_, Eq(0))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, set_configuration(_, Eq(1))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, claim_interface(_,Eq(0))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, 0x86, _, 512, _, 10)).Times(2).WillRepeatedly(Return(0));
    
    result = pViper->Open();
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    
    delete pLibUsbMock;
}

TEST(ViperboardTest, OpenKernelDriverActiveSuccess)
{
    libusb_context context;
    libusb_device_handle handle;
    Viperboard* pViper = nullptr;
    ViperResult_t result = VIPER_TRANSACTION_FAILURE;
    pLibUsbMock = new LibUsbMock();

    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS)));
    pViper = new Viperboard();

    EXPECT_CALL(*pLibUsbMock, open_device_with_vid_pid(_, Eq(VIPERBOARD_VENDOR_ID), Eq(VIPERBOARD_PRODUCT_ID))).WillOnce(Return(&handle));
    EXPECT_CALL(*pLibUsbMock, kernel_driver_active(_, Eq(0))).WillOnce(Return(1));
    EXPECT_CALL(*pLibUsbMock, detach_kernel_driver(_, Eq(0))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, set_configuration(_, Eq(1))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, claim_interface(_, Eq(0))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, 0x86, _, 512, _, 10)).Times(2).WillRepeatedly(Return(0));
    
    result = pViper->Open();
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    
    delete pLibUsbMock;
}

TEST(ViperboardTest, OpenFailsDeviceNotFound)
{
    libusb_context context;
    libusb_device_handle handle;
    Viperboard* pViper = nullptr;
    ViperResult_t result = VIPER_TRANSACTION_FAILURE;
    pLibUsbMock = new LibUsbMock();

    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS)));
    pViper = new Viperboard();

    EXPECT_CALL(*pLibUsbMock, open_device_with_vid_pid(_, _, _)).WillOnce(Return(nullptr));
    result = pViper->Open();
    
    ASSERT_EQ(VIPER_HW_NOT_FOUND, result);
    
    delete pLibUsbMock;
}

TEST(ViperboardTest, CloseSuccess)
{
    libusb_context context;
    libusb_device_handle handle;
    Viperboard* pViper = nullptr;
    ViperResult_t result = VIPER_TRANSACTION_FAILURE;
    pLibUsbMock = new LibUsbMock();

    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), Return(LIBUSB_SUCCESS)));
    pViper = new Viperboard();
    
    
    EXPECT_CALL(*pLibUsbMock, open_device_with_vid_pid(_, Eq(VIPERBOARD_VENDOR_ID), Eq(VIPERBOARD_PRODUCT_ID))).WillOnce(Return(&handle));
    EXPECT_CALL(*pLibUsbMock, kernel_driver_active(_, Eq(0))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, set_configuration(_, Eq(1))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, claim_interface(_,Eq(0))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, 0x86, _, 512, _, 10)).Times(2).WillRepeatedly(Return(0));
    pViper->Open();    
    
    EXPECT_CALL(*pLibUsbMock, release_interface(_, Eq(0))).WillOnce(Return(0));
    EXPECT_CALL(*pLibUsbMock, close(_));

    result = pViper->Close();
    
    ASSERT_EQ(VIPER_SUCCESS, result);
    
    delete pLibUsbMock;
}


