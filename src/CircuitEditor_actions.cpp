#include "CircuitEditor.h"

#include "Math.h"

#include <qaction.h>

void CircuitEditor::createActions() {

	// rotateRight
	action_componentRotateRight = new QAction("Rotate right\t E", this);
	action_componentRotateRight->setIcon(QIcon("data\\icons\\editRotateRight.ico"));
	action_componentRotateRight->setShortcut(QKeySequence("E"));	
	action_componentRotateRight->setData("right");
	connect(action_componentRotateRight, &QAction::triggered, this, [this] { slot_componentRotate(+90); }); // C++17 lambdas + Qt5. It's better than sex.

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

}

void CircuitEditor::createMidpointNode() {

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


void CircuitEditor::connectToClosest() {

	if (_mode == EditorMode::linkDrawing) {
		// TODO decent variable names
		int dist1 = std::numeric_limits<int>::infinity();
		int index = -1;

		for (int i = 0; i < circuit.nodes.size(); ++i) {

			int dist = QPoint::dotProduct(circuit.nodes[i]->pos() - mapFromGlobal(QCursor::pos()), circuit.nodes[i]->pos() - mapFromGlobal(QCursor::pos()));

			if (dist < dist1) {
				dist1 = dist;
				index = i;
			}

		}
		
		if (index = -1) //TODO when will that happen?
			return;
			
		
		nodeLinkStart->connectTo(circuit.nodes[index]);
		nodeLinkStart = circuit.nodes[index];

		update();

	}

}

void CircuitEditor::deleteElement() {



}