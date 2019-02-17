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

	// compute the error
	double relativeError = (matrixA * solutions - matrixB).norm() / matrixB.norm();
	logWindow->log("Relative error: " + QString::number(relativeError));

	// print out solutions
	logWindow->log("Solutions = " + vectorToString(solutions), LogEntryType::Debug);

	for (auto& it : circuit.nodes) 
		if (it->voltageIndex >= 1) 
			it->voltageValue = solutions(it->voltageIndex - 1);
		else
			it->voltageValue = double(0);


	for (auto& it : circuit.components)
		if (it->requireCurrentEntry())
			it->currentValue = solutions((circuit.voltageCount - 1) + it->currentIndex);

	return SimulationResult::Success;

}
