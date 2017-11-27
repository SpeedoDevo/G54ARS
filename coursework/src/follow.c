#ifndef __SUB_FOLLOW__
#define __SUB_FOLLOW__

#include "header.h"
#include "window.c"

const unsigned short windowSize = 10;
float weights[windowSize];

window lowWindow; long lowWindowData[windowSize];
window mainWindow; long mainWindowData[windowSize];
window highWindow; long highWindowData[windowSize];

float* youngest() {
	weights[0] = 0;
	weights[1] = 0;
	weights[2] = 0;
	weights[3] = 0;
	weights[4] = 0;
	weights[5] = 0;
	weights[6] = 0;
	weights[7] = 0.05;
	weights[8] = 0.15;
	weights[9] = 0.8;
	return &weights;
}

float* youngish() {
	weights[0] = 0.05;
	weights[1] = 0.05;
	weights[2] = 0.05;
	weights[3] = 0.05;
	weights[4] = 0.05;
	weights[5] = 0.05;
	weights[6] = 0.1;
	weights[7] = 0.1;
	weights[8] = 0.1;
	weights[9] = 0.4;
	return &weights;
}

void sampleLight() {
	long light = 0;
	HTCS2readRawWhite(colorSensor, true, light);
	put(&mainWindow, light);

	if (mainWindow.state == 0) {
		long lowCandidate = min(minWindow(&mainWindow), 400);
		if(abs( weight(&lowWindow, youngish()) - lowCandidate ) > 20) {
			put(&lowWindow, lowCandidate);
		}

		long highCandidate = max(maxWindow(&mainWindow), 500);
		if(abs( weight(&highWindow, youngish()) - highCandidate ) > 20) {
			put(&highWindow, highCandidate);
		}
	}
}

bool aroundLine;

task follow() {
	W(&lowWindow, &lowWindowData[0], windowSize);
	W(&mainWindow, &mainWindowData[0], windowSize);
	W(&highWindow, &highWindowData[0], windowSize);

	for(int i = 0; i < windowSize - 1; i++) {
		sampleLight();
	}
	for(int i = 0; i < windowSize; i++) {
		put(&lowWindow, 300);
		put(&highWindow, 800);
	}

	while (true) {
		ResultTP res = &results[iFollow];

		sampleLight();
		float white = weight(&mainWindow, youngest());

		long low = weight(&lowWindow, youngish());
		long high = weight(&highWindow, youngish());
		float threshold = (0.5 * low) + (0.5 * high);
		displayBigTextLine(2, "l: %d t: %d", white, threshold);

		aroundLine = white < threshold;
		if (aroundLine || time1[T1] < 5000) {
			if(aroundLine) {
				clearTimer(T1);
				control(res, speed * 2, 0);
			} else {
				control(res, 0, speed * 2);
			}
		} else {
			inactive(res);
		}

		abortTimeslice();
	}
}

#endif //__SUB_FOLLOW__
