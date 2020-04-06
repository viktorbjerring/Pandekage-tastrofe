
#include <stdint.h>
#include "project.h"
#include "UltrasonicSensor.h"

CY_ISR(Falling_ISR_addr) {
    uint16_t counter_diff_ = Counter_1_ReadPeriod() - Counter_1_ReadCounter();
    float pw_us = 1000000 * counter_diff_ / (float)CLOCK_FREQ;
    Ultrasonic.dist_ = REGRESSION(pw_us);
    Echo_ClearInterrupt();
}

void Init(void) {
    Ultrasonic.dist_ = 0;
    
    Counter_1_Start();
    PWM_1_Start();
    Falling_ISR_StartEx(Falling_ISR_addr);
}

void ReadDist(void) {
    Trig_Write(1);
    CyDelayUs(10);
    Trig_Write(0);
}

struct Ultrasonic Ultrasonic = {
    .Init = Init,
    .ReadDist = ReadDist
};