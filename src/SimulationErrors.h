#pragma once

#include <qstring.h>

enum class SimulationResult {

	Success,
	Error_NoGroundNode,
	Error_NoComponents,
	Error_InvalidMode

};

QString getSimulationErrorMessage(const SimulationResult result);