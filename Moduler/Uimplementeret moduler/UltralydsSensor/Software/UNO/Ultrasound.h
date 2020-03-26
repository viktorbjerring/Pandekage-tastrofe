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

class Ultrasound
{
private:
	static float dist_;
	static uint8_t count_;
	static bool isEcho_;

public:
	static void StartReading();
	
	static void Init();
	static void Interrupt();
	
	static float GetDist() { return dist_; }
	static uint8_t GetCount() { return count_; }
};

#endif //__ULTRASOUND_H__
