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
#include "../Geometry.h"

#include "../LogWindow.h"

#include <qpainter.h>
#include <qstring.h>

class VoltageSource : public Component {

public:

	VoltageSource() {

		ID = -1;

		_name = "voltage_source";

		properties.insert(std::pair<QString, Property>("voltage",
			Property("Voltage",
				"V",
				12)));


		_boundingRect.setCoords(-24, 10, 24, -10);

		_boundingRect = _boundingRect.normalized();

	}

	//
	void draw(QPainter& painter) {

		QPoint path[10] = { { -3, -10 }, { -3, 10 }, {3, -4}, {3, 4}, {4, -4}, {4, 4}, {4, 0}, {24, 0}, {-4, 0}, { -24, 0 } };

		for (auto& it : path)
			it = rotatePoint(it + _pos, _pos, _rotationAngle);

		for (int i = 0; i < 9; i += 2)
			painter.drawLine(path[i], path[i + 1]);

		//TODO this is remporary

		if (ID != -1) {
			QPoint pos(0, -12);
			painter.drawText(rotatePoint(pos + _pos, _pos, _rotationAngle % 180), letterIdentifierBase() + QString::number(serialNumber));


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

	}

	QString displayNameBase() {
		return "Voltage Source";
	}

	QString letterIdentifierBase() {
		return "V";
	}

	std::tuple<QString, QString, double> getSimulationResult() {

		return std::make_tuple("gg", "gg", 12);
	}

	// TODO this is temporary
	bool hasSimulationResult() {
		return true;
	}

	void updateNodeOffsets() {

		coupledNodes[0]->setOffset(QPoint(-24, 0));
		coupledNodes[1]->setOffset(QPoint(24, 0));

	}

};