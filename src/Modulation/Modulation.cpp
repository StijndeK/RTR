#include "Modulation.h"

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
				if (currentDistance < amplitudeStartValue) { currentDistance = amplitudeStartValue; }; // make sure multiplyvalue != 0 TODO: do automatically
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

void PositionModulation::CalculateAttackStepSize(float attackUpSec) {
	if (modType == linear) {
		upStep = (1.0 / updateRate) * (1.0 / attackUpSec);
	}
	else {
		upStep = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * attackUpSec));
	}
}

void PositionModulation::CalculateAttackDecreaseStepSize(float attackDownSec) {
	if (modType == linear) {
		downStep = 0 - ((1.0 / updateRate) * (1.0 / attackDownSec));
	}
	else {
		downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * attackDownSec));
	}
}


void PositionModulation::CalculateReleaseStepSize(float releaseSec) {
	release = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * releaseSec));
}

//--------------------------------------------------------------
// Time Modulation
//--------------------------------------------------------------

// reduce intensity when trigger = 1
float TimeModulation::CalculateModulation(float currentDistanceToGetToInRange, int trigger)
{
	if (trigger == 1) {
		if (currentDistance > 0.01) {
			currentDistance *= downStep;
		}
	}

	else {
		currentDistance = 1;
	}

	return currentDistance;
}

void TimeModulation::CalculateStepSize(float stepSec)
{
	// TODO: reverse the curve. 
	downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * stepSec));
}