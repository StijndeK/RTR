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

void PositionModulation::CalculateAttackStepSize(float attackUpMs, float attackDownMs) {
	if (modType == linear) {
		upStep = (1.0 / updateRate) * (1 / (attackUpMs / 1000.0));
		downStep = 0 - ((1.0 / updateRate) * (1 / (attackDownMs / 1000.0)));
	}
	else {
		upStep = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * (attackUpMs / 1000.0)));
		downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * (attackDownMs / 1000.0)));
	}
}

void PositionModulation::CalculateReleaseStepSize(float releaseMs) {
	release = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * (releaseMs / 1000.0)));
}

//--------------------------------------------------------------
// Time Modulation
//--------------------------------------------------------------

float TimeModulation::CalculateModulation(float currentDistanceToGetToInRange, int trigger)
{
	if (trigger == 1) {
		if (currentDistance > 0.01) {
			currentDistance *= downStep;
		}
	}

	// if no decreasing of intensity is required because the time modulation threshold has not been reached yet
	else {
		currentDistance = 1;
	}

	return currentDistance;
}

void TimeModulation::CalculateStepSize(float stepMs)
{
	downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * (stepMs / 1000.0)));
}
