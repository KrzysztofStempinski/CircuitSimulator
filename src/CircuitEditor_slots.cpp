/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	CircuitEditor_slots.h
//
//  ---------------------------------------------

#include "CircuitEditor.h"

#include <QAction>

#include "Math.h"

void CircuitEditor::slot_componentRotate(int angle) {
	switch (_mode) {
	case EditorMode::idle: {
		if (mouseOverComponent != circuit.components.end())
			(*mouseOverComponent)->setRotationAngle((*mouseOverComponent)->getRotationAngle() + angle);
	}
	break;

	case EditorMode::moving:
	case EditorMode::rectSelected: {
		for (const auto& it : _selectedComponents)
			(*it)->setRotationAngle((*it)->getRotationAngle() + angle);
	}
		break;

	case EditorMode::componentCreation: {
		_currentComponent->setRotationAngle(_currentComponent->getRotationAngle() + angle);
	}
	break;
	}

	update();
}

void CircuitEditor::slot_delete() {
	switch (_mode) {
	case EditorMode::idle: {
		if (mouseOverNode != circuit.nodes.end()) {
			if (!(*mouseOverNode)->isCoupled()) {
				circuit.deleteNode(mouseOverNode);
				clearSelection();
				mouseOverNode = circuit.nodes.end();

				break;
			}
		}

		if (mouseOverComponent != circuit.components.end()) {
			circuit.deleteComponent(mouseOverComponent);
			clearSelection();
			mouseOverComponent = circuit.components.end();

			break;
		}

		if (mouseOverLink.first != circuit.nodes.end()) {
			circuit.disconnectNodes(mouseOverLink.first, mouseOverLink.second);

			if ((*mouseOverLink.first)->connectedNodes.empty() && !(*mouseOverLink.first)->isCoupled())
				circuit.deleteNode(mouseOverLink.first);
			if ((*mouseOverLink.second)->connectedNodes.empty() && !(*mouseOverLink.second)->isCoupled())
				circuit.deleteNode(mouseOverLink.second);

			mouseOverLink = std::make_pair(circuit.nodes.end(), circuit.nodes.end());

			break;
		}
	}
	break;

	case EditorMode::moving:
	case EditorMode::rectSelected: {
		for (const auto& it : _selectedComponents)
			circuit.deleteComponent(it);

		for (const auto& it : _selectedNodes)
			circuit.deleteNode(it);

		_mode = EditorMode::idle;
	}
		break;
	}

	update();
}

void CircuitEditor::slot_nextNode() {

	if (_mode == EditorMode::linkDrawing) {
		if ((*nodeLinkStart)->isCoupled()) {

			auto it = std::find(std::begin((*nodeLinkStart)->getCoupledComponent()->coupledNodes),
			                    std::end((*nodeLinkStart)->getCoupledComponent()->coupledNodes),
			                    nodeLinkStart);

			// last node, switch to first
			if (it == std::next((*nodeLinkStart)->getCoupledComponent()->coupledNodes.end(), -1))
				nodeLinkStart = (*nodeLinkStart)->getCoupledComponent()->coupledNodes.front();
			else
				nodeLinkStart = *(++it);

			update();
		}
	}

}

void CircuitEditor::slot_createMidpointNode() {

	if (mouseOverLink.first != circuit.nodes.end()) {
		QPoint p1 = (*mouseOverLink.first)->pos();
		QPoint p2 = (*mouseOverLink.second)->pos();

		auto it = circuit.createNode(Math::snapPointToGrid((p1 + p2) / 2, GRID_SIZE));
		circuit.disconnectNodes(mouseOverLink.first, mouseOverLink.second);

		circuit.connectNodes(mouseOverLink.first, it);
		circuit.connectNodes(mouseOverLink.second, it);

		setMode(EditorMode::linkDrawing);
		nodeLinkStart = it;

		update();
	}

}
