/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Diode.h
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

class Diode : public Component {
public:

	Diode() {
		_name = "diode";

		properties["sat_curr"] = { "Saturation current", "A", 1e-14 };

		properties["therm_volt"] = { "Thermal voltage", "V", 26e-3 };

		_boundingRect.setCoords(-24, 12, 24, -12);

		_boundingRect = _boundingRect.normalized();
	}

	void draw(QPainter& painter) {
		std::vector<QPoint> path = { { -24, 0 }, {-6, 0 }, {-6, 12}, {-6, -12}, {-6, 0}, {6, 12}, {-6, 0}, {6, -12}, {6, 12}, {6, -12}, {6, 0}, {24, 0} };

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
		return false;
	}

	void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) {
		double sat = properties["sat_curr"].value;
		double vt = properties["therm_volt"].value;
		// TODO  division by zero etc
		double curr = sat * (std::exp((coupledNodes[1]->voltageValue - coupledNodes[0]->voltageValue) / vt) - 1);
		double Go = (sat / vt) * std::exp((coupledNodes[1]->voltageValue - coupledNodes[0]->voltageValue) / vt);
		double Ieq = curr - Go * (coupledNodes[1]->voltageValue - coupledNodes[0]->voltageValue);

		double vdk = coupledNodes[1]->prevVoltageValue - coupledNodes[0]->prevVoltageValue;

		double vtilde = coupledNodes[1]->voltageValue - coupledNodes[0]->voltageValue;

		double vcrit = vt * log(vt / (sqrt(2) * sat));

		//double vwould =

		//double vdiff =

		if (vtilde <= vcrit) {
			// no overflow occured, we may proceed without any PN junction damping
			addStampEntry(matrixA, Go, coupledNodes[1]->voltageIndex - 1, coupledNodes[1]->voltageIndex - 1);
			addStampEntry(matrixA, Go, coupledNodes[0]->voltageIndex - 1, coupledNodes[0]->voltageIndex - 1);
			addStampEntry(matrixA, -Go, coupledNodes[1]->voltageIndex - 1, coupledNodes[0]->voltageIndex - 1);
			addStampEntry(matrixA, -Go, coupledNodes[0]->voltageIndex - 1, coupledNodes[1]->voltageIndex - 1);

			addStampEntry(matrixB, -Ieq, coupledNodes[1]->voltageIndex - 1);
			addStampEntry(matrixB, Ieq, coupledNodes[0]->voltageIndex - 1);
		}
		else {
			double vdkp1 = vdk + vt * std::log(1 + (vtilde - vdk) / vt);

			addStampEntry(matrixA, 1, coupledNodes[1]->voltageIndex - 1, coupledNodes[1]->voltageIndex - 1);
			addStampEntry(matrixA, 1, coupledNodes[0]->voltageIndex - 1, coupledNodes[0]->voltageIndex - 1);
			addStampEntry(matrixA, -1, coupledNodes[1]->voltageIndex - 1, coupledNodes[0]->voltageIndex - 1);
			addStampEntry(matrixA, -1, coupledNodes[0]->voltageIndex - 1, coupledNodes[1]->voltageIndex - 1);

			addStampEntry(matrixB, -vdkp1, coupledNodes[1]->voltageIndex - 1);
			addStampEntry(matrixB, vdkp1, coupledNodes[0]->voltageIndex - 1);
		}
	}

	QString displayNameBase() {
		return "Ideal (Schockley) diode";
	}

	QString letterIdentifierBase() {
		return "D";
	}

	SimulationResult getSimulationResult() {
		double current = properties["sat_curr"].value * (std::exp((coupledNodes[1]->voltageValue - coupledNodes[0]->voltageValue) / properties["therm_volt"].value) - 1);

		return { letterIdentifierBase() + QString::number(serialNumber), "Device current [A]", current };
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
