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

		std::list<Node*> nodes;
		std::list<Component*> components;

		void createNode(const QPoint& pos, bool _isCoupled = false, Component* _coupledComponent = NULL);
	
		std::list<Node*>::iterator deleteNode(std::list<Node*>::iterator node);
		std::list<Node*>::iterator deleteNode(Node* node);

		void createComponent(QString componentName, const QPoint& pos, bool createNodes = true);

		void deleteComponent(Component* component);
		void deleteComponent(std::list<Component*>::iterator component);

		bool loadFromFile(QString fileName);
		bool saveToFile(QString fileName);

		int voltageCount;
		int currentCount;

};

