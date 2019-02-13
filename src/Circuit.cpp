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

void Circuit::createNode(const QPoint& pos, bool _isCoupled, Component* _coupledComponent) {

	//TODO complete refactoring
	Node* newNode = new Node(pos, _isCoupled, _coupledComponent);

	nodes.push_back(newNode);

}

std::list<Node*>::iterator Circuit::deleteNode(std::list<Node*>::iterator node){

	(*node)->removeInboundLinks();

	delete (*node);

	return nodes.erase(node);

}


std::list<Node*>::iterator Circuit::deleteNode(Node* node) {

	node->removeInboundLinks();

	delete node;

	return nodes.erase(std::find(std::begin(nodes), std::end(nodes), node));

}

void Circuit::createComponent(QString componentName, const QPoint& pos, bool createNodes){
	
	Component* newComponent = new Component(componentName);

	if (createNodes) {
		for (int i = 0; i < newComponent->getNumberOfNodes(); ++i) {
			createNode(QPoint(0, 0), true, newComponent);
			newComponent->coupledNodes.push_back(nodes.back());
		}
	}

	int count = std::count_if(std::begin(components), std::end(components), [componentName](Component* c) { return c->getName() == componentName; });
	// I actually find lambda expressions sexy, in a particular way
	count++;

	// TODO why is this shit hack necessary
	if (count == 0)
		count = 1;

	newComponent->serialNumber = count;

	newComponent->setPos(pos);
	newComponent->updateNodeOffsets();

	components.push_back(newComponent);

}

void Circuit::deleteComponent(std::list<Component*>::iterator component) {
	
	for (const auto &jt : (*component)->coupledNodes) {
		for (auto it = std::begin(nodes); it != std::end(nodes); /**/) {

			if (jt == (*it)) {
				it = deleteNode(it);
				break;
			} else {
				++it;
			}

		}
	}

	delete (*component);

	components.erase(component);

}


void Circuit::deleteComponent(Component* component) {

	for (const auto &jt : component->coupledNodes) {
		for (auto it = std::begin(nodes); it != std::end(nodes); /**/) {

			if (jt == (*it)) {
				it = deleteNode(it);
				break;
			}
			else {
				++it;
			}

		}
	}

	delete component;

	components.erase(std::find(std::begin(components), std::end(components), component));

}
