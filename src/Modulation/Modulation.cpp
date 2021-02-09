#include "Modulation.h"

//--------------------------------------------------------------
// updown Modulation Base
//--------------------------------------------------------------

void UpDownModulationBase::CalculateAttackStepSize(float attackUpSec) {
	if (modType == linear) {
		upStep = (1.0 / updateRate) * (1.0 / attackUpSec);
		cout << "upstep linear" << downStep << endl;
	}
	else {
		upStep = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * attackUpSec));
		cout << "upstep: " << downStep << endl;
	}
}

void UpDownModulationBase::CalculateAttackDecreaseStepSize(float attackDownSec) {
	if (modType == linear) {
		downStep = 0 - ((1.0 / updateRate) * (1.0 / attackDownSec));
		cout << "downstep linear" << downStep << endl;
	}
	else {
		downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * attackDownSec));
		cout << "downstep: " << downStep << endl;
	}
}

//--------------------------------------------------------------
// Position Modulation
//--------------------------------------------------------------

float PositionModulation::CalculateModulation(float currentDistanceToGetToInRange, int trigger) {
	// attack stage
	if (trigger == 1) {
		float buffer = 0.01; // buffer because value might not get to exact goal because of step sizes
		if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer) {
			if (modType == linear) {
				currentDistance += (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
			else {
				if (currentDistance < amplitudeStartValue) { currentDistance = amplitudeStartValue; }; 
				currentDistance *= (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
		}
	}

	// release value is fixed, exponential, and not influenced by game data
	else {
		if (currentDistance > 0.01) {
			currentDistance *= release;
		}
	}

	return currentDistance;
}

void PositionModulation::CalculateReleaseStepSize(float releaseSec) {
	release = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * releaseSec));
}

//--------------------------------------------------------------
// Time Modulation
//--------------------------------------------------------------

// reduce intensity when trigger = 1
float TimeModulation::CalculateModulation(int trigger)
{
	if (trigger == 1) {
		if (currentDistance > range) {
			// calculate exp curve
			currentDistanceExp *= downStepExp;

			// calculate AC curve
			currentDistanceAc *= downStepAc;

			// set curve based on ratio between Ac and Exp
			currentDistance = ((1 - currentDistanceAc) * curveRatio) + (currentDistanceExp * (1 - curveRatio));
		}
	}

	else {
		currentDistance = 1;
		currentDistanceExp = 1;
		currentDistanceAc = amplitudeStartValue; 
	}

	return currentDistance;
}

void TimeModulation::CalculateStepSize(float stepSec)
{
	downStepExp = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * stepSec));
	downStepAc = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * stepSec));
}


//--------------------------------------------------------------
// Action Modulation
//--------------------------------------------------------------

ActionModulation::ActionModulation()
{
	currentDistance = 1;
}

float ActionModulation::CalculateModulation(float currentDistanceToGetToInRange, int trigger) {
	if (trigger == 1) {
		float buffer = 0.01;
		if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer) {
			if (modType == linear) {
				currentDistance += (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
			else {
				if (currentDistance < amplitudeStartValue) { currentDistance = amplitudeStartValue; };
				currentDistance *= (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
		}
	}


	else {
		currentDistance = 1;
	}

	return currentDistance;
}
