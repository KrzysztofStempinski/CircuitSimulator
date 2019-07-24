/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Property.h
//
//  ---------------------------------------------

#pragma once

#include <qstring.h>

template <typename T>
struct PropertyGeneric {
	QString displayName;
	QString unit;
	T value;
};

using Property = PropertyGeneric<double>;

using PropertyMap = std::map<QString, Property>;