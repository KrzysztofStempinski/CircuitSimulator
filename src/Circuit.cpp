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

	// TODO get rid of _isCoupled field, it's not necessary
	nodes.push_back(new Node(pos, _isCoupled, _coupledComponent));

}

void Circuit::deleteNode(Node* node) {

	node->removeInboundLinks();

	delete node;

	nodes.erase(std::find(std::begin(nodes), std::end(nodes), node));

}

void Circuit::createComponent(QString componentName, const QPoint& pos, bool createNodes) {

	Component* newComponent = new Component(componentName);

	int count = std::count_if(std::begin(components), std::end(components), [componentName](Component* c) { return c->getName() == componentName; });
	// I actually find lambda expressions sexy, in a particular way
	count++;

	// TODO why is this shit hack necessary
	if (count == 0)
		count = 1;

	newComponent->serialNumber = count;

	newComponent->setPos(pos);

	if (createNodes) {

		for (int i = 0; i < newComponent->getNumberOfNodes(); ++i) {
			createNode(QPoint(0, 0), true, newComponent);
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
	
