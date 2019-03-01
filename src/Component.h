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

class Component;

#include "Node.h"
#include "Property.h"
#include "Geometry.h"

#include "LogWindow.h"

#include <qpainter.h>

class Component {

	protected:

		QString _name;

		QPoint _pos;
		int _rotationAngle;
		QRect _boundingRect;


	public:

		Component() {
		
		//_
			_rotationAngle = 0;
			isSelected = false;
			currentValue = double(0);
			currentIndex = -1;
		
		
		}

		LogWindow* logWindow; // TODO temp
		
		int ID;
		int serialNumber;
		QString getName();
		virtual QString getDisplayNameBase() = 0;
		virtual QString getLetterIdentifierBase() = 0;

		//
		const QPoint pos() const;
		void setPos(const QPoint& newPos);

		//
		virtual void draw(QPainter& painter) = 0;
		bool isMouseOver(const QPoint& mousePos) const;
		bool isWithinRectangle(const QRect& rect) const;
		
		int getRotationAngle() const;
		void setRotationAngle(const int angle);

		//
		virtual int getNumberOfNodes() = 0;

		//
		std::map<QString, Property> properties;
		
		//
		std::vector<Node*> coupledNodes;

		//TODO refactor
		int currentIndex;
		double currentValue;

		bool isSelected;
		QPoint prevPos;

		// DCOP STUFF
		virtual bool requireCurrentEntry() = 0;

		virtual void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount) = 0;

		virtual std::tuple<QString, QString, double> getSimulationResult() = 0;

		// TODO this is temporary
        virtual bool hasSimulationResult() = 0;

		virtual void updateNodeOffsets() = 0;

		void saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator);

};
