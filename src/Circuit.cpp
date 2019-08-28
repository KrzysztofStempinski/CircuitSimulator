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

void Circuit::createNode(const QPoint& pos, Component* _coupledComponent) {
	nodes.push_back(new Node(pos, _coupledComponent));
}

void Circuit::deleteNode(Node* node) {
	node->removeInboundLinks();

	delete node;

	nodes.erase(std::find(std::begin(nodes), std::end(nodes), node));
}

void Circuit::createComponent(std::string componentName, const QPoint& pos, const bool createNodes) {
	
	auto newComponent = new Component(componentName);

	newComponent->serialNumber = 1 + std::count_if(std::begin(components), std::end(components), [componentName](Component* c) { return c->name() == componentName; });

	newComponent->setPos(pos);

	if (createNodes) {
		for (int i = 0; i < newComponent->nodeCount(); ++i) {
			createNode(QPoint(0, 0), newComponent);
			newComponent->coupledNodes.push_back(nodes.back());
		}

		newComponent->updateNodeOffsets();
	}

	components.push_back(newComponent);
}

void Circuit::deleteComponent(Component* component) {
	for (int i = 0; i < std::size(component->coupledNodes); ++i) {
		for (int j = 0; j < std::size(nodes); /**/) {
			if (nodes[j] == component->coupledNodes[i]) {
				deleteNode(nodes[j]);
				break;
			}
			else {
				++j;
			}
		}
	}

	delete component;

	components.erase(std::find(std::begin(components), std::end(components), component));
}