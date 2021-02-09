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

class UpDownModulationBase : public ModulationBase, public ModulationTypes
{
public:
	void CalculateAttackStepSize(float attackUpSec);
	void CalculateAttackDecreaseStepSize(float attackDownSec);

	modulationType modType = exponential;

	float upStep;
	float downStep;
};

class PositionModulation : public UpDownModulationBase, public ModulationTypes
{
public:
	float CalculateModulation(float currentDistanceToGetToInRange, int trigger);
	void CalculateReleaseStepSize(float releaseSec);

private:
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
	float range = 0.3;
};

class ActionModulation : public UpDownModulationBase, public ModulationTypes
{
public:
	float CalculateModulation(float currentDistanceToGetToInRange, int trigger);
};