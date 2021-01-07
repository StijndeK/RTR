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
		if (currentDistance < currentDistanceToGetToInRange) {	// add
			currentDistance = currentDistance + upStep;
		}
		else {													// remove
			currentDistance = currentDistance - downStep;
		}
	}

	return currentDistance;
}

void Modulation::CalculateStepSize(float totalAttackMs, float totalReleaseMs) {
	upStep = (1.0 / updateRate) * (1 / (totalAttackMs / 1000.0));
	downStep = (1.0 / updateRate) * (1 / (totalReleaseMs / 1000.0));
}