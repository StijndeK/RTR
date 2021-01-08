#pragma once
#include "ofMain.h"
#include "ModulationTypes.h"

class Modulation : public ModulationTypes
{
public:
	Modulation();
	~Modulation();

	float CalculateModulation(float currentDistanceToGetToInRange);
	void CalculateStepSize(float totalAttackMs, float totalReleaseMs);

	float currentDistance = 0;
	float upStep;
	float downStep;
	float updateRate = 60.f;
	double amplitudeStartValue = 0.001;

	modulationType modType = linear; // false = exponential, true = linear TODO: make enum
private:

};

