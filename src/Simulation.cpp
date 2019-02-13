#include "Simulation.h"

#include "DCOP.h"

void markAdjacentNodes(Node* nodeBegin, int voltageIndex) {

	nodeBegin->voltageIndex = voltageIndex;

	for (const auto &it : nodeBegin->connectedNodes)
		if (it->voltageIndex == -1)
			markAdjacentNodes(it, voltageIndex);

}

SimulationResult prepareSimulation(Circuit& circuit, Eigen::VectorXd& solutions, LogWindow* logWindow) {

	// sanity check
	if (circuit.components.empty())
		return SimulationResult::Error_NoComponents;

	// cleanup
	for (auto& it : circuit.nodes) {
		it->voltageIndex = -1;
		it->voltageValue  = 0;
	}

	// first pass
	bool foundGround = false;
	for (const auto& it : circuit.nodes) {
		if (it->voltageIndex == -1) {
			if (it->isCoupled() && it->getCoupledComponent()->getName() == QString("ground")) {// TODO actually implement checking whether this is ground from config file
				markAdjacentNodes(it, 0);
				//it->voltageValue = 0;// perhaps node->isGround() ???	
				foundGround = true;
			}
		}
	}		

	if (!foundGround)
		return SimulationResult::Error_NoGroundNode;
																	
	int voltageIndex = 0;
	for (auto& it : circuit.nodes) {
		if (it->voltageIndex == -1) {
			voltageIndex++;
			markAdjacentNodes(it, voltageIndex);
		}
	}

	circuit.voltageCount = voltageIndex;

	int currentIndex = 0;
	for (const auto& it : circuit.components) {

		if (it->requireCurrentEntry()) {
			currentIndex++;
			it->currentIndex = currentIndex;
		}

		it->logWindow = logWindow;
		it->prepareForSimulation(circuit.voltageCount);

	}

	circuit.currentCount = currentIndex;

	return SimulationResult::Success;

}

SimulationResult simulate(Circuit& circuit, SimulationMode mode, Eigen::VectorXd& solutions, LogWindow* logWindow){

	logWindow->log("Starting simulation...");

	SimulationResult result = prepareSimulation(circuit, solutions, logWindow);

	if (result != SimulationResult::Success)
		return result;

	switch (mode) {
	
		case SimulationMode::DCOP: 
			return DCOP(circuit, solutions, logWindow);
		break;
	
	}

}