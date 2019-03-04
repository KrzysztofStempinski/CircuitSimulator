#include "Circuit.h"

#include "MatrixHelper.h"

// TODO settings entry or sth like that?
constexpr int MAX_ITERATIONS = 400; // should be enough
constexpr double DELTA = 1e-6;

SimulationResult Circuit::_prepareDCOP() {

	// sanity check
	if (components.empty())
		return SimulationResult::Error_NoComponents;

	// cleanup
	for (auto& it : nodes) 
		it->reset();

	// first pass
	bool foundGround = false;
	for (const auto& it : nodes) {
		if (it->voltageIndex == -1) {
			if (it->isCoupled() && it->getCoupledComponent()->getName() == QString("ground")) {// TODO actually implement checking whether this is ground from config file

				it->markAdjacentNodes(0);

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
			it->markAdjacentNodes(voltageIndex);
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

	Eigen::MatrixXd matrixA_linear(voltageCount + currentCount, voltageCount + currentCount);
	Eigen::MatrixXd matrixA_nonlinear(voltageCount + currentCount, voltageCount + currentCount);
	Eigen::VectorXd matrixB_linear(voltageCount + currentCount);
	Eigen::VectorXd matrixB_nonlinear(voltageCount + currentCount);

	Eigen::VectorXd solutions;
	Eigen::VectorXd prevSolutions;

	// apply linear stamps
	matrixA_linear.fill(0);
	matrixB_linear.fill(0);

	for (auto &it : components)
		if (it->linear())
			it->applyComponentStamp(matrixA_linear, matrixB_linear, voltageCount);

	int iteration = 1;

	double previousMagnitude = 0;

	bool convergedCurrents = false;
	bool convergedVoltages = false;

	bool converged = false;

	while (!converged && iteration < MAX_ITERATIONS) {

		matrixA_nonlinear.fill(0);
		matrixB_nonlinear.fill(0);

		for (auto &it : components)
			if (!it->linear())
				it->applyComponentStamp(matrixA_nonlinear, matrixB_nonlinear, voltageCount);

		solutions = (matrixA_linear + matrixA_nonlinear).partialPivLu().solve(matrixB_linear + matrixB_nonlinear);

		if (iteration > 1) {

			double delta = (solutions - prevSolutions).squaredNorm();
		//	logWindow->log("delta was " + QString::number(delta));

			converged = delta <= solutions.size() * DELTA * DELTA;

		}
		
		prevSolutions = solutions;


		for (auto& it : nodes)
			if (it->voltageIndex >= 1)
				it->voltageValue = solutions(it->voltageIndex - 1);
			else
				it->voltageValue = double(0);

		for (auto& it : components)
			if (it->requiresCurrentEntry())
				it->currentValue = solutions((voltageCount - 1) + it->currentIndex);

		// TODO optimize
		//for (int i = 0; i < voltageCount; ++i)

		logWindow->log("Solutions at iteration " + QString::number(iteration) + " = " + vectorToString(solutions), LogEntryType::Debug);

		iteration++;

	}

	logWindow->log("Final solutions = " + vectorToString(solutions), LogEntryType::Debug);
		// print out solutions
		//logWindow->log("Matrix A = " + matrixToString(matrixA), LogEntryType::Debug);
		//logWindow->log("Matrix B = " + matrixToString(matrixB), LogEntryType::Debug);
	if (converged) {
		logWindow->log("Converged in " + QString::number(iteration) + " iterations.");	
		return SimulationResult::Success;
	
	} else {
		return SimulationResult::Success;
		logWindow->log("FAILED!");
	//	return SimulationResult::Error_NewtonTimedOut;
	}

	//else 
//




}
