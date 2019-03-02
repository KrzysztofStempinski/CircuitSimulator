#include "Circuit.h"

#include "MatrixHelper.h"

void Circuit::_markAdjacentNodes(Node* nodeBegin, const int voltageIndex) {

	nodeBegin->voltageIndex = voltageIndex;

	for (const auto &it : nodeBegin->connectedNodes)
		if (it->voltageIndex == -1) // if a node wasn't visited yet
			_markAdjacentNodes(it, voltageIndex);

}

SimulationResult Circuit::_prepareDCOP() {

	// sanity check
	if (components.empty())
		return SimulationResult::Error_NoComponents;

	// cleanup
	for (auto& it : nodes) {
		it->voltageIndex = -1;
		it->voltageValue  = 0;
	}

	// first pass
	bool foundGround = false;
	for (const auto& it : nodes) {
		if (it->voltageIndex == -1) {
			if (it->isCoupled() && it->getCoupledComponent()->getName() == QString("ground")) {// TODO actually implement checking whether this is ground from config file
				_markAdjacentNodes(it, 0);
				//it->voltageValue = 0;// perhaps node->isGround() ???	
				foundGround = true;
			}
		}
	}		

	if (!foundGround)
		return SimulationResult::Error_NoGroundNode;
																	
	int voltageIndex = 0;
	for (auto& it : nodes) {
		if (it->voltageIndex == -1) {
			voltageIndex++;
			_markAdjacentNodes(it, voltageIndex);
		}
	}

	voltageCount = voltageIndex;

	int currentIndex = 0;
	for (const auto& it : components) {

		if (it->requiresCurrentEntry()) {
			currentIndex++;
			it->currentIndex = currentIndex;
		}

	}

	currentCount = currentIndex;

	return SimulationResult::Success;

}

SimulationResult Circuit::simulate() {

	logWindow->log("Starting simulation...");

	SimulationResult preparationResult = _prepareDCOP();
	if (preparationResult != SimulationResult::Success)
		return preparationResult;

	Eigen::MatrixXd matrixA(voltageCount + currentCount, voltageCount + currentCount);
	Eigen::VectorXd matrixB(voltageCount + currentCount);
	Eigen::VectorXd solutions;

	int itno = 0;
	int MAX = 50;

	while (itno < MAX) {

		itno++;

		matrixA.fill(0);
		matrixB.fill(0);

		for (auto &it : components)
			it->applyComponentStamp(matrixA, matrixB, voltageCount);
		//	it->prepareForSimulation(voltageCount);

		solutions = matrixA.partialPivLu().solve(matrixB);

		logWindow->log("Solutions at iteration " + QString::number(itno) + "=" + vectorToString(solutions), LogEntryType::Debug);

		for (auto& it : nodes)
			if (it->voltageIndex >= 1)
				it->voltageValue = solutions(it->voltageIndex - 1);
			else
				it->voltageValue = double(0);

		for (auto& it : components)
			if (it->requiresCurrentEntry())
				it->currentValue = solutions((voltageCount - 1) + it->currentIndex);


	}
		// print out solutions
		logWindow->log("Matrix A = " + matrixToString(matrixA), LogEntryType::Debug);
		logWindow->log("Matrix B = " + matrixToString(matrixB), LogEntryType::Debug);

		logWindow->log("Solutions = " + vectorToString(solutions), LogEntryType::Debug);

	return SimulationResult::Success;

}
