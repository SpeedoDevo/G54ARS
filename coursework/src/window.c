#ifndef __SUB_WINDOW__
#define __SUB_WINDOW__

#include "header.h"

typedef struct {
	short state;
	short size;
	long* window;
} window;

void W(window* w, long* data, unsigned short size) {
	w->state = 0;
	w->size = size;
	w->window = data;
}

void put(window* w, long value) {
	w->window[w->state++] = value;
	w->state %= w->size;
}

float weight(window* w, float* weights) {
	float sum = 0;
	for(int i = 0; i < w->size; i++) {
		int j = (w->state + i) % w->size;
		sum += w->window[j] * weights[i];
	}

	return sum;
}

long minWindow(window* w) {
	float res = 10000;
	for(int i = 0; i < w->size; i++) {
		int j = (w->state + i) % w->size;
		res = min(w->window[j], res);
	}

	return res;
}

long maxWindow(window* w) {
	float res = 0;
	for(int i = 0; i < w->size; i++) {
		int j = (w->state + i) % w->size;
		res = max(w->window[j], res);
	}

	return res;
}

#endif //__SUB_WINDOW__
