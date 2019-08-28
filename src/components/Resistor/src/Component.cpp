#include <string>
#include <optional>

#include <qpainter>

extern "C" {
	__declspec(dllexport) int __stdcall getVal();
}


std::string name() {
	return "resistor";
}

std::string displayNameBase() {
	return "Resistor";
}

std::string letterIdentifierBase() {
	return "R";
}

void draw(QPainter& painter) {
	std::vector<QPoint> path = { { -24, 0 }, {-11, 0 }, {-7, -7}, {-2, 7}, {3, -7}, {8, 7}, {12, 0}, {24, 0} };

	for (auto& it : path)
		it = rotatePoint(it + _pos, _pos, _rotationAngle);

	for (int i = 1; i < path.size(); ++i)
		painter.drawLine(path[i - 1], path[i]);

	//TODO this is remporary
	if (serialNumber > 0) {
		QPoint pos(-4, -16);
		painter.drawText(rotatePoint(pos + _pos, _pos, _rotationAngle % 180), letterIdentifierBase() + QString::number(serialNumber));
	}
}

int nodeCount() {
	return 2;
}

bool requiresCurrentEntry() {
	return false;
}

bool linear() {
	return true;
}

std::optional<SimulationResult> simulationResult() {
	double current = (coupledNodes[1]->voltageValue - coupledNodes[0]->voltageValue) / properties["resistance"].value;

	return { { letterIdentifierBase() + QString::number(serialNumber), "Device current [A]", current } };
}

void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) {
	// TODO  division by zero etc
	double val = 1 / properties["resistance"].value;

	addStampEntry(matrixA, val, coupledNodes[1]->voltageIndex - 1, coupledNodes[1]->voltageIndex - 1);
	addStampEntry(matrixA, -val, coupledNodes[1]->voltageIndex - 1, coupledNodes[0]->voltageIndex - 1);
	addStampEntry(matrixA, -val, coupledNodes[0]->voltageIndex - 1, coupledNodes[1]->voltageIndex - 1);
	addStampEntry(matrixA, val, coupledNodes[0]->voltageIndex - 1, coupledNodes[0]->voltageIndex - 1);
}

PropertyMap propertiesFuncTEMP() {
	return { "resistance",  { "Resistance", QString::fromWCharArray(L"\u03A9"), 1000 } };
}

QRect boundingRect() {
	return QRect(-24, 12, 24, -12).normalized();
}

std::vector<std::pair<int, int>> nodeOffsets() {

	return { std::make_pair<-24, 0>, std::make_pair<24, 0> };
}