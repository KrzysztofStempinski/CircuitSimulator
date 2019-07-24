/*

	This file is part of CircuitSimulator
	Copyright (C) 2018-2019 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	VoltageSource.h
//
//  ---------------------------------------------

#pragma once

#include <vector>
#include <string>

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/istreamwrapper.h"

#include "..//eigen//Dense"

#include "../Component.h"
#include "../Property.h"

#include "../LogWindow.h"

#include <qpainter.h>
#include <qstring.h>

class VoltageSource : public Component {
public:

	VoltageSource() {
		_name = "voltage_source";

		properties["voltage"] = { "Voltage", "V", 12 };

		_boundingRect.setCoords(-24, 10, 24, -10);

		_boundingRect = _boundingRect.normalized();
	}

	void draw(QPainter& painter) {
		std::vector<QPoint> path = { { -3, -10 }, { -3, 10 }, {3, -4}, {3, 4}, {4, -4}, {4, 4}, {4, 0}, {24, 0}, {-4, 0}, { -24, 0 } };

		for (auto& it : path)
			it = rotatePoint(it + _pos, _pos, _rotationAngle);

		for (int i = 0; i < path.size(); i += 2)
			painter.drawLine(path[i], path[i + 1]);

		//TODO this is remporary
		if (serialNumber > 0) {
			QPoint pos(0, -12);
			painter.drawText(rotatePoint(pos + _pos, _pos, _rotationAngle % 180), letterIdentifierBase() + QString::number(serialNumber));
		}
	}

	int nodeCount() {
		return 2;
	}

	// DCOP STUFF
	bool requiresCurrentEntry() {
		return true;
	}

	void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) {
		addStampEntry(matrixA, 1, coupledNodes[1]->voltageIndex - 1, voltageCount + currentIndex - 1);
		addStampEntry(matrixA, -1, coupledNodes[0]->voltageIndex - 1, voltageCount + currentIndex - 1);
		addStampEntry(matrixA, 1, voltageCount + currentIndex - 1, coupledNodes[1]->voltageIndex - 1);
		addStampEntry(matrixA, -1, voltageCount + currentIndex - 1, coupledNodes[0]->voltageIndex - 1);

		addStampEntry(matrixB, properties["voltage"].value, voltageCount + currentIndex - 1);
	}

	QString displayNameBase() {
		return "Voltage Source";
	}

	QString letterIdentifierBase() {
		return "V";
	}

	SimulationResult getSimulationResult() {
		return { letterIdentifierBase() + QString::number(serialNumber), "Device current [A]", currentValue };
	}

	// TODO this is temporary
	bool hasSimulationResult() {
		return true;
	}

	void updateNodeOffsets() {
		coupledNodes[0]->setOffset(QPoint(-24, 0));
		coupledNodes[1]->setOffset(QPoint(24, 0));
	}

	bool linear() {
		return true;
	}
};
