/* 
* Ultrasound.h
*
* Created: 23/03/2020 21:35:48
* Author: oskar
*/


#ifndef __ULTRASOUND_H__
#define __ULTRASOUND_H__

#include <stdint.h>

#define REGRESSION(x) (0.2779 * x - 0.2373)

//extern int8_t dist_;
//extern bool isEcho_;
//extern bool readingReady_;

int getBatterLevel();
void Init();
void Interrupt();

#endif //__ULTRASOUND_H__
