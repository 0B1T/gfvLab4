#include "ui.h"

int handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {   
        case '1' :
        {
            return 50;
        }
        break;
    }
    
    return 0;
}