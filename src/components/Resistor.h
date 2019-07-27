/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Resistor.h
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
#include "../MatrixHelper.h"

#include "../Math.h"

#include <qpainter.h>
#include <qstring.h>

class Resistor : public Component {
public:

	Resistor() {
		_name = "resistor";

		properties["resistance"] = { "Resistance", QString::fromWCharArray(L"\u03A9"), 1000 };

		_boundingRect.setCoords(-24, 12, 24, -12);

		_boundingRect = _boundingRect.normalized();
	}

	void draw(QPainter& painter) {
		std::vector<QPoint> path = { { -24, 0 }, {-11, 0 }, {-7, -7}, {-2, 7}, {3, -7}, {8, 7}, {12, 0}, {24, 0} };

		for (auto& it : path)
			it = Math::rotatePoint(it + _pos, _pos, _rotationAngle);

		for (int i = 1; i < path.size(); ++i)
			painter.drawLine(path[i - 1], path[i]);

		//TODO this is remporary
		if (serialNumber > 0) {
			QPoint pos(-4, -16);
			painter.drawText(Math::rotatePoint(pos + _pos, _pos, _rotationAngle % 180), letterIdentifierBase() + QString::number(serialNumber));
		}
	}

	int nodeCount() {
		return 2;
	}

	// DCOP STUFF
	bool requiresCurrentEntry() {
		return false;
	}

	void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) {
		// TODO  division by zero etc
		double val = 1 / properties["resistance"].value;

		addStampEntry(matrixA, val, (*coupledNodes[1])->voltageIndex - 1, (*coupledNodes[1])->voltageIndex - 1);
		addStampEntry(matrixA, -val, (*coupledNodes[1])->voltageIndex - 1, (*coupledNodes[0])->voltageIndex - 1);
		addStampEntry(matrixA, -val, (*coupledNodes[0])->voltageIndex - 1, (*coupledNodes[1])->voltageIndex - 1);
		addStampEntry(matrixA, val, (*coupledNodes[0])->voltageIndex - 1, (*coupledNodes[0])->voltageIndex - 1);
	}

	QString displayNameBase() {
		return "Resistor";
	}

	QString letterIdentifierBase() {
		return "R";
	}

	std::optional<SimulationResult> getSimulationResult() {
		double current = ((*coupledNodes[1])->voltageValue - (*coupledNodes[0])->voltageValue) / properties["resistance"].value;

		return { { letterIdentifierBase() + QString::number(serialNumber), "Device current [A]", current } };
	}

	void updateNodeOffsets() {
		(*coupledNodes[0])->setOffset(QPoint(-24, 0));
		(*coupledNodes[1])->setOffset(QPoint(24, 0));
	}

	bool linear() {
		return true;
	}
};
