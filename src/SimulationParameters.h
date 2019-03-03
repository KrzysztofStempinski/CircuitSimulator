#pragma once

#include "Component.h"

enum class SimulationMode {

	DCOP,
	DCSweep

};

struct SimulationParameters {

	SimulationMode mode;

	Component* componentToSweep;
	QString propertyToSweep;
	double start, end, delta;

};
