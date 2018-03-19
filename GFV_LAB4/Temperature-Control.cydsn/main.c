#include <stdio.h>
#include "project.h"
#include "PIDControl.h"
#include "readTemp.h"
#include "twoComplementConverter.h"
#include "ui.h"

static char outputBuffer[256];
static float setPoint = 30; // degrees celcius

#define SAMPLES_PER_SECOND 3
static uint16_t sampleWaitTimeInMilliseconds = 1000 / SAMPLES_PER_SECOND;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    float Kp = 2.0f;
    float Ki = 1.0f/30.0f;
    float Kd = 0.0f;
    float integralMax = 3000;
    float integralMin = -3000;
    float temp = 0;
    int tempStore = 0;
    float controlSignal = 0;

    float dt = ((float)sampleWaitTimeInMilliseconds) / 1000; // dt is measured in seconds
    PIDControl_init(Kp, Ki, Kd, integralMax, integralMin, dt);
    PIDControl_changeSetPoint(setPoint);
    
    // HW initialization:
    I2C_Start();
    UART_Start();
    PWM_Start();

    // Terminal welcome message:
    UART_PutString("Temperature control application started\r\n");

    for(;;)
    {
        // LM75_1:
        tempStore = readTemp(0x48);
        temp = twoComplementConverter(&tempStore);
        
        float error = setPoint - temp;
        float proportionalPart = 0;
        float integralPart = 0;
        float derivativePart = 0;
        
        controlSignal = PIDControl_doStep(temp, &proportionalPart, &integralPart, &derivativePart);            
        snprintf(outputBuffer, sizeof(outputBuffer), 
            "%.0f, %.1f, %.1f, %.6f, %.1f, %.3f, %.1f, %.1f, %.6f, %.1f \r\n", 
            setPoint, temp, error, controlSignal, Kp, Ki, Kd, 
            proportionalPart, integralPart, derivativePart);

        // Controls PWM based upon controlSignal:
        if(controlSignal < 0) controlSignal = 0;
        if(controlSignal > 100) controlSignal = 100;
        PWM_WriteCompare(controlSignal);
        
        UART_PutString(outputBuffer);

        CyDelay(sampleWaitTimeInMilliseconds);
    }
}