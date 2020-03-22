/* ========================================
 *
 * Copyright De Engagerede Mænd, 2020
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdio.h>
#include "project.h"
#include "../../../Software/UltrasonicSensor.h"

char buffer[1024];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    UART_1_Start();
    Ultrasonic.Init();

    for(;;)
    {
        Ultrasonic.ReadDist();
        CyDelay(50);
        sprintf(buffer, "Dist:\t%.2f\n\r", Ultrasonic.dist_);
        UART_1_PutString(buffer);
    }
}

/* [] END OF FILE */
