/*
 * board.c
 *
 * Created: 19/04/2020 16.44.25
 *  Author: Mikkel
 */ 

#include "board.h"

//Global variables
bool heat_on_ctrl = false;		//Control unit heat on/off variable

bool heat_ok = false;			//Heat level variable

bool pancake_done = false;		//Internal pancake done

uint16_t pan1_cooking_time = 0;		//Current cooking time of pancake on pan 1

uint16_t pan2_cooking_time = 0;		//Current cooking time of pancake on pan 2

bool pan1Free = true;			//Pan 1 status

bool pan2Free = true;			//Pan 2 status