#include "Modulation.h"

Modulation::Modulation()
{
}

Modulation::~Modulation()
{
}


float Modulation::CalculateModulation(float currentDistanceToGetToInRange) {
	// set output value
	float buffer = 0.01; // buffer because value might not get to exact goal because of step sizes
	if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer) {
		if (modType == linear) {
			if (currentDistance < currentDistanceToGetToInRange) {	// add
				currentDistance += upStep;
			}
			else {													// remove
				currentDistance += downStep;
			}
		}
		else {
			currentDistance += amplitudeStartValue; // make sure multiplyvalue != 0 TODO: do automatically

			if (currentDistance < currentDistanceToGetToInRange) {	// add
				currentDistance *= upStep;
			}
			else {													// remove
				currentDistance *= downStep;
			}
		}
	}

	return currentDistance;
}

void Modulation::CalculateStepSize(float totalAttackMs, float totalReleaseMs) {
	if (modType == linear) {
		upStep = (1.0 / updateRate) * (1 / (totalAttackMs / 1000.0));
		downStep = (1.0 / updateRate) * (1 / (totalReleaseMs / 1000.0));
	}
	else {
		upStep = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * (totalAttackMs / 1000.0)));
		downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * (totalReleaseMs / 1000.0)));
		cout << upStep << endl;
	}
}