
#include "SimulationErrors.h"

QString getSimulationErrorMessage(SimulationResult result) {

	switch (result) {

	case SimulationResult::Error_NoGroundNode:
		return QString("no ground node found");
		break;

	case SimulationResult::Error_NoComponents:
		return QString("no components present in the circuit");
		break;

	case SimulationResult::Error_NewtonFailed:
		return QString("Newton's iterations failed to converge");
		break;

	default:
		return QString("Unknown error.");
		break;
	}

}