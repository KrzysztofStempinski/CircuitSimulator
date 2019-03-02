#include "SimulationErrors.h"

QString getSimulationErrorMessage(SimulationResult result) {

	switch (result) {

		case SimulationResult::Error_NoGroundNode:
			return QString("no ground node found");
		break;

		case SimulationResult::Error_NoComponents:
			return QString("no components present in the circuit");
		break;

		case SimulationResult::Error_InvalidMode:
			return QString("invalid (or unimplemented?) mode of operation");
		break;

		case SimulationResult::Error_NewtonTimedOut:
			return QString("Newton's method failed to converge");
			break;

		// should never happen
		default:
			return QString("unknown error.");
		break;

	}

}