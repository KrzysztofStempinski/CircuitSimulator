#include "Circuit.h"
#include "MatrixHelper.h"

// TODO settings entry or sth like that?
constexpr int MAX_ITERATIONS = 400; // should be enough
constexpr double DELTA = 1e-6;

void Circuit::prepareDCOP() {

	// sanity check
	if (components.empty())
		throw SimulationException("no components present in the circuit");

	// cleanup
	for (auto& it : nodes) 
		it->reset();

	// first pass
	bool foundGround = false;
	for (const auto& it : nodes) {
		if (it->voltageIndex == -1) {
			if (it->isCoupled() && it->getCoupledComponent()->getName() == QString("ground")) {// TODO actually implement checking whether this is ground from config file

				it->markAdjacentNodes(0);
				
				foundGround = true;

			}
		}
	}

	if (!foundGround)
		throw SimulationException("no ground node present in the circuit");

	int voltageIndex = 0;
	for (auto& it : nodes) {
		if (it->voltageIndex == -1) {
			voltageIndex++;
			it->markAdjacentNodes(voltageIndex);
		}
	}

	voltageCount = voltageIndex;

	int currentIndex = 0;
	for (const auto& it : components) 
		if (it->requiresCurrentEntry()) 
			it->currentIndex = ++currentIndex;

	currentCount = currentIndex;

}

void Circuit::simulate() {

	prepareDCOP();

	Eigen::MatrixXd matrixA_linear(voltageCount + currentCount, voltageCount + currentCount);
	Eigen::MatrixXd matrixA_nonlinear(voltageCount + currentCount, voltageCount + currentCount);
	Eigen::VectorXd matrixB_linear(voltageCount + currentCount);
	Eigen::VectorXd matrixB_nonlinear(voltageCount + currentCount);

	Eigen::VectorXd solutions;
	Eigen::VectorXd prevSolutions;

	matrixA_linear.fill(0);
	matrixB_linear.fill(0);

	// apply stamps from linear components (resistors etc.)
	for (auto &it : components)
		if (it->linear())
			it->applyComponentStamp(matrixA_linear, matrixB_linear, voltageCount);

	int iteration = 0;
	bool converged = false;

	while (!converged && iteration <= MAX_ITERATIONS) {

		iteration++;

		matrixA_nonlinear.fill(0);
		matrixB_nonlinear.fill(0);

		// apply stamps from non-linear components (like diodes)
		for (auto &it : components)
			if (!it->linear())
				it->applyComponentStamp(matrixA_nonlinear, matrixB_nonlinear, voltageCount);

		// solve the assembled MNA matrix
		solutions = (matrixA_linear + matrixA_nonlinear).partialPivLu().solve(matrixB_linear + matrixB_nonlinear);

		// we want x^(n+1)_i - x^(n)_i to be <= DELTA for all i (for every pair of voltages/currents),
		// then we claim that we have achieved  convergence. It is equivalent to checking whether norm^2
		// of solutions(at (n+1)-iteration) - solutions(at (n)-iteration) is <= dim(solutions) * DELTA^2. 
		if (iteration > 1) 
			converged = (solutions - prevSolutions).squaredNorm() <= solutions.size() * DELTA * DELTA;
		
		prevSolutions = solutions;
		
		// update node voltages/component currents
		for (auto& it : nodes)
			if (it->voltageIndex >= 1) {
				it->prevVoltageValue = it->voltageValue;
				it->voltageValue = solutions(it->voltageIndex - 1);
			}
			else {
				it->voltageValue = it->prevVoltageValue = double(0);
			}
			

		for (auto& it : components)
			if (it->requiresCurrentEntry())
				it->currentValue = solutions((voltageCount - 1) + it->currentIndex);


	}

	logWindow->log("Final solutions = " + vectorToString(solutions), LogEntryType::Debug);

	if (converged) {
		logWindow->log("Newton's method converged in " + QString::number(iteration) + " iterations.");	
	} else {
		throw SimulationException("Newton's method failed to converge");
	}

}
