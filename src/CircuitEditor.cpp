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

#include "Log.h"

#include "CircuitEditor.h"

#include <qwidget.h>
#include <QMenu>

void CircuitEditor::selectComponent(Component* component) {
	_selectedComponents.push_back(component);
	component->isSelected = true;
}

void CircuitEditor::selectNode(Node* node) {
	_selectedNodes.push_back(node);
	node->isSelected = true;
}

//
//	CircuitEditor::mousePressEvent()
//
void CircuitEditor::mousePressEvent(QMouseEvent* event) {

	//if (event->button() == Qt::RightButton) 
		//MouseRButtonDown(point.x(), point.y());
	/*else*/ if (event->button() == Qt::LeftButton) 
		mouseButtonLeftDown(event->pos());

}

// 
// CircuitEditor::mouseReleaseEvent()
//
void CircuitEditor::mouseReleaseEvent(QMouseEvent* event) {

	if (event->button() == Qt::RightButton) 
		mouseButtonRightUp(event->pos());
	else if (event->button() == Qt::LeftButton) 
		mouseButtonLeftUp(event->pos());

}

// 
// CircuitEditor::mouseMoveEvent()
//
void CircuitEditor::mouseMoveEvent(QMouseEvent* event) {
	mouseMove(event->pos());
}

// 
// mouseDoubleClickEvent
//
void CircuitEditor::mouseDoubleClickEvent(QMouseEvent* event) {

	if (event->button() == Qt::LeftButton) 
		mouseButtonLeftDblClick(event->pos());

}

// 
//	CircuitEditor::CircuitEditor()
// 
CircuitEditor::CircuitEditor() {

	nodeLinkStart = nullptr;

	mouseOverNode = nullptr;

	mouseOverComponent = nullptr;

	_currentComponent = "resistor";

    _mode = EditorMode::_default;

	setMouseTracking(true); 
	circuit = Circuit();

	_scale = 1.0;

	createActions();

}

//
// CircuitEditor::drawEverything()
//
void CircuitEditor::drawEverything(QPainter &painter) {

	QColor colorNormal, colorHighlighted, colorSelected;

	colorNormal = Qt::cyan;
	colorHighlighted = Qt::blue;
	colorSelected = Qt::red;

	painter.translate(QPoint(_scale, _scale));
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	QPen pen;

	// draw grid
	pen.setColor(Qt::darkGray);
	pen.setWidth(1);
	painter.setPen(pen);
	for (int i = 0; i < painter.window().right() - painter.window().left(); i += GRID_SIZE)
		for (int j = 0; j < painter.window().bottom() - painter.window().top(); j += GRID_SIZE)
			painter.drawPoint(i, j);

	// draw all the components
	for (auto it = std::begin(circuit.components); it != std::end(circuit.components); ++it) {

		if ((*it)->isSelected) {
			pen.setColor(colorSelected);
			pen.setWidth(2);
		}
		else if ((*it) == mouseOverComponent && _mode != EditorMode::linkDrawing) {
			pen.setColor(colorHighlighted);
			pen.setWidth(2);
		}
		else {
			pen.setColor(colorNormal);
			pen.setWidth(1);
		}

		painter.setPen(pen);

		(*it)->draw(painter);
	}


	// if we're in component creation mode, draw the currently selected one
	if (_mode == EditorMode::componentCreation) {
		pen.setColor(Qt::lightGray);
		pen.setWidth(1);
		painter.setPen(pen);
		for (auto &it : _tempComponentGeometry)
			it->draw(painter, _tempComponentPos, _tempComponentRotationAngle);
	}

	// draw nodes and links between them
	for (auto it = std::begin(circuit.nodes); it != std::end(circuit.nodes); ++it) {

		pen.setColor(colorNormal);
		pen.setWidth(1);
		painter.setPen(pen);

		for (const auto &it2 : (*it)->connectedNodes) {
			//  TODO refactor, optimize etc.
			if ((mouseOverLink.first == it2 && mouseOverLink.second == (*it)) || (mouseOverLink.first == (*it) && mouseOverLink.second == it2)) {

				pen.setColor(colorHighlighted);
				pen.setWidth(2);
				painter.setPen(pen);

				painter.drawLine((*it)->getPos(), it2->getPos());

				pen.setColor(colorNormal);
				pen.setWidth(1);
				painter.setPen(pen);

			} else {
				painter.drawLine((*it)->getPos(), it2->getPos());
			}

		}


		if ((*it)->isSelected) {
			pen.setColor(colorSelected);
			pen.setWidth(2);
		} else if ((*it) == mouseOverNode) {
			pen.setColor(colorHighlighted);
			pen.setWidth(2);
		} else {
			pen.setColor(colorNormal);
			pen.setWidth(1);
		}

		painter.setPen(pen);

		(*it)->draw(&painter);

	}

	// paint the selection rect
	if (_mode == EditorMode::rectSelect) {
		pen.setColor(colorSelected);
		painter.setPen(pen);
		painter.drawRect(_selectionRect);
	}

	// paint the currently drawn link
	if (_mode == EditorMode::linkDrawing) {

		pen.setColor(colorNormal);
		painter.setPen(pen); 

		if (mouseOverNode != nullptr) 
			painter.drawLine(nodeLinkStart->getPos(), mouseOverNode->getPos());
		else 
			painter.drawLine(nodeLinkStart->getPos(), snapPointToGrid(mapFromGlobal(QCursor::pos()), GRID_SIZE));

	}

}

// 
//	isMouseOverLink
//
std::pair<Node*, Node*> CircuitEditor::isMouseOverLink(const QPoint& mousePos) {

	for (const auto &it : circuit.nodes) 
		for (const auto it2 : it->connectedNodes) 
			if (isPointOnLine(it->getPos(), it2->getPos(), mousePos)) 
				return std::make_pair(it, it2);

	return std::make_pair(nullptr, nullptr);

}

void CircuitEditor::clearSelection() {

	_selectedNodes.clear();

	for (const auto &it : circuit.nodes) 
		it->isSelected = false;

	_selectedComponents.clear();

	for (const auto &it : circuit.components) 
		it->isSelected = false;

}


// 
//	CircuitEditor::mouseButtonLeftDown()
//
void  CircuitEditor::mouseButtonLeftDown(const QPoint& mousePos) {

	switch (_mode) {

		case EditorMode::linkDrawing: {

			// if we click on a node1 while drawing a link from node2, connect them together
			// and start drawing a new link from node1
			if (mouseOverNode != nullptr && mouseOverNode != nodeLinkStart) {

				mouseOverNode->connectTo(nodeLinkStart);

				_mode = EditorMode::linkDrawing;
				nodeLinkStart = mouseOverNode;

				break;

			} 
		
			// if we click on a link, create node between nodes 
			// between which the link is conducted
			if (mouseOverLink.first != nullptr) {

				mouseOverLink.first->disconnectFrom(mouseOverLink.second);
				circuit.createNode(snapPointToGrid(mousePos, GRID_SIZE));

				circuit.nodes.back()->connectTo(mouseOverLink.first);
				circuit.nodes.back()->connectTo(mouseOverLink.second);
				circuit.nodes.back()->connectTo(nodeLinkStart);

				nodeLinkStart = circuit.nodes.back();

				break;

			}

			// if we click ona free space, create node there
			circuit.createNode(snapPointToGrid(mousePos, GRID_SIZE));
			circuit.nodes.back()->connectTo(nodeLinkStart);

			nodeLinkStart = circuit.nodes.back();

			update();

		} 
		break;

		case EditorMode::rectSelected: {

			// start dragging components/nodes around if user does lmb down on one of the selected items
			if ((mouseOverNode != nullptr && mouseOverNode->isSelected) || (mouseOverComponent != nullptr && mouseOverComponent->isSelected)) {

				if (mouseOverNode != nullptr) 
					_origin = mouseOverNode->getPos();
				else 
					_origin = mouseOverComponent->getPos();

				_mode = EditorMode::moving;

				for (const auto& it : _selectedComponents) 
					it->prevPos = it->getPos();
			
				for (const auto& it : _selectedNodes)
					it->prevPos = it->getPos();

			} else {

				clearSelection();
                _mode = EditorMode::_default;

				mouseButtonLeftDown(mousePos); // TODO temporary very ugly no good awful hack

			}

		} 
		break;

		case EditorMode::componentCreation: {
		
			circuit.createComponent(_currentComponent, snapPointToGrid(mousePos, GRID_SIZE));
			circuit.components.back()->setRotationAngle(_tempComponentRotationAngle);

			update();
	
		} 
		break;

        case EditorMode::_default:
		{

			if (mouseOverNode != nullptr /* TODO */ && !mouseOverNode->isCoupled()) {

				clearSelection();


				_selectedNodes.push_back(mouseOverNode);
				mouseOverNode->isSelected = true;
				mouseOverNode->prevPos = mouseOverNode->getPos();
				_origin = mousePos;
				_mode = EditorMode::moving;

				update();
				break;

			}

			if (mouseOverComponent != nullptr) {

				clearSelection();
				_selectedComponents.push_back(mouseOverComponent);
				mouseOverComponent->isSelected = true;
				mouseOverComponent->prevPos = mouseOverComponent->getPos();
				_origin = mousePos;
				_mode = EditorMode::moving;

				update();
				break;

			}

			if (mouseOverLink.first != nullptr) {

				clearSelection();

				_mode = EditorMode::linkMoving;

				_origin = mousePos;

				linkToMove.first = mouseOverLink.first;
				linkToMove.second = mouseOverLink.second;

				linkToMove.first->prevPos = linkToMove.first->getPos();
				linkToMove.second->prevPos = linkToMove.second->getPos();

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
void CircuitEditor::mouseMove(const QPoint& mousePos) {

	mouseOverLink = isMouseOverLink(mousePos); 

	mouseOverNode = nullptr;
	mouseOverComponent = nullptr;

	// cycle throught all the nodes/components to check whether cursor is over one
	for (auto it = std::begin(circuit.nodes); it != std::end(circuit.nodes); ++it) {
		if ((*it)->isMouseOver(mousePos)) {
			mouseOverNode = (*it);
			update();
			break;
		}
	}

	// we only check whether we are over component if mouse isn't over node
	if (mouseOverNode == nullptr) {
		for (auto it = std::begin(circuit.components); it != std::end(circuit.components); ++it){
			if ((*it)->isMouseOver(mousePos)) {
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

	switch (_mode) {
		case EditorMode::componentCreation: {

			_tempComponentPos = snapPointToGrid(mousePos, GRID_SIZE);
			update();

		}
		break;

		case EditorMode::linkMoving: {

			if (!linkToMove.first->isCoupled() && !linkToMove.second->isCoupled()) {

				linkToMove.first->setPos(snapPointToGrid(linkToMove.first->prevPos + mousePos - _origin, GRID_SIZE));
				linkToMove.second->setPos(snapPointToGrid(linkToMove.second->prevPos + mousePos - _origin, GRID_SIZE));

				update();

			}
		}
		break;

		case EditorMode::moving: {

			const QPoint dif = mousePos - _origin;

			for (const auto& it : _selectedComponents) 
				it->setPos(snapPointToGrid(it->prevPos + dif, GRID_SIZE));

			for (const auto& it : _selectedNodes) 
				if (!it->isCoupled()) 
					it->setPos(snapPointToGrid(it->prevPos + dif, GRID_SIZE));

			update();

		}
		break;

		case EditorMode::rectSelect: {

			clearSelection();

			_selectionRect = QRect(_rectStartPoint, mousePos);

			for (const auto& it : circuit.nodes) {

				if (it->isWithinRectangle(_selectionRect) && /*TODO nodes!*/ !it->isCoupled()) {

					it->isSelected = true;
					_selectedNodes.push_back(it);

				} else 
					it->isSelected = false;

			}

			for (const auto& it : circuit.components) {

				if (it->isWithinRectangle(_selectionRect)) {

					it->isSelected = true;

					_selectedComponents.push_back(it);

					for (const auto &nt : it->coupledNodes) 
						nt->isSelected = true;

				} else {

					it->isSelected = false;

					for (const auto &nt : it->coupledNodes)
						nt->isSelected = false;


				}

			}

		}
		break;

	}

}

// 
// CircuitEditor::mouseButtonLeftUp()
//
void  CircuitEditor::mouseButtonLeftUp(const QPoint& mousePos) {

	switch (_mode) {

		case EditorMode::linkMoving: {
            _mode = EditorMode::_default;
		}
		break;

		// if we just stopped drawing the selection rect
		case EditorMode::rectSelect: {

			// if we have some components selected, 
            if (!(_selectedNodes.empty() && _selectedComponents.empty()))
				_mode = EditorMode::rectSelected;
			else
                _mode = EditorMode::_default;

		}
		break;

		// if we we're moving components/nodes
		case EditorMode::moving: {

			// if we have more than one element selected
				if (_selectedNodes.size() > 1 || _selectedComponents.size() > 1)
					_mode = EditorMode::rectSelected;
				else {
					clearSelection();
                    _mode = EditorMode::_default;
				}

		}		
		break;

	}

	update();

}

// 
//	CircuitEditor::mouseButtonLeftDblClick()
//
void  CircuitEditor::mouseButtonLeftDblClick(const QPoint& mousePos) {

	clearSelection();

	if (mouseOverNode != nullptr) {
		_mode = EditorMode::linkDrawing;
		nodeLinkStart = mouseOverNode;
	}

	if (mouseOverLink.first != nullptr) {

		circuit.createNode(snapPointToGrid(mousePos, GRID_SIZE));

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
void CircuitEditor::paintEvent(QPaintEvent *) {

	QPainter painter(this);
	painter.scale(_scale, _scale);

	drawEverything(painter);

}

void CircuitEditor::setCurrentComponent(QString component) {

	_currentComponent = component;

	// load component geometry from file
	QString fileName = "data\\components\\" + component + ".json"; 

	// TODO error handling
	std::ifstream ifs(fileName.toStdString());
	rapidjson::IStreamWrapper isw(ifs);

	rapidjson::Document componentData;
	componentData.ParseStream(isw);

	loadGeometryFromJSON(componentData["display"], _tempComponentGeometry);
	_tempComponentRotationAngle = 0;

}

void CircuitEditor::mouseButtonRightUp(const QPoint& mousePos) {

	switch (_mode) {
	
		// if drawing a link, switch back to idle WITHOUT displaying context menu
		case EditorMode::linkDrawing: {

			nodeLinkStart = nullptr;
            _mode = EditorMode::_default;

		}
		break;

		// if creating a component, do as above
		case EditorMode::componentCreation: {

            _mode = EditorMode::_default;

			// cleanup
			for (auto &it : _tempComponentGeometry)
				delete it;

		}
		break;

		// if drawing selection rect, abort
		case EditorMode::rectSelect: {

            _mode = EditorMode::_default;
			clearSelection();

		}
		break;

		// if we have selected items, always display context menu
		case EditorMode::rectSelected: {
			
			if ((mouseOverNode != nullptr && mouseOverNode->isSelected) || (mouseOverComponent != nullptr && mouseOverComponent->isSelected))
				displayContextMenu(mapToGlobal(mousePos));
			else {
				clearSelection();
				update();
				displayContextMenu(mapToGlobal(mousePos));
			}
		
		}
		break;

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

void CircuitEditor::displayContextMenu(const QPoint& mousePos) {

	QMenu menu(this);

	if (!_selectedComponents.empty() || !_selectedNodes.empty()) 
		menu.addAction("Delete");
	

	if (_selectedComponents.size() == 1) 
		menu.addAction("Properties");
	

	menu.exec(mousePos);

}

void CircuitEditor::wheelEvent(QWheelEvent * event){

	_scale += (event->delta() / WHEEL_STEP); 

	if (_scale < 1.0)
		_scale = 1.0;

	update();

}

EditorMode CircuitEditor::getMode() {
	return _mode;
}

void CircuitEditor::setMode(EditorMode newMode) {
	_mode = newMode;
}
