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

#include <string>

#include "Component.h"
#include "Math.h"

QString Component::getName() {
	return _name;
}

void Component::setPos(const QPoint& newPos) {
	_pos = newPos;

	for (const auto& it : coupledNodes)
		it->updatePos();
};

int Component::getRotationAngle() const {
	return this->_rotationAngle;
}

void Component::setRotationAngle(const int angle) {
	// division by zero is apparently bad
	if (angle == 0)
		_rotationAngle = 0;
	else
		_rotationAngle = angle % (Math::sign(angle) * 360);

	// NOTE we assume that the only rotation angles will be 0, +-90, +=180, +-270, +-360
	if (_rotationAngle % 180 != 0)
		_boundingRect = _boundingRect.transposed();

	//  update node positions
	setPos(_pos);
}

void Component::saveToJSON(rapidjson::Value& arrayComponents, rapidjson::Document::AllocatorType& allocator) {
	rapidjson::Value valueComponent;
	valueComponent.SetObject();

	valueComponent.AddMember("ID", ID, allocator);

	rapidjson::Value name(_name.toUtf8(), _name.size(), allocator);
	valueComponent.AddMember("name", name, allocator);

	rapidjson::Value position(rapidjson::kArrayType);
	position.PushBack(_pos.x(), allocator);
	position.PushBack(_pos.y(), allocator);
	valueComponent.AddMember("position", position, allocator);

	valueComponent.AddMember("rotationAngle", _rotationAngle, allocator);

	// TODO save properties to file
	/*if (!properties.empty()) {
		rapidjson::Value propertiesVal;
		for (auto& it : properties) {
			rapidjson::Value valueProperty(it.first.toUtf8(), it.second.value, allocator);
			propertiesVal.AddMember(, valueProperty);
		}

		valueComponent.AddMember("properties", valueComponent, allocator);
	}*/

	arrayComponents.PushBack(valueComponent, allocator);
}