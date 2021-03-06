/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	CircuitEditor.h
//
//  ---------------------------------------------

#pragma once

#include "Circuit.h"

#include <qwidget.h>
#include <qevent.h>
#include <qpoint.h>

#include <optional>

constexpr int GRID_SIZE = 8;

enum class EditorMode {
	idle,
	canvasDragging,
	linkDrawing,
	rectSelect,
	rectSelected,
	moving,
	nodeCreation,
	componentCreation,
	linkMoving
};

class CircuitEditor : public QWidget {
	// TODO refactor, private/public etc.

	Q_OBJECT

private:

	Component* _currentComponent;
	QPoint _rectStartPoint;

	QRect _selectionRect;

	QPoint _origin;

	std::list<std::list<Node*>::iterator> _selectedNodes;
	std::list<std::list<Component*>::iterator> _selectedComponents;

	EditorMode _mode;

public:

	CircuitEditor();

	Circuit circuit;

	void createActions();

	// public Qt actions to use by anyone
	QAction* action_componentRotateRight;
	QAction* action_componentRotateLeft;
	QAction* action_delete;
	QAction* action_nextNode;
	QAction* action_createMidpointNode;

	std::list<Node*>::iterator nodeLinkStart;
	std::list<Node*>::iterator mouseOverNode;
	std::list<Component*>::iterator mouseOverComponent;

	std::pair<std::list<Node*>::iterator, std::list<Node*>::iterator> linkToMove;
	std::pair<std::list<Node*>::iterator, std::list<Node*>::iterator> mouseOverLink;

	void selectComponent(std::list<Component*>::iterator component);
	void selectNode(std::list<Node*>::iterator node);

	void mouseButtonLeftDown(const QPoint& mousePos);
	void mouseButtonLeftUp(const QPoint& mousePos);
	void mouseButtonLeftDblClick(const QPoint& mousePos);
	void mouseButtonRightUp(const QPoint& mousePos);

	void mouseMove(const QPoint& mousePos);

	void clearSelection();

	// TODO refactor
	std::pair<std::list<Node*>::iterator, std::list<Node*>::iterator> isMouseOverLink(const QPoint& mousePos);

	EditorMode mode() const;
	void setMode(const EditorMode newMode);

	void setCurrentComponent(const QString component);

	void drawEverything(QPainter& painter);
	void paintEvent(QPaintEvent*) override;

	void displayContextMenu(const QPoint& mousePos);

public:

signals:

public slots:

	void slot_createMidpointNode();
	void slot_componentRotate(int angle);
	void slot_delete();
	void slot_nextNode();

	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
};
