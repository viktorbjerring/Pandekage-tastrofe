#pragma once

#include <stdint.h>
#include "project.h"

#define CLOCK_FREQ 100000
#define REGRESSION(x) (x + 2.9862) / 55.128

struct Ultrasonic {
    float dist_;
    
    void (*Init)(void);
    void (*ReadDist)(void);
};

extern struct Ultrasonic Ultrasonic;