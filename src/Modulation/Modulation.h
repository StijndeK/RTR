#pragma once
#include "ofMain.h"
#include "ModulationTypes.h"

class Modulation : public ModulationTypes
{
public:
	float CalculateModulation(float currentDistanceToGetToInRange, int trigger);
	void CalculateStepSize(float attackUpMs, float attackDownMs, float releaseMs);

	float currentDistance = 0;
	float upStep;
	float downStep;
	float release;
	float updateRate = 60.f;
	double amplitudeStartValue = 0.01;

	modulationType modType = exponential;
private:

};

