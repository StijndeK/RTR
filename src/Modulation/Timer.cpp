#include "Timer.h"

void Timer::setLength(float timeInMs)
{
	addValue = (1.0 / 60) * (1 / timeInMs);
}

void Timer::setFunctionToCall(void(&functionToCall)())
{
	_functionToCall = functionToCall;
}

// returns 0 untill value is 1
void Timer::update()
{
	if (currentTick != 2) {
		if (currentTick < 1) {
			// update tick;
			currentTick += addValue;
		}
		else {
			// call the function
			(*_functionToCall)();

			// check if looping or timer finished
			if (looping) {
				startTimer();
			}
			else {
				stopTimer();
			}
		}
	}
}

void Timer::startTimer()
{
	currentTick = 0;
}

void Timer::stopTimer()
{
	currentTick = 2;
}