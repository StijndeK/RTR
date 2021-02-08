#pragma once
#include "ofMain.h"

class ThresholdChecker
{
public:
	void setFunctionToCall(void(&functionToCall)());
	void (*_functionToCall)();
};

class ActionCalculator : public ThresholdChecker
{
public:
	// base treshold checker functions
	void setTreshold(float timeInMs);

	// own functions
	void update(float currentValue);
	void startActionCalculator();
	void stopActionCalculator();
	bool calculateAction = false;

private:
	std::vector<float> values;
	std::vector<float> values2;
	int currentVecIt = 0;
	float threshold = 0;
	int sampleSize = 200;
};


class Timer : public ThresholdChecker
{
public:
	void setLength(float timeInMs);
	void update();
	void startTimer();
	void stopTimer();

	bool looping = false;
private:
	float currentTick = 0;
	float addValue;
};

