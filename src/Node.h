/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Node.h
//
//  ---------------------------------------------

#pragma once

#include <vector>

#include <qpainter.h>

#include "../rapidjson/rapidjson.h"

#include "SimulableObject.h"
#include "EditorObject.h"

class Node;

#include "Component.h"

constexpr int NODE_SIZE = 3;

class Node : public SimulableObject, public EditorObject {

	QPoint _posOffset;
	Component* _coupledComponent;

public:

	int ID;

	Node(const QPoint& newPos, Component* coupledComponent = nullptr);

	std::vector<std::list<Node*>::iterator> connectedNodes;

	void setPos(const QPoint& newPos);
	void updatePos();
	void setOffset(const QPoint& offset);

	bool isCoupled();
	Component* getCoupledComponent();

	void draw(QPainter& painter);

	void saveToJSON(rapidjson::Value& nodeArray, rapidjson::Document::AllocatorType& allocator);

	// TODO decent argument names?
	void markAdjacentNodes(int _voltageIndex);
};
