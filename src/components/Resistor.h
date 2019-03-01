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
#include "../Geometry.h"

#include "../LogWindow.h"

#include <qpainter.h>
#include <qstring.h>

class Resistor : public Component {

	public:

		Resistor(){

			ID = -1;

			_name = "resistor";

			properties.insert(std::pair<QString, Property>("resistance", 
					Property("Resistance",
						QString::fromUtf8("\u03C9"),
						1000.0f)));


			_boundingRect.setCoords(-24, 12, 24, -12);

			_boundingRect = _boundingRect.normalized();
			
		}

		//
		void draw(QPainter& painter) {

			QPoint path[8] = { { -24, 0 }, {-11, 0 }, {-7, -7}, {-2, 7}, {3, -7}, {8, 7}, {12, 0}, {24, 0} };

			for (auto& it : path)
				it = rotatePoint(it + _pos, _pos, _rotationAngle);

			for (int i = 1; i < 8; ++i)
				painter.drawLine(path[i - 1], path[i]);

			//TODO this is remporary

			if (ID != -1) {
			QPoint pos(0, 12);
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
			matrixA(coupledNodes[1]->voltageIndex, coupledNodes[1]->voltageIndex) = 1 / properties["resistance"].value;
			matrixA(coupledNodes[1]->voltageIndex, coupledNodes[0]->voltageIndex) = - 1 / properties["resistance"].value;
			matrixA(coupledNodes[0]->voltageIndex, coupledNodes[1]->voltageIndex) = -1 / properties["resistance"].value;
			matrixA(coupledNodes[0]->voltageIndex, coupledNodes[0]->voltageIndex) = 1 / properties["resistance"].value;

		}

		QString displayNameBase() {
			return "Resistor";
		} 

		QString letterIdentifierBase() {
			return "R";
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
