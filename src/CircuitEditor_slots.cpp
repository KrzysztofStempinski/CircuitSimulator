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
	case EditorMode::_default: {
		if (mouseOverComponent != nullptr)
			mouseOverComponent->setRotationAngle(mouseOverComponent->getRotationAngle() + angle);
	}
							   break;

	case EditorMode::moving:
	case EditorMode::rectSelected: {
		for (const auto& it : _selectedComponents)
			it->setRotationAngle(it->getRotationAngle() + angle);
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
	case EditorMode::_default: {
		if (mouseOverNode != nullptr) {
			if (!mouseOverNode->isCoupled()) {
				circuit.deleteNode(mouseOverNode);
				clearSelection();
				mouseOverNode = nullptr;

				break;
			}
		}

		if (mouseOverComponent != nullptr) {
			circuit.deleteComponent(mouseOverComponent);
			clearSelection();
			mouseOverComponent = nullptr;

			break;
		}

		if (mouseOverLink.first != nullptr) {
			mouseOverLink.first->disconnectFrom(mouseOverLink.second);

			if (mouseOverLink.first->connectedNodes.empty() && !mouseOverLink.first->isCoupled())
				circuit.deleteNode(mouseOverLink.first);
			if (mouseOverLink.second->connectedNodes.empty() && !mouseOverLink.second->isCoupled())
				circuit.deleteNode(mouseOverLink.second);

			mouseOverLink = std::make_pair(nullptr, nullptr);

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

		_mode = EditorMode::_default;
	}
								   break;
	}

	update();
}

void CircuitEditor::slot_nextNode() {
	if (_mode == EditorMode::linkDrawing) {
		if (nodeLinkStart->isCoupled()) {
			auto it = std::find(std::begin(nodeLinkStart->getCoupledComponent()->coupledNodes),
				std::end(nodeLinkStart->getCoupledComponent()->coupledNodes),
				nodeLinkStart);

			// last node, switch to first
			if (it == std::next(nodeLinkStart->getCoupledComponent()->coupledNodes.end(), -1))
				nodeLinkStart = nodeLinkStart->getCoupledComponent()->coupledNodes.front();
			else
				nodeLinkStart = *(++it);

			update();
		}
	}
}

void CircuitEditor::slot_createMidpointNode() {
	if (mouseOverLink.first != nullptr) {
		QPoint p1 = mouseOverLink.first->pos();
		QPoint p2 = mouseOverLink.second->pos();

		circuit.createNode(snapPointToGrid((p1 + p2) / 2, GRID_SIZE));
		mouseOverLink.first->disconnectFrom(mouseOverLink.second);

		mouseOverLink.first->connectTo(circuit.nodes.back());
		mouseOverLink.second->connectTo(circuit.nodes.back());

		setMode(EditorMode::linkDrawing);
		nodeLinkStart = circuit.nodes.back();

		update();
	}
}