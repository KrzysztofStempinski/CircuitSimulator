/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	CircuitEditor_actions.h
//
//  ---------------------------------------------

#include "CircuitEditor.h"

#include <qaction.h>

void CircuitEditor::createActions() {
	// rotateRight
	action_componentRotateRight = new QAction("Rotate right\t E", this);
	action_componentRotateRight->setIcon(QIcon("data\\icons\\editRotateRight.ico"));
	action_componentRotateRight->setShortcut(QKeySequence("E"));
	action_componentRotateRight->setData("right");
	connect(action_componentRotateRight, &QAction::triggered, this, [this] { slot_componentRotate(+90); });

	addAction(action_componentRotateRight);

	// rotateLeft
	action_componentRotateLeft = new QAction("Rotate left\t Q", this);
	action_componentRotateLeft->setIcon(QIcon("data\\icons\\editRotateLeft.ico"));
	action_componentRotateLeft->setShortcut(QKeySequence("Q"));
	action_componentRotateLeft->setData("left");
	connect(action_componentRotateLeft, &QAction::triggered, this, [this] { slot_componentRotate(-90); });

	addAction(action_componentRotateLeft);

	// delete
	action_delete = new QAction("Delete\t Del", this);
	action_delete->setIcon(QIcon("data\\icons\\editDelete.ico"));
	action_delete->setShortcut(QKeySequence::Delete);
	connect(action_delete, SIGNAL(triggered()), this, SLOT(slot_delete()));

	addAction(action_delete);

	// next node
	action_nextNode = new QAction("Next node\tSpace", this);
	action_nextNode->setShortcut(QKeySequence(Qt::Key_Space));
	connect(action_nextNode, SIGNAL(triggered()), this, SLOT(slot_nextNode()));

	addAction(action_nextNode);

	action_createMidpointNode = new QAction("Create node at midpoint\tM", this);
	action_createMidpointNode->setShortcut(QKeySequence("M"));
	connect(action_createMidpointNode, SIGNAL(triggered()), this, SLOT(slot_createMidpointNode()));

	addAction(action_createMidpointNode);
}