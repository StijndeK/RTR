#include "ActionCalculator.h"

void ActionCalculator::setTreshold(float timeInMs)
{
	threshold = timeInMs;
}

void ActionCalculator::setFunctionToCall(void(&functionToCall)())
{
	_functionToCall = functionToCall;
}

void ActionCalculator::update(float currentValue)
{
	if (calculateAction) {

	}
	// if vector < 500 
	//		add current value to vector
	// if vector > 500
	//		overwrite oldest value of vector
	//		do the check to get a 0 1 rating of how similar new value is to previous values
	//		if this similarity is above threshold. call the function and  return the rating for the distancetogettovalue of the modulator
}

void ActionCalculator::startActionCalculator()
{
	values.clear();
	calculateAction = true;
}

void ActionCalculator::stopActionCalculator()
{
	calculateAction = false;
}
