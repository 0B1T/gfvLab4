#include "twoComplementConverter.h"

float twoComplementConverter(int * dataPtr)
{
    float x = 0;
    
    if(* dataPtr < 256)
        x = (float) * dataPtr / 2.0f;
    else
        x = -((float)(~ * dataPtr + 1) / 2.0f + 256);
        
    return x;
}