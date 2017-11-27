#ifndef __SUB_AVOID__
#define __SUB_AVOID__

#include "header.h"
#include "follow.c"

task avoid() {
	typedef enum avoidState {
		detecting,
		turnAvoid,
		forwardAvoid,
		correctTurn,
		correctTurnUndo,
		correctForward,
		returnTurn,
		returnUndo,
		returnForward
	};

	int lowThreshold = 10;
	int highThreshold = 30;
	int turnAvoidExtra = 50;
	int forwardAvoidTime = 3000;
	float detectionTurnRatio = 0.7;
	int maxTurnCorrection = 1500;
	int turnUndoTime = 600;
	int correctForwardTime = 3000;
	int maxReturnTurn = 1000;
	int returnUndoTime = 300;
	int returnForwardTime = 1500;
	int direction;

	const int doubleSpeed = speed * 2;

	avoidState state = detecting;
	while(true) {
		ResultTP res = &results[iAvoid];
		int distL = getUSDistance(leftSonar);
		int distR = getUSDistance(rightSonar);
		displayBigTextLine(4, "%d cm %d cm", distL, distR);

		int dist;
		if(direction == 1) {
			dist = distL;
		} else if (direction == -1) {
			dist = distR;
		}

		switch (state) {
			case turnAvoid:
				if (dist < highThreshold || time1[T2] < turnAvoidExtra) {
					control(res, -direction * doubleSpeed, direction * doubleSpeed);
					if (dist < highThreshold) {
						clearTimer(T2);
					}
				} else {
					state = forwardAvoid;
					clearTimer(T2);
				}
			break;


			case forwardAvoid:
				if (time1[T2] < forwardAvoidTime) {
					control(res, doubleSpeed, doubleSpeed);
				} else {
					state = correctTurn;
					clearTimer(T2);
				}
			break;


			case correctTurn:
				if (dist > highThreshold && time1[T2] < maxTurnCorrection) {
					control(res, direction * doubleSpeed * detectionTurnRatio, -direction * doubleSpeed * detectionTurnRatio);
				} else {
					state = correctTurnUndo;
					clearTimer(T2);
				}
			break;

			case correctTurnUndo:
				if (time1[T2] < turnUndoTime) {
					control(res, -direction * doubleSpeed, direction * doubleSpeed);
				} else {
					state = correctForward;
					clearTimer(T2);
				}
			break;

			case correctForward:
				if (time1[T2] < correctForwardTime) {
					control(res, doubleSpeed, doubleSpeed);
				} else {
					state = returnTurn;
					clearTimer(T2);
				}
			break;

			case returnTurn:
				if (dist > highThreshold && time1[T2] < maxReturnTurn) {
					control(res, direction * doubleSpeed * detectionTurnRatio, -direction * doubleSpeed * detectionTurnRatio);
				} else {
					state = returnUndo;
					clearTimer(T2);
				}
			break;

			case returnUndo:
				if (time1[T2] < returnUndoTime) {
					control(res, -direction * doubleSpeed, direction * doubleSpeed);
				} else {
					state = returnForward;
				}
			break;

			case returnForward:
				if(time1[T2] > returnForwardTime / 2 && aroundLine) {
					state = detecting;
				} else if(time1[T2] < returnForwardTime) {
					control(res, doubleSpeed, doubleSpeed);
				} else {
					state = detecting;
				}
			break;

			default:
			case detecting:
				if (distL < lowThreshold || distR < lowThreshold) {
					if (distL <= distR) {
						state = turnAvoid;
						direction = 1; //right
					} else {
						state = turnAvoid;
						direction = -1; //left
					}
				}

				inactive(res);
				break;
		}

		abortTimeslice();
	}
}

#endif //__SUB_AVOID__
