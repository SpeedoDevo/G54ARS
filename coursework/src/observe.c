#ifndef __SUB_OBSERVE__
#define __SUB_OBSERVE__

#include "header.h"

typedef struct {
	float x;
	float y;
	float th;
} position;

void Pose(position* p, float x, float y, float th) {
	p->x = x;
	p->y = y;
	p->th = th;
}

void Pose(position* p, position* q) {
	Pose(p, q->x, q->y, q->th);
}

position origin;
position pose;

const int mapSize = 300;
position graph[mapSize];
int graphPosition;

void push(position* p) {
	Pose(&graph[graphPosition++], p);
}

position* lastLogged() {
	return &graph[graphPosition-1];
}

float distance(position* p, position* q) {
	return sqrt(pow(p->x - q->x, 2) + pow(p->y - q->y, 2));
}

const float interval = 10;
void updatePose() {
	static const float L = 14.5; //10 - 14.5
	static const float R = 2.8;
	static const float perSec = interval / 1000.0;
	static const float degreesToVelocity = (PI/180.0) / perSec * R;

	static int prevLeft = 0;
	static int prevRight = 0;

	int leftEncoder = getMotorEncoder(left);
	int rightEncoder = getMotorEncoder(right);

	int deltaEncoderLeft = leftEncoder - prevLeft;
	int deltaEncoderRight = rightEncoder - prevRight;

	float vLeft = deltaEncoderLeft * degreesToVelocity;
	float vRight = deltaEncoderRight * degreesToVelocity;
	float v = (vLeft + vRight) / 2;
	float omega = (vLeft - vRight) / L;

	float dx = v * cos(pose.th) * perSec;
	float dy = v * sin(pose.th) * perSec;
	float dth = omega * perSec;

	pose.x += dx;
	pose.y += dy;
	pose.th += dth;

	//gyro correction
	long deg = getGyroDegrees(gyro);
	float gyroTh = deg * -0.0174533;
	pose.th = 0.3 * pose.th + 0.7 * gyroTh;

	prevLeft = leftEncoder;
	prevRight = rightEncoder;
}

task observe() {
	typedef enum state {
		exploring,
		locating,
		stopping
	};
	state S = exploring;

	ResultTP res = &results[iObserve];

	Pose(&origin, 0, 0, 0);
	Pose(&pose, &origin);


	graphPosition = 0;
	push(&pose);

	while (true) {
		updatePose();
		displayBigTextLine(6, "%d %d %1.1f", pose.x, pose.y, pose.th);

		switch(S) {
			case exploring:
				static bool leftOrigin = false;
				if(distance(&graph[1], &pose) > 30) {
					leftOrigin = true;
				}

				if((PI / 12) < abs(lastLogged()->th - pose.th)) {
					push(&pose);

					if(leftOrigin && distance(&origin, &pose) < 20) {
						push(&origin);
						S = locating;
					}
				}
				inactive(res);
			break;
			case locating:
				static position target;
				static bool haveTarget = false;
				displayBigTextLine(8, "%d %d", target.x, target.y);
				if (!haveTarget) {
					float dist = 0;
					for(int i = 1; i < graphPosition; i++) {
						float thisDist = distance(&graph[i-1], &graph[i]);

						if(thisDist > dist) {
							dist = thisDist;
							Pose(&target, (graph[i-1].x + graph[i].x) / 2, (graph[i-1].y + graph[i].y) / 2, 0);
						}
					}
					haveTarget = true;
				} else if (distance(&target, &pose) < 20) {
					S = stopping;
				}
				inactive(res);
			break;
			case stopping:
				control(res, 0, 0);
				displayBigTextLine(10, "done");
			break;
		}

		sleep(interval);
	}
}

#endif //__SUB_OBSERVE__
