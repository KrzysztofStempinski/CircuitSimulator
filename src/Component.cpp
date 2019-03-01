/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Component.cpp
// 
//  ---------------------------------------------

#include <iomanip>
#include <string>

#include "Component.h"

QString Component::getName() {
	return _name;
}

void Component::setPos(const QPoint& newPos){

	_pos = newPos;

	for (const auto& it : coupledNodes)
		it->updatePos();

};

bool Component::isMouseOver(const QPoint& mousePos) const {

	return (abs(mousePos.x() - _pos.x()) <= _boundingRect.width() / 2 && abs(mousePos.y() - _pos.y()) <= _boundingRect.height() / 2);

}

bool Component::isWithinRectangle(const QRect& rect) const {

	return _boundingRect.translated(_pos).intersects(rect);

}

const QPoint Component::pos() const {

	return _pos;

}

int Component::getRotationAngle() const {

	return this->_rotationAngle;

}

void Component::setRotationAngle(const int angle) {

	// division by zero is apparently bad
	if (angle == 0)
		_rotationAngle = 0;
	else 
		_rotationAngle = angle % (sign(angle) * 360);

	// NOTE we assume that the only rotation angles will be 0, +-90, +=180, +-270, +-360
	if (_rotationAngle % 180 != 0)
		_boundingRect = _boundingRect.transposed();

	//  update node positions
	setPos(_pos);

}

void Component::saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator) {
	/*
	rapidjson::Value valueComponent;
	valueComponent.SetObject();

	valueComponent.AddMember("ID", ID, allocator);

	rapidjson::Value name(_name.toStdString().c_str(), _name.toStdString().size(), allocator); // TODO wtf is with those strings
	valueComponent.AddMember("name", name, allocator);

	rapidjson::Value position(rapidjson::kArrayType);
	position.PushBack(_pos.x(), allocator);
	position.PushBack(_pos.y(), allocator);
	valueComponent.AddMember("position", position, allocator);

	valueComponent.AddMember("rotationAngle", _rotationAngle, allocator);

	if (!properties.empty()) {

		rapidjson::Value arrayProperties(rapidjson::kArrayType);

		for (auto &it : properties)
			arrayProperties.PushBack(it.second.value, allocator);

		valueComponent.AddMember("properties", arrayProperties, allocator);

	}

	arrayComponents.PushBack(valueComponent, allocator);
	*/
}
