

#pragma once

#include <qstring.h>

enum class SimulationResult {

	Success,
	Error_NoGroundNode,
	Error_NoComponents,

	Error_NewtonFailed

};


QString getSimulationErrorMessage(SimulationResult result);