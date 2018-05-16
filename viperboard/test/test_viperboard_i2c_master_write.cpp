#include <string.h>
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
using ::testing::WithArg;
using ::testing::SetArrayArgument;
using ::testing::DoAll;

using namespace Viper;

#define VIPERBOARD_VENDOR_ID  (0x2058)
#define VIPERBOARD_PRODUCT_ID (0x1005)

class ViperboardI2CMasterWriteTest : public ::testing::Test
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


ACTION_P2(SaveArrayPointee, pointer, length)
{
    for (int i = 0; i < length; i++)
    {
        pointer[i] = arg0[i];
    }
}



TEST_F(ViperboardI2CMasterWriteTest, Write10SuccessOneTransfer)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t data[200] = {0xAA};
    
    uint8_t msg[100] = {0x59};
    uint16_t msgLength = 10;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    uint16_t transferLength = msgLength+9;
    int bytesTransferred = 0;


    memset(msg, 0x59, 100);
    memset(data, 0xAA, 200);

    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(transferLength), _, Eq(1000u))).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data, transferLength)), SetArgPointee<4>(transferLength), Return(0)));

    result = pI2CMaster->Write(slaveAddress, registerAddress, msgLength, msg);

    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x40, data[2]);
    ASSERT_EQ(0x0A, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(0x00, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x59, data[9]);
    ASSERT_EQ(0x59, data[10]);
    ASSERT_EQ(0x59, data[11]);
    ASSERT_EQ(0x59, data[12]);
    ASSERT_EQ(0x59, data[13]);
    ASSERT_EQ(0x59, data[14]);
    ASSERT_EQ(0x59, data[15]);
    ASSERT_EQ(0x59, data[16]);
    ASSERT_EQ(0x59, data[17]);
    ASSERT_EQ(0x59, data[18]);
    ASSERT_EQ(0xAA, data[19]);

    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardI2CMasterWriteTest, Write213SuccessOneTransfer)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t data[300] = {0xAA};
    
    uint8_t msg[300] = {0x59};
    uint16_t msgLength = 213;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    uint16_t transferLength = msgLength+9;
    int bytesTransferred = 0;


    memset(msg, 0x59, 300);
    memset(data, 0xAA, 300);

    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(transferLength), _, Eq(1000u))).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data, transferLength)), SetArgPointee<4>(transferLength), Return(0)));

    result = pI2CMaster->Write(slaveAddress, registerAddress, msgLength, msg);

    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x40, data[2]);
    ASSERT_EQ(0xD5, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(0x00, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x59, data[9]);
    ASSERT_EQ(0x59, data[10]);
    ASSERT_EQ(0x59, data[220]);
    ASSERT_EQ(0x59, data[221]);
    ASSERT_EQ(0xAA, data[222]);

    ASSERT_EQ(VIPER_SUCCESS, result);
}

MATCHER_P2(EqArray, buffer, length, "")
{
    return std::equal(arg, arg + length, buffer);
}

TEST_F(ViperboardI2CMasterWriteTest, Write20SuccessOneTransfer)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t data[300] = {0xAA};
    
    uint8_t msg[300] = {0x59};
    uint16_t msgLength = 20;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    uint16_t transferLength = msgLength+9;
    int bytesTransferred = 0;


    memset(msg, 0x59, 300);
    memset(data, 0x59, 29);
    data[0] = 0;
    data[1] = 0;
    data[2] = 0x40;
    data[3] = 0x14;
    data[4] = 0x00;
    data[5] = 0;
    data[6] = 0;
    data[7] = 0;
    data[8] = 0;

    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), EqArray(data, transferLength), Eq(transferLength), _, Eq(1000u))).WillOnce(DoAll(SetArgPointee<4>(transferLength), Return(0)));

    result = pI2CMaster->Write(slaveAddress, registerAddress, msgLength, msg);


    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardI2CMasterWriteTest, Write290SuccessOneTransfer)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t data[300] = {0xAA};
    
    uint8_t msg[300] = {0x59};
    uint16_t msgLength = 290;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    uint16_t transferLength = msgLength+9;
    int bytesTransferred = 0;


    memset(msg, 0x59, 300);
    memset(data, 0xAA, 300);

    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(transferLength), _, Eq(1000u))).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data, transferLength)), SetArgPointee<4>(transferLength), Return(0)));

    result = pI2CMaster->Write(slaveAddress, registerAddress, msgLength, msg);

    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x40, data[2]);
    ASSERT_EQ(0x22, data[3]);
    ASSERT_EQ(0x01, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(0x00, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x59, data[9]);
    ASSERT_EQ(0x59, data[10]);
    ASSERT_EQ(0x59, data[297]);
    ASSERT_EQ(0x59, data[298]);
    ASSERT_EQ(0xAA, data[299]);

    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardI2CMasterWriteTest, Write600SuccessTwoTransfers)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t data[600] = {0xAA};
    uint8_t data2[600] = {0xAA};
    
    uint8_t msg[600] = {0x59};
    uint16_t msgLength = 600;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    int bytesTransferred = 0;


    memset(msg, 0x59, 600);
    memset(data, 0xAA, 600);
    memset(data2, 0xAA, 600);

    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(512), _, Eq(1000u))).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data, 512)), SetArgPointee<4>(512), Return(0)));
    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(106), _, Eq(1000u))).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data2, 106)), SetArgPointee<4>(106), Return(0)));

    result = pI2CMaster->Write(slaveAddress, registerAddress, msgLength, msg);

    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0x00, data[1]);
    ASSERT_EQ(0x40, data[2]);
    ASSERT_EQ(0xF7, data[3]);
    ASSERT_EQ(0x01, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(0x00, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x59, data[9]);
    ASSERT_EQ(0x59, data[10]);
    ASSERT_EQ(0x59, data[510]);
    ASSERT_EQ(0x59, data[511]);
    ASSERT_EQ(0xAA, data[512]);

    ASSERT_EQ(0x00, data2[0]);
    ASSERT_EQ(0xF7, data2[1]);
    ASSERT_EQ(0x41, data2[2]);
    ASSERT_EQ(0x61, data2[3]);
    ASSERT_EQ(0x00, data2[4]);
    ASSERT_EQ(0x00, data2[5]);
    ASSERT_EQ(0x00, data2[6]);
    ASSERT_EQ(0x00, data2[7]);
    ASSERT_EQ(0x00, data2[8]);
    ASSERT_EQ(0x59, data2[9]);
    ASSERT_EQ(0x59, data2[10]);
    ASSERT_EQ(0x59, data2[104]);
    ASSERT_EQ(0x59, data2[105]);
    ASSERT_EQ(0xAA, data2[106]);

    ASSERT_EQ(VIPER_SUCCESS, result);
}


TEST_F(ViperboardI2CMasterWriteTest, Write2048SuccessFiveTransfers)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t data[600] = {0xAA};
    uint8_t data1[600] = {0xAA};

    uint8_t msg[2048] = {0x59};
    uint16_t msgLength = 2048;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    int bytesTransferred = 0;


    memset(msg, 0x59, 2048);
    memset(data, 0xAA, 600);
    memset(data1, 0xAA, 600);

    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(512), _, Eq(1000u))).Times(4).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data1, 512)), SetArgPointee<4>(512), Return(0)))
                                                                                            .WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data1, 512)), SetArgPointee<4>(512), Return(0)))
                                                                                            .WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data1, 512)), SetArgPointee<4>(512), Return(0)))
                                                                                            .WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data1, 512)), SetArgPointee<4>(512), Return(0)));
                                                                                      
    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(45), _, Eq(1000u))).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data, 45)), SetArgPointee<4>(45), Return(0)));

    result = pI2CMaster->Write(slaveAddress, registerAddress, msgLength, msg);

    ASSERT_EQ(0x00, data[0]);
    ASSERT_EQ(0xDC, data[1]);
    ASSERT_EQ(0x47, data[2]);
    ASSERT_EQ(0x24, data[3]);
    ASSERT_EQ(0x00, data[4]);
    ASSERT_EQ(0x00, data[5]);
    ASSERT_EQ(0x00, data[6]);
    ASSERT_EQ(0x00, data[7]);
    ASSERT_EQ(0x00, data[8]);
    ASSERT_EQ(0x59, data[9]);
    ASSERT_EQ(0x59, data[10]);
    ASSERT_EQ(0x59, data[43]);
    ASSERT_EQ(0x59, data[44]);
    ASSERT_EQ(0xAA, data[45]);

    ASSERT_EQ(VIPER_SUCCESS, result);
}

TEST_F(ViperboardI2CMasterWriteTest, WriteIncorrectLengthReturnsInvalidParameter)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t msg[2048] = {0x59};
    uint16_t msgLength = 2048;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    memset(msg, 0x59, 2048);

    result = pI2CMaster->Write(slaveAddress, registerAddress, 2049, msg);

    ASSERT_EQ(VIPER_INVALID_PARAMETER, result);
}

TEST_F(ViperboardI2CMasterWriteTest, WriteIncorrectNrBytesTransactionFailure)
{
    ViperResult_t result = VIPER_OTHER_ERROR;
    II2C_Master* pI2CMaster = pViper->GetI2CMasterInterface();
    
    uint8_t data[300] = {0xAA};
    
    uint8_t msg[300] = {0x59};
    uint16_t msgLength = 213;
    uint8_t slaveAddress = 0x48;
    uint8_t registerAddress = 0x12;

    uint16_t transferLength = msgLength+9;
    int bytesTransferred = 0;


    memset(msg, 0x59, 300);
    memset(data, 0xAA, 300);

    EXPECT_CALL(*pLibUsbMock, bulk_transfer(_, Eq(0x02), _, Eq(transferLength), _, Eq(1000u))).WillOnce(DoAll(WithArg<2>(SaveArrayPointee(data, transferLength)), SetArgPointee<4>(77),Return(0)));

    result = pI2CMaster->Write(slaveAddress, registerAddress, msgLength, msg);

    ASSERT_EQ(VIPER_TRANSACTION_FAILURE, result);
}

