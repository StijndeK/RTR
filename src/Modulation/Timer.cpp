#include "Timer.h"

void Timer::setLength(float timeInMs)
{
	addValue = (1.0 / 60) * (1 / (timeInMs / 1000.0));
}

void Timer::setFunctionToCall(void(&functionToCall)())
{
	_functionToCall = functionToCall;
}

// returns 0 untill value is 1
void Timer::timerTick()
{
	if (currentTick < 1) {
		currentTick += addValue;
	}
	else {
		currentTick = 0;
		(*_functionToCall)();
	}
}
