/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Component.h
//
//  ---------------------------------------------

#pragma once

#include <vector>
#include <string>

#include <qpainter.h>

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/istreamwrapper.h"

#include "..//eigen//Dense"

#include "SimulableObject.h"
#include "EditorObject.h"
#include "Property.h"

#include <optional>

class Component;

#include "Node.h"

struct SimulationResult {
	QString name = QString::null;
	QString unit = QString::null;
	double value = double(0);
};

class Component : public SimulableObject, public EditorObject {
protected:

	QString _name;

	int _rotationAngle;

public:

	Component() : _rotationAngle(0), ID(-1) {
	}

	int ID;
	int serialNumber;
	QString getName();

	virtual QString displayNameBase() = 0;
	virtual QString letterIdentifierBase() = 0;

	void setPos(const QPoint& newPos);

	virtual void draw(QPainter& painter) = 0;

	int getRotationAngle() const;
	void setRotationAngle(const int angle);

	virtual int nodeCount() = 0;
	virtual bool requiresCurrentEntry() = 0;
	virtual bool linear() = 0;

	PropertyMap properties;

	std::vector<Node*> coupledNodes;

	virtual void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) = 0;

	virtual std::optional<SimulationResult> getSimulationResult();

	virtual void updateNodeOffsets() = 0;

	void saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator);

};
