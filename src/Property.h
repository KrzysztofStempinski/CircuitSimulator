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

// TODO use templates (or something like that), so that component
// properties can not only be doubles, but, say, strings and bools as well

#include <qstring.h>

struct Property {

	QString displayName;
	QString unit;
	double value;

		Property(QString newDisplayName, QString newUnit, double newValue) 
			: displayName(newDisplayName), unit(newUnit), value(newValue) {};

		Property() : displayName("error"), unit("error"), value(0.0) {};

};
