#pragma once
#include "ofMain.h"

class Modulation
{
public:
	Modulation();
	~Modulation();

	void MockData();
	float CalculateModulation(float currentDistanceToGetToInRange);
	void CalculateStepSize(float totalAttackMs, float totalReleaseMs);

	float currentDistance = 0;
	float upStep;
	float downStep;
	float updateRate = 60.f;
	double amplitudeStartValue = 0.001;

	bool modType = true; // false = exponential, true = linear
private:

};

