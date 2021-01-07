#pragma once

class ModulationData
{
public:
	ModulationData();
	~ModulationData();

	void MockData();

	float ConvertToDecimalData();

	float currentDistanceToGetTo = 10000;
	float positionGoal;
	float positionStart;
	float totalDistance;
private:

};

