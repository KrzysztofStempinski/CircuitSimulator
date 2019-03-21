/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	IdealOpAmp.h
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

#include "../LogWindow.h"

#include <qpainter.h>
#include <qstring.h>

class IdealOpAmp : public Component {

public:

	IdealOpAmp() {

		ID = -1;

		_name = "ideal_op_amp";

		_boundingRect.setCoords(-24, 12, 24, -12);

		_boundingRect = _boundingRect.normalized();

	}

	//
	void draw(QPainter& painter) {

		QPoint path[12] = { { -24, 0 }, {-6, 0 }, {-6, 12}, {-6, -12}, {-6, 0}, {6, 12}, {-6, 0}, {6, -12}, {6, 12}, {6, -12}, {6, 0}, {24, 0} };

		for (auto& it : path)
			it = rotatePoint(it + _pos, _pos, _rotationAngle);

		for (int i = 0; i < 11; i += 2)
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
		return false;
	}

	void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) {



	}

	QString displayNameBase() {
		return "Ideal (Schockley) diode";
	}

	QString letterIdentifierBase() {
		return "D";
	}

	std::tuple<QString, QString, double> getSimulationResult() {

		double current = properties["sat_curr"].value * (std::exp((coupledNodes[1]->voltageValue - coupledNodes[0]->voltageValue) / properties["therm_volt"].value) - 1);

		return std::make_tuple(letterIdentifierBase() + QString::number(serialNumber), "Device current [A]", current);

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
		return false;
	}

};
