#pragma once
#include "ofMain.h"

class Timer
{
public:
	void setLength(float timeInMs);
	void setFunctionToCall(void (&functionToCall)());
	void update();
	void startTimer();

	void stopTimer();

	bool looping = false;
private:
	float currentTick = 0;
	float addValue;
	void (*_functionToCall)();
};

