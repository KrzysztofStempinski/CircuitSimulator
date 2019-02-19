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

#include "SimpleParser/SimpleParser.h"

class Component;

#include "Node.h"
#include "Property.h"
#include "Geometry.h"

#include "LogWindow.h"

#include <qpainter.h>

enum class MNAmatrix {
	A,
	B
};

struct ComponentStamp {

	MNAmatrix matrix;

	int rowNumber;
	int columnNumber;

	exprtk::expression<double> expression;

	double value;

};

class Component {

	private:

		rapidjson::Document _componentData;

		QString _name;

		QPoint _pos;
		int _rotationAngle;
		QRect _boundingRect;

		GeometryData _geometryObjects;

		// simulation-related stuff
		void _loadSimulationVariables(int voltageCount);

	//	TokenMap _simulationVariables;

		void _loadComponentStamps();
		std::list<ComponentStamp> _stamps;
		exprtk::symbol_table<double> _symbolTableProperties;

		SimpleParser::VarTable _varTableIndices;

	public:

		Component(QString name);

		LogWindow* logWindow; // TODO temp
		
		int ID;
		int serialNumber;
		QString getName();
		QString getDisplayNameBase();
		QString getLetterIdentifierBase();

		//
		const QPoint pos() const;
		void setPos(const QPoint& newPos);

		//
		void draw(QPainter& painter);
		bool isMouseOver(const QPoint& mousePos) const;
		bool isWithinRectangle(const QRect& rect) const;
		
		int getRotationAngle() const;
		void setRotationAngle(const int angle);

		//
		int getNumberOfNodes();

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
		bool requireCurrentEntry();

		void prepareForSimulation(int voltageCount);
		void applyComponentStamp(Eigen::MatrixXd& matrixA, Eigen::VectorXd& matrixB, int voltageCount, LogWindow* logWindow);

		std::tuple<QString, QString, double> getSimulationResult();

		// TODO this is temporary
        bool hasSimulationResult();

		void updateNodeOffsets();

		void saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator);

};
