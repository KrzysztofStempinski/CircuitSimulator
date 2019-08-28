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
#include <optional>

#include <qpainter.h>

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/istreamwrapper.h"

#include "..//eigen//Dense"

#include "SimulableObject.h"
#include "EditorObject.h"
#include "Property.h"
#include "Component.h"
#include "Math.h"

#include "utils/DLLFunctionLoader.h"

class Component;

#include "Node.h"

struct SimulationResult {
	QString name = QString::null;
	QString unit = QString::null;
	double value = double(0);
};

class Component : public SimulableObject, public EditorObject {

protected:
	int _rotationAngle;

public:

	std::function<std::string()> name;
	std::function<std::string()> displayNameBase;
	std::function<std::string()> letterIdentifierBase;

	std::function<void(QPainter& painter)> draw;
	std::function<int()> nodeCount;
	std::function<bool()> requiresCurrentEntry;
	std::function<bool()> linear;

	std::function<std::vector<std::pair<int, int>>()> nodeOffsets;

	std::function<void(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount)> applyComponentStamp;

	std::function<std::optional<SimulationResult>()> simulationResult;

	std::function<QRect()> boundingRect;

	std::function<PropertyMap()> propertiesFuncTEMP;

	Component(const std::string& _name) : _rotationAngle(0), ID(-1) {

		// TODO rewrite!!!
		std::string dllName = _name + ".dll";
		dllName[0] = std::toupper(dllName[0]);
	
		name					= loadDllFunc<std::string()>(dllName, "name");
		displayNameBase			= loadDllFunc<std::string()>(dllName, "displayNameBase");
		letterIdentifierBase	= loadDllFunc<std::string()>(dllName, "letterIdentifierBase");
		draw					= loadDllFunc<void(QPainter & painter)>(dllName, "draw");
		nodeCount				= loadDllFunc<int()>(dllName, "nodeCount");
		requiresCurrentEntry	= loadDllFunc<bool()>(dllName, "requiresCurrentEntry");
		linear					= loadDllFunc<bool()>(dllName, "linear");

		applyComponentStamp		= loadDllFunc<void(Eigen::MatrixXd & matrixA, Eigen::VectorXd & matrixB, int voltageCount)>(dllName, "applyComponentStamp");

		simulationResult		= loadDllFunc<std::optional<SimulationResult>()>(dllName, "simulationResult");

		nodeOffsets				= loadDllFunc<std::vector<std::pair<int, int>>()>(dllName, "nodeOffsets");

		boundingRect			= loadDllFunc<QRect()>(dllName, "boundingRect");

		propertiesFuncTEMP		= loadDllFunc<PropertyMap()>(dllName, "propertiesFuncTEMP");

		properties = propertiesFuncTEMP();

	}

	void updateNodeOffsets() {

		std::vector<std::pair<int, int>> _nodeOffsets = nodeOffsets();

		for (int i = 0; i < _nodeOffsets.size(); ++i)
			coupledNodes[i]->setOffset(QPoint(_nodeOffsets[i].first, _nodeOffsets[i].second));
		
	}

	int ID;
	int serialNumber;

	void setPos(const QPoint& newPos) {
		_pos = newPos;

		for (const auto& it : coupledNodes)
			it->updatePos();
	}

	int getRotationAngle() const {
		return this->_rotationAngle;
	}

	void setRotationAngle(int angle) {
		// division by zero is apparently bad
		if (angle == 0)
			_rotationAngle = 0;
		else
			_rotationAngle = angle % (sign(angle) * 360);

		// NOTE we assume that the only rotation angles will be 0, +-90, +=180, +-270, +-360
		if (_rotationAngle % 180 != 0)
			_boundingRect = _boundingRect.transposed();

		//  update node positions
		setPos(_pos);
	}

	PropertyMap properties;

	std::vector<Node*> coupledNodes;

	void saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value valueComponent;
		valueComponent.SetObject();

		valueComponent.AddMember("ID", ID, allocator);

		rapidjson::Value name(name().c_str(), name().size(), allocator);
		valueComponent.AddMember("name", name, allocator);

		rapidjson::Value position(rapidjson::kArrayType);
		position.PushBack(_pos.x(), allocator);
		position.PushBack(_pos.y(), allocator);
		valueComponent.AddMember("position", position, allocator);

		valueComponent.AddMember("rotationAngle", _rotationAngle, allocator);

		// TODO save properties to file
		/*if (!properties.empty()) {
			rapidjson::Value propertiesVal;
			for (auto& it : properties) {
				rapidjson::Value valueProperty(it.first.toUtf8(), it.second.value, allocator);
				propertiesVal.AddMember(, valueProperty);
			}

			valueComponent.AddMember("properties", valueComponent, allocator);
		}*/

		arrayComponents.PushBack(valueComponent, allocator);
	}

};
