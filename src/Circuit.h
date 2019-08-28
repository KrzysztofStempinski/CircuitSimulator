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

#include "Node.h"
#include "SimulationErrors.h"
#include "LogWindow.h"

class Circuit {
private:

	void prepareDCOP();

public:

	LogWindow* logWindow;

	std::vector<Node*> nodes;
	std::vector<Component*> components;

	void createNode(const QPoint& pos, Component* _coupledComponent = nullptr);

	void deleteNode(Node* node);

	void createComponent(std::string componentName, const QPoint& pos, const bool createNodes = true);

	void deleteComponent(Component* component);

	bool loadFromFile(const QString fileName);
	bool saveToFile(const QString fileName);

	int voltageCount;
	int currentCount;

	void simulate();
};
