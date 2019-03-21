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

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/istreamwrapper.h"

#include "..//eigen//Dense"

#include "SimulableObject.h"
#include "EditorObject.h"

class Component;

#include "Node.h"
#include "Property.h"

#include "LogWindow.h"

#include <qpainter.h>

class Component : public SimulableObject, public EditorObject {

	protected:

		QString _name;

		int _rotationAngle;

	public:

		Component() : _rotationAngle(0), ID(-1) {}
		LogWindow* logWindow; // TODO temp
		
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
			
		std::map<QString, Property> properties;
		
		std::vector<Node*> coupledNodes;

		virtual void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) = 0;

		virtual std::tuple<QString, QString, double> getSimulationResult() = 0;

		// TODO this is temporary
        virtual bool hasSimulationResult() = 0;

		virtual void updateNodeOffsets() = 0;

		void saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator);

};
