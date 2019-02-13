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

#include "Log.h"

Node::Node(const QPoint& newPos, bool isCoupled, Component* coupledComponent){

	_isCoupled = isCoupled;
	_coupledComponent = coupledComponent;

	isSelected = false;

	voltageIndex = -1;
	voltageValue = double(0);
	_pos = newPos;

	int size = 2 + _isCoupled;

	_boundingRect.setCoords(-size, size, size, -size);

}

bool Node::isCoupled() {
	return _isCoupled;
}


Component* Node::getcoupledComponent(){
	return _coupledComponent;
}


QPoint Node::getPos() {
	return _pos;
}

void Node::setPos(const QPoint &newPos) {
	_pos = newPos;
}

void Node::updatePos() {

	_pos = rotatePoint(_coupledComponent->getPos() + _posOffset, _coupledComponent->getPos(), _coupledComponent->getRotationAngle());

}

void Node::setOffset(const QPoint& offset) {
	
	_posOffset = offset;
		
}

bool Node::isMouseOver(const QPoint& mousePos) {

	int size = NODE_SIZE + _isCoupled; // we add _isCoupled again so that bounding rect is slightly bigger, hence easier to select

	return (abs(mousePos.x() - (_pos.x())) <= size && abs(mousePos.y() - (_pos.y())) <= size);

}

void Node::connectTo(Node* node) {

	connectedNodes.push_back(node);
	node->connectedNodes.push_back(this);

}

void Node::disconnectFrom(Node* node) {

	auto it = std::find(std::begin(connectedNodes), std::end(connectedNodes), node);

	if (it != std::end(connectedNodes))
		connectedNodes.erase(it);

	it = std::find(std::begin(node->connectedNodes), std::end(node->connectedNodes), this);

	if (it != std::end(node->connectedNodes))
		node->connectedNodes.erase(it);

}

bool Node::isConnectedTo(Node* node) {
	
	auto it = find(std::begin(connectedNodes), std::end(connectedNodes), node);

	return (it != std::end(connectedNodes));

}

void Node::draw(QPainter *painter){

	int size = NODE_SIZE + _isCoupled;

	// TODO ugly hack
	painter->fillRect(_pos.x() - size, _pos.y() - size, 2 * size, 2 * size, painter->pen().color());

	if (DEBUG)
		painter->drawText(QPoint(_pos.x() + 6, _pos.y() + 6), QString::number(voltageIndex));
	
}

bool Node::isWithinRectangle(const QRect& rect){
			    		
	return _boundingRect.translated(_pos).intersects(rect);

}

void Node::removeInboundLinks(){

	for (const auto& it : connectedNodes) {

		auto jt = std::find(std::begin(it->connectedNodes), std::end(it->connectedNodes), this);

		if (jt != std::end(it->connectedNodes)) {
			it->connectedNodes.erase(jt);
				//break; // TODO why not?
		}

	}

}

void Node::saveToJSON(rapidjson::Value& nodeArray, rapidjson::Document::AllocatorType& allocator) {

	rapidjson::Value valueNode;
	valueNode.SetObject();

	valueNode.AddMember("ID", ID, allocator);

	rapidjson::Value position(rapidjson::kArrayType);
	position.PushBack(_pos.x(), allocator);
	position.PushBack(_pos.y(), allocator);

	valueNode.AddMember("position", position, allocator);

	if (!std::empty(connectedNodes)) {

		rapidjson::Value connectedNodesArray(rapidjson::kArrayType);

		for (const auto& it : connectedNodes)
			connectedNodesArray.PushBack(it->ID, allocator);

		valueNode.AddMember("connectedNodes", connectedNodesArray, allocator);

	}

	if (_isCoupled) {
		valueNode.AddMember("coupled", true, allocator);
		valueNode.AddMember("coupledComponent", _coupledComponent->ID, allocator);
	}

	nodeArray.PushBack(valueNode, allocator);

}