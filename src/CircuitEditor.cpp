/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	CircuitEditor.cpp
//
//  ---------------------------------------------

#include "Config.h"

#include "CircuitEditor.h"

#include <qwidget.h>
#include <QMenu>

#include "ComponentList.h"

void CircuitEditor::selectComponent(Component* component){
	_selectedComponents.push_back(component);
	component->selected = true;
}

void CircuitEditor::selectNode(Node* node){
	_selectedNodes.push_back(node);
	node->selected = true;
}

//
//	CircuitEditor::mousePressEvent()
//
void CircuitEditor::mousePressEvent(QMouseEvent* event){
	//if (event->button() == Qt::RightButton)
	//MouseRButtonDown(point.x(), point.y());
	/*else*/
	if (event->button() == Qt::LeftButton)
		mouseButtonLeftDown(event->pos());
}

//
// CircuitEditor::mouseReleaseEvent()
//
void CircuitEditor::mouseReleaseEvent(QMouseEvent* event){
	if (event->button() == Qt::RightButton)
		mouseButtonRightUp(event->pos());
	else if (event->button() == Qt::LeftButton)
		mouseButtonLeftUp(event->pos());
}

//
// CircuitEditor::mouseMoveEvent()
//
void CircuitEditor::mouseMoveEvent(QMouseEvent* event){
	mouseMove(event->pos());
}

//
// mouseDoubleClickEvent
//
void CircuitEditor::mouseDoubleClickEvent(QMouseEvent* event){
	if (event->button() == Qt::LeftButton)
		mouseButtonLeftDblClick(event->pos());
}

//
//	CircuitEditor::CircuitEditor()
//
CircuitEditor::CircuitEditor(){
	setMouseTracking(true);

	nodeLinkStart = nullptr;
	mouseOverNode = nullptr;
	mouseOverComponent = nullptr;

	_mode = EditorMode::idle;

	createActions();

	circuit = Circuit();
}

/*
	CircuitEditor::drawEverything()
*/
void CircuitEditor::drawEverything(QPainter& painter){
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	// draw grid
	painter.setPen(Config::Pens::grid);
	for (int i = 0; i < painter.window().right() - painter.window().left(); i += GRID_SIZE)
		for (int j = 0; j < painter.window().bottom() - painter.window().top(); j += GRID_SIZE)
			painter.drawPoint(i, j);

	// draw all the components
	for (const auto& it : circuit.components){
		if (it->selected)
			painter.setPen(Config::Pens::selected);
		else if (it == mouseOverComponent && _mode != EditorMode::linkDrawing)
			painter.setPen(Config::Pens::highlighted);
		else
			painter.setPen(Config::Pens::normal);

		it->draw(painter);
	}

	// if we're in component creation mode, draw the currently selected one
	if (_mode == EditorMode::componentCreation){
		painter.setPen(Config::Pens::outline);
		_currentComponent->draw(painter);
	}

	// draw nodes and links between them
	for (const auto& it : circuit.nodes){
		painter.setPen(Config::Pens::normal);

		for (const auto& it2 : it->connectedNodes){
			//  TODO refactor, optimize etc.
			if ((mouseOverLink.first == it2 && mouseOverLink.second == it) || (mouseOverLink.first == it && mouseOverLink.second == it2)){
				painter.setPen(Config::Pens::highlighted);

				painter.drawLine(it->pos(), it2->pos());

				painter.setPen(Config::Pens::normal);
			}
			else
				painter.drawLine(it->pos(), it2->pos());
		}

		if (it->selected)
			painter.setPen(Config::Pens::selected);
		else if (it == mouseOverNode)
			painter.setPen(Config::Pens::highlighted);
		else
			painter.setPen(Config::Pens::normal);

		it->draw(painter);
	}

	// paint the selection rect
	if (_mode == EditorMode::rectSelect){
		painter.setPen(Config::Pens::selected);
		painter.drawRect(_selectionRect);
	}

	// paint the currently drawn link
	if (_mode == EditorMode::linkDrawing){
		QPoint snappedCursor = Math::snapPointToGrid(mapFromGlobal(QCursor::pos()), GRID_SIZE);
		QPoint adjustedPosition = snappedCursor;

		painter.setPen(Config::Pens::normal);

		// we allow the user only to draw straight connections,
		// i.e. either y = y_o or x=x_o, where (x_o, y_o) is position
		// of nodeLinkStart

		// compute distances to line x=x_o and y=y_o
		int distX = std::abs(nodeLinkStart->pos().x() - snappedCursor.x());
		int distY = std::abs(nodeLinkStart->pos().y() - snappedCursor.y());

		if (distX < distY)
			adjustedPosition.setX(nodeLinkStart->pos().x());
		else
			adjustedPosition.setY(nodeLinkStart->pos().y());

		// draw the link currently being created
		if (mouseOverNode != nullptr)
			painter.drawLine(nodeLinkStart->pos(), mouseOverNode->pos());
		else
			painter.drawLine(nodeLinkStart->pos(), adjustedPosition);

		// extended crosshair
		//TODO actually implement...
		/*
		pen.setColor(Qt::green);
		pen.setWidth(1);
		pen.setStyle(Qt::DashLine);
		painter.setPen(pen);

		painter.drawLine(0, snappedCursor.y(), painter.window().right(), snappedCursor.y());
		painter.drawLine(snappedCursor.x(), 0, snappedCursor.x(), painter.window().bottom());
		*/
	}
}

/*
	isMouseOverLink
*/
std::pair<Node*, Node*> CircuitEditor::isMouseOverLink(const QPoint& mousePos){
	for (const auto& it : circuit.nodes)
		for (const auto it2 : it->connectedNodes)
			if (Math::isPointOnLine(it->pos(), it2->pos(), mousePos))
				return std::make_pair(it, it2);

	return std::make_pair(nullptr, nullptr);
}

void CircuitEditor::clearSelection(){
	_selectedNodes.clear();

	for (const auto& it : circuit.nodes)
		it->selected = false;

	_selectedComponents.clear();

	for (const auto& it : circuit.components)
		it->selected = false;
}

//
//	CircuitEditor::mouseButtonLeftDown()
//
void CircuitEditor::mouseButtonLeftDown(const QPoint& mousePos){
	switch (_mode){
	case EditorMode::linkDrawing: {
		// if we click on a node1 while drawing a link from node2, connect them together
		// and start drawing a new link from node1
		if (mouseOverNode != nullptr && mouseOverNode != nodeLinkStart){
			mouseOverNode->connectTo(nodeLinkStart);

			_mode = EditorMode::linkDrawing;
			nodeLinkStart = mouseOverNode;

			break;
		}

		// if we click on a link, create node between nodes
		// between which the link is conducted
		if (mouseOverLink.first != nullptr){
			mouseOverLink.first->disconnectFrom(mouseOverLink.second);
			circuit.createNode(Math::snapPointToGrid(mousePos, GRID_SIZE));

			circuit.nodes.back()->connectTo(mouseOverLink.first);
			circuit.nodes.back()->connectTo(mouseOverLink.second);
			circuit.nodes.back()->connectTo(nodeLinkStart);

			nodeLinkStart = circuit.nodes.back();

			break;
		}

		// if we click ona free space, create node there
		// taking into account that we can create straight lines

		QPoint adjustedPosition = Math::snapPointToGrid(mousePos, GRID_SIZE);

		const int distX = std::abs(nodeLinkStart->pos().x() - adjustedPosition.x());
		const int distY = std::abs(nodeLinkStart->pos().y() - adjustedPosition.y());

		if (distX < distY)
			adjustedPosition.setX(nodeLinkStart->pos().x());
		else
			adjustedPosition.setY(nodeLinkStart->pos().y());

		circuit.createNode(adjustedPosition);
		circuit.nodes.back()->connectTo(nodeLinkStart);

		nodeLinkStart = circuit.nodes.back();

		update();
	}
	break;

	case EditorMode::rectSelected: {
		// start dragging components/nodes around if user does lmb down on one of the selected items
		if ((mouseOverNode != nullptr && mouseOverNode->selected) || (mouseOverComponent != nullptr && mouseOverComponent->selected)){
			if (mouseOverNode != nullptr)
				_origin = mouseOverNode->pos();
			else
				_origin = mouseOverComponent->pos();

			_mode = EditorMode::moving;

			for (const auto& it : _selectedComponents)
				it->prevPos = it->pos();

			for (const auto& it : _selectedNodes)
				it->prevPos = it->pos();
		}
		else{
			clearSelection();
			_mode = EditorMode::idle;

			mouseButtonLeftDown(mousePos); // TODO temporary very ugly no good awful hack
		}
	}
	break;

	case EditorMode::componentCreation: {
		circuit.createComponent(_currentComponent->getName(), Math::snapPointToGrid(mousePos, GRID_SIZE));
		circuit.components.back()->setRotationAngle(_currentComponent->getRotationAngle());

		update();
	}
	break;

	case EditorMode::idle: {
		if (mouseOverNode != nullptr /* TODO */ && !mouseOverNode->isCoupled()){
			clearSelection();

			_selectedNodes.push_back(mouseOverNode);
			mouseOverNode->selected = true;
			mouseOverNode->prevPos = mouseOverNode->pos();
			_origin = mousePos;
			_mode = EditorMode::moving;

			update();
			break;
		}

		if (mouseOverComponent != nullptr){
			clearSelection();
			_selectedComponents.push_back(mouseOverComponent);
			mouseOverComponent->selected = true;
			mouseOverComponent->prevPos = mouseOverComponent->pos();
			_origin = mousePos;
			_mode = EditorMode::moving;

			update();
			break;
		}

		if (mouseOverLink.first != nullptr){
			clearSelection();

			_mode = EditorMode::linkMoving;

			_origin = mousePos;

			linkToMove.first = mouseOverLink.first;
			linkToMove.second = mouseOverLink.second;

			linkToMove.first->prevPos = linkToMove.first->pos();
			linkToMove.second->prevPos = linkToMove.second->pos();

			update();
			break;
		}

		clearSelection();
		_mode = EditorMode::rectSelect;
		_rectStartPoint = mousePos;
	}
	break;
	}
}

//
// CircuitEditor::mouseMove()
//
void CircuitEditor::mouseMove(const QPoint& mousePos){
	mouseOverLink = isMouseOverLink(mousePos);

	mouseOverNode = nullptr;
	mouseOverComponent = nullptr;

	// cycle throught all the nodes/components to check whether cursor is over one
	for (auto it = std::begin(circuit.nodes); it != std::end(circuit.nodes); ++it){
		if ((*it)->isMouseOver(mousePos)){
			mouseOverNode = (*it);
			update();
			break;
		}
	}

	// we only check whether we are over component if mouse isn't over node
	if (mouseOverNode == nullptr){
		for (auto it = std::begin(circuit.components); it != std::end(circuit.components); ++it){
			if ((*it)->isMouseOver(mousePos)){
				mouseOverComponent = (*it);
				update();
				break;
			}
		}
	}

	//redraw, just so that nothing remains highlighted
	if (mouseOverNode == nullptr)
		if (mouseOverComponent == nullptr)
			update();

	switch (_mode){
	case EditorMode::componentCreation: {
		_currentComponent->setPos(Math::snapPointToGrid(mousePos, GRID_SIZE));
		update();
	}
	break;

	case EditorMode::linkMoving: {
		if (!linkToMove.first->isCoupled() && !linkToMove.second->isCoupled()){
			linkToMove.first->setPos(Math::snapPointToGrid(linkToMove.first->prevPos + mousePos - _origin, GRID_SIZE));
			linkToMove.second->setPos(Math::snapPointToGrid(linkToMove.second->prevPos + mousePos - _origin, GRID_SIZE));

			update();
		}
	}
	break;

	case EditorMode::moving: {
		const QPoint dif = mousePos - _origin;

		for (const auto& it : _selectedComponents)
			it->setPos(Math::snapPointToGrid(it->prevPos + dif, GRID_SIZE));

		for (const auto& it : _selectedNodes)
			if (!it->isCoupled())
				it->setPos(Math::snapPointToGrid(it->prevPos + dif, GRID_SIZE));

		update();
	}
	break;

	case EditorMode::rectSelect: {
		clearSelection();

		_selectionRect = QRect(_rectStartPoint, mousePos);

		for (const auto& it : circuit.nodes){
			if (it->isWithinRectangle(_selectionRect) && /*TODO nodes!*/ !it->isCoupled()){
				it->selected = true;
				_selectedNodes.push_back(it);
			}
			else
				it->selected = false;
		}

		for (const auto& it : circuit.components){
			if (it->isWithinRectangle(_selectionRect)){
				it->selected = true;

				_selectedComponents.push_back(it);

				for (const auto& nt : it->coupledNodes)
					nt->selected = true;
			}
			else{
				it->selected = false;

				for (const auto& nt : it->coupledNodes)
					nt->selected = false;
			}
		}
	}
	break;
	}
}

//
// CircuitEditor::mouseButtonLeftUp()
//
void CircuitEditor::mouseButtonLeftUp(const QPoint& mousePos){
	switch (_mode){
	case EditorMode::linkMoving: {
		_mode = EditorMode::idle;
	}
	break;

		// if we just stopped drawing the selection rect
	case EditorMode::rectSelect: {
		// if we have some components selected,
		if (!(_selectedNodes.empty() && _selectedComponents.empty()))
			_mode = EditorMode::rectSelected;
		else
			_mode = EditorMode::idle;
	}
	break;

		// if we we're moving components/nodes
	case EditorMode::moving: {
		// if we have more than one element selected
		if (_selectedNodes.size() > 1 || _selectedComponents.size() > 1)
			_mode = EditorMode::rectSelected;
		else{
			clearSelection();
			_mode = EditorMode::idle;
		}
	}
	break;
	}

	update();
}

//
//	CircuitEditor::mouseButtonLeftDblClick()
//
void CircuitEditor::mouseButtonLeftDblClick(const QPoint& mousePos){
	clearSelection();

	if (mouseOverNode != nullptr){
		_mode = EditorMode::linkDrawing;
		nodeLinkStart = mouseOverNode;
	}

	if (mouseOverLink.first != nullptr){
		circuit.createNode(Math::snapPointToGrid(mousePos, GRID_SIZE));

		mouseOverLink.first->connectTo(circuit.nodes.back());
		mouseOverLink.second->connectTo(circuit.nodes.back());

		mouseOverLink.first->disconnectFrom(mouseOverLink.second);

		_mode = EditorMode::linkDrawing;
		nodeLinkStart = circuit.nodes.back();
	}

	update();
}

//
// CircuitEditor::paintEvent()
//
void CircuitEditor::paintEvent(QPaintEvent*){
	QPainter painter(this);
	drawEverything(painter);
}

void CircuitEditor::setCurrentComponent(const QString component){
	_currentComponent = getComponentFromName(component);
}

void CircuitEditor::mouseButtonRightUp(const QPoint& mousePos){
	switch (_mode){
		// if drawing a link, switch back to idle WITHOUT displaying context menu
	case EditorMode::linkDrawing: {
		nodeLinkStart = nullptr;
		_mode = EditorMode::idle;
		break;
	}
		// if creating a component, do as above
	case EditorMode::componentCreation: {
		_mode = EditorMode::idle;

		// cleanup
		delete _currentComponent;
	}
	break;

		// if drawing selection rect, abort
	case EditorMode::rectSelect: {
		_mode = EditorMode::idle;
		clearSelection();
	}
	break;

		// if we have selected items, always display context menu
	case EditorMode::rectSelected: {
		if ((mouseOverNode != nullptr && mouseOverNode->selected) || (mouseOverComponent != nullptr && mouseOverComponent->selected))
			displayContextMenu(mapToGlobal(mousePos));
		else{
			clearSelection();
			update();
			displayContextMenu(mapToGlobal(mousePos));
		}
		break;
	}

		// display the context menu in all other cases
	default: {
		if (mouseOverNode != nullptr /*TODO */ && !mouseOverNode->isCoupled())
			_selectedNodes.push_back(mouseOverNode);
		else if (mouseOverComponent != nullptr)
			_selectedComponents.push_back(mouseOverComponent);

		displayContextMenu(mapToGlobal(mousePos));
	}
		break;
	}

	update();
}

void CircuitEditor::displayContextMenu(const QPoint& mousePos){
	QMenu menu(this);

	if (!_selectedComponents.empty() || !_selectedNodes.empty())
		menu.addAction("Delete");

	if (_selectedComponents.size() == 1)
		menu.addAction("Properties");

	menu.exec(mousePos);
}

EditorMode CircuitEditor::mode() const{
	return _mode;
}

void CircuitEditor::setMode(const EditorMode newMode){
	_mode = newMode;
}
