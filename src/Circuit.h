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

#include <list>

#include "Node.h"
#include "SimulationErrors.h"

class Circuit {
private:

	void prepareDCOP();

public:

	std::list<Node*> nodes;
	std::list<Component*> components;

	//TODO order of arguments in createNode/component should be the same
	std::list<Node*>::iterator createNode(const QPoint& pos, Component* _coupledComponent = nullptr);
	void deleteNode(std::list<Node*>::iterator node);

	void connectNodes(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2);
	void disconnectNodes(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2);
	bool areNodesConnected(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2);

	std::list<Component*>::iterator createComponent(const QString componentName, const QPoint& pos, const bool createNodes = true);
	void deleteComponent(std::list<Component*>::iterator component);

	bool loadFromFile(const QString fileName);
	bool saveToFile(const QString fileName);

	int voltageCount;
	int currentCount;

	void simulate();
};
