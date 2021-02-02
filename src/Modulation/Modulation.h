#pragma once
#include "ofMain.h"
#include "ModulationTypes.h"

class Modulation : public ModulationTypes
{
public:
	float CalculateModulation(float currentDistanceToGetToInRange, int trigger);
	void CalculateAttackStepSize(float attackUpMs, float attackDownMs);
	void CalculateReleaseStepSize(float releaseMs);

	float currentDistance = 0;
	float updateRate = 60.f;
	double amplitudeStartValue = 0.001;

	modulationType modType = exponential;

private:
	float upStep;
	float downStep;
	float release;
};

