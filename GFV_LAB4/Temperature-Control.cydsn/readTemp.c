#include "readTemp.h"

static uint8_t status = 0;
static uint8_t readByte1 = 0;
static uint8_t readByte2 = 0;

int readTemp(uint8_t addr)
{
    status = I2C_MasterSendStart(addr, 1);

    if (I2C_MSTR_NO_ERROR == status)
    {
        readByte1 = I2C_MasterReadByte(I2C_ACK_DATA);
        readByte2 = I2C_MasterReadByte(I2C_NAK_DATA);
    }

    I2C_MasterSendStop();
    
    return (readByte2 >> 7) | (readByte1 << 1);
}