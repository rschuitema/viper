#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "libusb_mock.hpp"
#include "viperboard.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using namespace Viper;

TEST(ViperboardTest, ConstructionSuccess)
{
    libusb_context context;
    pLibUsbMock = new LibUsbMock();
    EXPECT_CALL(*pLibUsbMock, init(_)).WillOnce(DoAll(SetArgPointee<0>(&context), 
                                                      Return(LIBUSB_SUCCESS)));

    Viperboard* pViper = new Viperboard();
    EXPECT_FALSE(nullptr == pViper);

    delete pViper;
    delete pLibUsbMock;
}
