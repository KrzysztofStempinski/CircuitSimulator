/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Circuit.h
// 
//  ---------------------------------------------

#pragma once

class Circuit;

#include "Node.h"
#include "Geometry.h"

class Circuit {

	public:

		std::vector<Node*> nodes;
		std::vector<Component*> components;

		void createNode(const QPoint& pos, Component* _coupledComponent = nullptr);
	
		void deleteNode(Node* node);

		void createComponent(QString componentName, const QPoint& pos, bool createNodes = true);

		void deleteComponent(Component* component);

		bool loadFromFile(const QString fileName);
		bool saveToFile(const QString fileName);

		int voltageCount;
		int currentCount;

};

