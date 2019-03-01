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

class Node;

#include "Component.h"
#include "Geometry.h"

constexpr int NODE_SIZE = 3;

class Node {

	private:

		QPoint _pos; 
		QPoint _posOffset;
		QRect _boundingRect;
		Component* _coupledComponent;

	public:

		int ID;

		Node(const QPoint& newPos, Component* coupledComponent = nullptr);

		std::vector<Node*> connectedNodes;

		void setPos(const QPoint& newPos);
		const QPoint pos() const;
		void updatePos();
		void setOffset(const QPoint& offset);

		void connectTo(Node* node);
		void disconnectFrom(Node* node);
		bool isConnectedTo(Node* node);

		void removeInboundLinks();

		bool isCoupled();
		Component* getCoupledComponent();
		
		void draw(QPainter* painter);

		bool isMouseOver(const QPoint& mousePos);
		bool isWithinRectangle(const QRect& rect);
		
		// TODO refactor
		//CircuitEfitor related stuff stuff
		bool selected;
		QPoint prevPos;

		void saveToJSON(rapidjson::Value& nodeArray, rapidjson::Document::AllocatorType& allocator);

		// SIMULATION STUFF
		int voltageIndex;
		double voltageValue;

};


