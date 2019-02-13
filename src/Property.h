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

		PropertyGeneric(QString newDisplayName, QString newUnit, T newValue) 
			: displayName(newDisplayName), unit(newUnit), value(newValue) {};

		PropertyGeneric() : displayName("error"), unit("error"), value(T(0)) {};

};

using Property = PropertyGeneric<double>;