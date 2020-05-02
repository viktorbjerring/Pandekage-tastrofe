/*
 * board.c
 *
 * Created: 19/04/2020 16.44.25
 *  Author: Mikkel
 */ 

#include "board.h"

//Global variables
volatile bool heat_on_ctrl = false;		//Control unit heat on/off variable

volatile bool heat_ok_pan1 = false;			//Heat level variable

volatile bool heat_ok_pan2 = false;

volatile bool heat_ok = false;

volatile bool pancake_done = false;		//Internal pancake done

volatile uint16_t pan1_cooking_time = 0;		//Current cooking time of pancake on pan 1

volatile uint16_t pan2_cooking_time = 0;		//Current cooking time of pancake on pan 2

volatile bool pan1Free = true;			//Pan 1 status

volatile bool pan2Free = true;			//Pan 2 status