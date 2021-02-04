#pragma once
#include "ofMain.h"
#include "ModulationTypes.h"

class ModulationBase
{
public:
	float updateRate = 60.f;
	double amplitudeStartValue = 0.001;
	float currentDistance = 0;
};

class PositionModulation : public ModulationBase, public ModulationTypes
{
public:
	float CalculateModulation(float currentDistanceToGetToInRange, int trigger);
	void CalculateAttackStepSize(float attackUpSec);
	void CalculateAttackDecreaseStepSize(float attackDownSec);
	void CalculateReleaseStepSize(float releaseSec);

	modulationType modType = exponential;

private:
	float upStep;
	float downStep;
	float release;
};

class TimeModulation : public ModulationBase
{
public:
	float CalculateModulation(int trigger);
	void CalculateStepSize(float stepSec);

private:
	float currentDistanceExp = 0;
	float currentDistanceAc = 1;
	float curveRatio = 0.5;
	float downStepExp;
	float downStepAc;
};