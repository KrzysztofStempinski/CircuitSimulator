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

class Circuit {

	void prepareDCOP();

public:

	std::list<Node*> nodes;
	std::list<Component*> components;

	std::list<Node*>::iterator createNode(const QPoint& pos, Component* _coupledComponent = nullptr);
	void deleteNode(std::list<Node*>::iterator node);

	void connectNodes(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2);
	void disconnectNodes(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2);
	[[nodiscard]] bool areNodesConnected(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2) const;

	std::list<Component*>::iterator createComponent(const QString componentName, const QPoint& pos, const bool createNodes = true);
	void deleteComponent(std::list<Component*>::iterator component);

	bool saveToFile(const QString& fileName);	
	bool loadFromFile(const QString& fileName);

	int voltageCount;
	int currentCount;

	void simulate();
};
