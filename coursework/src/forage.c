#ifndef __SUB_FORAGE__
#define __SUB_FORAGE__

#include "header.h"

task forage() {
	while (true) {
		ResultTP res = &results[iForage];
		control(res, speed, speed);
		abortTimeslice();
	}
}

#endif //__SUB_FORAGE__
