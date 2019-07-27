/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Node.cpp
//
//  ---------------------------------------------

#include "Node.h"
#include "Math.h"

Node::Node(const QPoint& newPos, Component* coupledComponent)
	: _coupledComponent(coupledComponent){
	_pos = newPos;
	int size = NODE_SIZE + isCoupled();
	_boundingRect.setCoords(-size, size, size, -size);
}

bool Node::isCoupled(){
	return _coupledComponent != nullptr;
}

//TODO can we do this with iterators?
Component* Node::getCoupledComponent(){
	return _coupledComponent;
}

void Node::setPos(const QPoint& newPos){
	_pos = newPos;
}

void Node::updatePos(){
	_pos = Math::rotatePoint(_coupledComponent->pos() + _posOffset, _coupledComponent->pos(), _coupledComponent->getRotationAngle());
}

void Node::setOffset(const QPoint& offset){
	_posOffset = offset;
}

void Node::draw(QPainter& painter){

	int size = NODE_SIZE + isCoupled();

	painter.fillRect(_pos.x() - size, _pos.y() - size, 2 * size, 2 * size, painter.pen().color());

	//if (DEBUG)
	//	painter.drawText(QPoint(_pos.x() + 6, _pos.y() + 6), QString::number(voltageIndex));
}

void Node::saveToJSON(rapidjson::Value& nodeArray, rapidjson::Document::AllocatorType& allocator){
	rapidjson::Value valueNode;
	valueNode.SetObject();

	valueNode.AddMember("ID", ID, allocator);

	rapidjson::Value position(rapidjson::kArrayType);
	position.PushBack(_pos.x(), allocator);
	position.PushBack(_pos.y(), allocator);

	valueNode.AddMember("position", position, allocator);

	if (!std::empty(connectedNodes)){
		rapidjson::Value connectedNodesArray(rapidjson::kArrayType);

		for (const auto& it : connectedNodes)
			connectedNodesArray.PushBack((*it)->ID, allocator);

		valueNode.AddMember("connectedNodes", connectedNodesArray, allocator);
	}

	if (isCoupled()){
		valueNode.AddMember("coupled", true, allocator);
		valueNode.AddMember("coupledComponent", _coupledComponent->ID, allocator);
	}

	nodeArray.PushBack(valueNode, allocator);
}

void Node::markAdjacentNodes(const int _voltageIndex){

	voltageIndex = _voltageIndex;

	for (const auto& it : connectedNodes)
		if ((*it)->voltageIndex == -1) // if a node wasn't visited yet
			(*it)->markAdjacentNodes(voltageIndex);

}
