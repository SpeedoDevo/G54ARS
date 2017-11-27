#pragma config(Sensor, S1,     gyro,           sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S2,     rightSonar,     sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     colorSensor,          sensorI2CCustom)
#pragma config(Sensor, S4,     leftSonar,      sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          right,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          left,          tmotorEV3_Large, PIDControl, encoder)

#ifndef __SUB_MAIN__
#define __SUB_MAIN__

#include "header.h"
#include "avoid.c"
#include "follow.c"
#include "forage.c"
#include "observe.c"

task main() {
	resetGyro(gyro);
	sleep(500);
	startTask(forage);
	startTask(follow);
	startTask(avoid);
	startTask(observe);
	while (true) {
		ResultTP ctrl = &results[iFollow];

		int j;
		for (int i = 0; i < behaviours; i++) {
			ResultTP res = &results[i];

			if (res->active) {
				ctrl = res;
				j = i;
			}
		}

		displayBigTextLine(0, "%d c: %d-%d", j, ctrl->left, ctrl->right);

		setMotorSpeed(left, ctrl->left);
		setMotorSpeed(right, ctrl->right);
		abortTimeslice();
	}
}

#endif //__SUB_MAIN__
