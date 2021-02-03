#pragma once
#include "ofMain.h"

class Timer
{
public:
	void setLength(float timeInMs);
	void setFunctionToCall(void (&functionToCall)());
	void timerTick();

	float currentTick = 0;
	float addValue;
	void (*_functionToCall)();
};

