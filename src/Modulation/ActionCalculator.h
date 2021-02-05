#pragma once
#include <vector>

class ActionCalculator
{
public:
	// base treshold checker functions
	void setTreshold(float timeInMs);
	void setFunctionToCall(void(&functionToCall)());

	// own functions
	void update(float currentValue);
	void startActionCalculator();
	void stopActionCalculator();
	bool calculateAction = false;
private:
	std::vector<float> values;
	void (*_functionToCall)();
	float threshold = 0;
};

