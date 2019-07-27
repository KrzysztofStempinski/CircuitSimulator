/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Circuit.cpp
//
//  ---------------------------------------------

#include "Circuit.h"

#include "ComponentList.h"

std::list<Node*>::iterator Circuit::createNode(const QPoint& pos, Component* _coupledComponent) {

	nodes.emplace_back(new Node(pos, _coupledComponent));
	return  std::prev(nodes.end());

}

void Circuit::deleteNode(std::list<Node*>::iterator node) {

	// remove inbound links
	// TODO what?
	for (auto it = std::begin((*node)->connectedNodes); it != std::end((*node)->connectedNodes); ++it)
		(*node)->connectedNodes.erase(it);

	delete* node;

	nodes.erase(node);

}

void Circuit::connectNodes(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2){

	(*node1)->connectedNodes.push_back(node2);
	(*node2)->connectedNodes.push_back(node1);

}

void Circuit::disconnectNodes(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2){
	
	const auto it1 = std::find(std::begin((*node1)->connectedNodes), std::end((*node1)->connectedNodes), node2);
	if (it1 != std::end((*node1)->connectedNodes))
		(*node1)->connectedNodes.erase(it1);

	const auto it2 = std::find(std::begin((*node2)->connectedNodes), std::end((*node2)->connectedNodes), node1);
	if (it2 != std::end((*node2)->connectedNodes))
		(*node2)->connectedNodes.erase(it2);

}

bool Circuit::areNodesConnected(std::list<Node*>::iterator node1, std::list<Node*>::iterator node2) const {
		
	const auto it = find(std::begin((*node1)->connectedNodes), std::end((*node1)->connectedNodes), node2);
	return (it != std::end((*node1)->connectedNodes));

}

std::list<Component*>::iterator Circuit::createComponent(const QString componentName, const QPoint& pos, const bool createNodes) {
	//TODO refactor
	Component* newComponent = getComponentFromName(componentName);

	newComponent->serialNumber = 1 + std::count_if(std::begin(components), std::end(components), [componentName](Component* c) { return c->getName() == componentName; });

	newComponent->setPos(pos);

	if (createNodes) {
		for (int i = 0; i < newComponent->nodeCount(); ++i) 
			newComponent->coupledNodes.push_back(createNode(QPoint(0, 0), newComponent));
		newComponent->updateNodeOffsets();
	}
	
	components.emplace_back(newComponent);

	return std::prev(components.end());

}

void Circuit::deleteComponent(std::list<Component*>::iterator component) {

	for (const auto& it : (*component)->coupledNodes)
		deleteNode(it);

	delete *component;

	components.erase(component);

}