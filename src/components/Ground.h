/*

	This file is part of CircuitSimulator
	Copyright (C) 2018-2019 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Ground.h
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

class Ground : public Component {

public:

	Ground() {

		ID = -1;

		_name = "ground";

		_boundingRect.setCoords(-12, 15, 12, -15);

		_boundingRect = _boundingRect.normalized();

	}

	//
	void draw(QPainter& painter) {

		QPoint path[16] = { { -12, 0 }, { 12, 0 }, { -12, -1 }, { 12, -1 }, {-10, 3}, {10, 3}, {-8, 6}, {8, 6}, {-6, 9}, { 6, 9 }, { -4, 12}, {4, 12}, {-2, 15}, {2, 15}, {0, 0}, {0, -15} };

		for (auto& it : path)
			it = rotatePoint(it + _pos, _pos, _rotationAngle);

		for (int i = 0; i < 15; i += 2)
			painter.drawLine(path[i], path[i + 1]);

		//TODO this is remporary

		if (serialNumber > 0) {
			QPoint pos(0, -24);
			painter.drawText(rotatePoint(pos + _pos, _pos, _rotationAngle % 180), letterIdentifierBase() + QString::number(serialNumber));


		}
	}

	int nodeCount() {
		return 1;
	}

	// DCOP STUFF
	bool requiresCurrentEntry() {
		return false;
	}

	void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) {

	}

	QString displayNameBase() {
		return "Ground";
	}

	QString letterIdentifierBase() {
		return "GND";
	}

	std::tuple<QString, QString, double> getSimulationResult() {

		return std::make_tuple("gg", "gg", 12);
	}

	// TODO this is temporary
	bool hasSimulationResult() {
		return false;
	}

	void updateNodeOffsets() {

		coupledNodes[0]->setOffset(QPoint(0, -15));

	}

};
