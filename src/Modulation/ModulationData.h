#pragma once

class ModulationData
{
public:
	void mockData();
	float convertToDecimalData(float position);
	float currentDistanceToGetTo = 0;

private:
	float positionGoal;
	float positionStart;
	float totalDistance;
};

