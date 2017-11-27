#ifndef __SUB_HEADER__
#define __SUB_HEADER__

#include "HitechnicColorSensor.h"

typedef struct {
	int left;
	int right;
	bool active;
} ResultT, *ResultTP;

const int behaviours = 4;
const int iForage = 0;
const int iFollow = 1;
const int iAvoid = 2;
const int iObserve = 3;

const int speed = 10;

ResultT results[behaviours];

#include "util.c"

#endif //__SUB_HEADER__
