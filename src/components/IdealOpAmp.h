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

		_boundingRect.setCoords(-38, 44, 82, -44);

		_boundingRect = _boundingRect.normalized();

	}

	//
	void draw(QPainter& painter) {

		std::vector<QPoint> path = {	{ -38, 18 }, { -12, 18 }, 
										{ -38, -18 }, { -12, -18 }, 
										{ -12, 44 }, { -12, -44 }, 
										{ -12, 44 }, { 62, 0 }, 
										{ -12, -44 }, { 62, 0}, 
										{ 62, 0 }, { 82, 0 }, 
										// plus sign
										{ -6, 18 }, { 6, 18 },
										{ 0, -24 }, { 0, -12 },
										// minus sign
										{ -6, -18 }, { 6, -18 },
									}; 
		
		for (auto& it : path)
			it = rotatePoint(it + _pos, _pos, _rotationAngle);

		for (int i = 0; i < path.size(); i += 2)
			painter.drawLine(path[i], path[i + 1]);

		//TODO this is remporary
		if (serialNumber > 0) {
			QPoint pos(30, -30);
			painter.drawText(rotatePoint(pos + _pos, _pos, _rotationAngle % 180), letterIdentifierBase() + QString::number(serialNumber));
		}

	}

	int nodeCount() {
		return 3;
	}

	// DCOP STUFF
	bool requiresCurrentEntry() {
		return false;
	}

	void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) {



	}

	QString displayNameBase() {
		return "Ideal op-amp";
	}

	QString letterIdentifierBase() {
		return "OP";
	}

	std::tuple<QString, QString, double> getSimulationResult() {

		return std::make_tuple("", "", 0);

	}

	// TODO this is temporary
	bool hasSimulationResult() {
		return true;
	}

	void updateNodeOffsets() {

		coupledNodes[0]->setOffset(QPoint(-38, 18)); // V-
		coupledNodes[1]->setOffset(QPoint(-38, -18)); // V_
		coupledNodes[2]->setOffset(QPoint(82, 0)); // Vout

	}

	bool linear() {
		return false;
	}

};
