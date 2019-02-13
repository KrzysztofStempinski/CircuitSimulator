#pragma once

#include "..//eigen//Dense"

#include "Circuit.h"
#include "LogWindow.h"
#include "SimulationErrors.h"
#include "MatrixHelper.h"

SimulationResult DCOP(Circuit& circuit, Eigen::VectorXd& solutions, LogWindow* logWindow) {

	// assemble MNA matrices
	Eigen::MatrixXd matrixA(circuit.voltageCount + circuit.currentCount, circuit.voltageCount + circuit.currentCount);
	Eigen::VectorXd matrixB(circuit.voltageCount + circuit.currentCount);

	matrixA.fill(0);
	matrixB.fill(0);

	for (auto &it : circuit.components)
		it->applyComponentStamp(matrixA, matrixB, circuit.voltageCount, logWindow);

	solutions = matrixA.partialPivLu().solve(matrixB);

	double relativeError = (matrixA * solutions - matrixB).norm() / matrixB.norm();
	logWindow->log("Relative error: " + QString::number(relativeError));

	logWindow->log("Solutions = " + vectorToString(solutions), LogEntryType::Debug);

	// save solutions etc. 
	// TODO write actually decent comments

	for (auto& it : circuit.nodes) {
		if (it->voltageIndex >= 1) {
			it->voltageValue = static_cast<double>(solutions(it->voltageIndex - 1));
		}
		else
			it->voltageValue = double(0);
	}

	for (auto& it : circuit.components)
		if (it->requireCurrentEntry())
			it->currentValue = static_cast<double>(solutions((circuit.voltageCount - 1) + it->currentIndex));

	return SimulationResult::Success;

}
