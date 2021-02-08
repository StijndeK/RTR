#include "ThresholdChecker.h"


void ThresholdChecker::setFunctionToCall(void(&functionToCall)())
{
	_functionToCall = functionToCall;
}

//--------------------------------------------------------------
// Action Threshold
//--------------------------------------------------------------

void ActionCalculator::setTreshold(float timeInMs)
{
	threshold = timeInMs;
}

void ActionCalculator::update(float currentValue)
{
	if (calculateAction) {
		if (values.size() < 100 || values2.size() < 100) {
			if (values.size() < 100) {
				values.push_back(currentValue);
			}
			else {
				values2.push_back(currentValue);
			}
		}
		else {
			cout << "trigger action modulation" << endl;

			// calculate average
			float average = accumulate(values.begin(), values.end(), 0.0) / 100;
			float average2 = accumulate(values2.begin(), values2.end(), 0.0) / 100;

			cout << "average 1: " << average << endl;
			cout << "average 2: " << average2 << endl;

			// check deviation with current average
			if (average2 >= average - threshold || average2 <= average + threshold) {
				// call the function
				//(*_functionToCall)();
			}

			// clear vectors to restart count 
			// TODO: start replacing values and have a constant check
			values.clear();
			values2.clear();

			//values[currentVecIt] = currentValue;
			//currentVecIt = (currentVecIt + 1) % 100;
		}
	}

	// TODO: if this similarity is above threshold. call the function and  return the rating for the distancetogettovalue of the modulator
}

void ActionCalculator::startActionCalculator()
{
	values.clear();
	currentVecIt = 0;
	calculateAction = true;
}

void ActionCalculator::stopActionCalculator()
{
	calculateAction = false;
}

//--------------------------------------------------------------
// Timer Threshold
//--------------------------------------------------------------

void Timer::setLength(float timeInMs)
{
	addValue = (1.0 / 60) * (1 / timeInMs);
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