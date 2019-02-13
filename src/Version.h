/*

	This file is part of CircuitSimulator
	Copyright (C) 2018-2019 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Version.h
// 
//  ---------------------------------------------

#pragma once

#include <qstring.h>

constexpr int VERSION_MAJOR		= 0;
constexpr int VERSION_MINOR		= 1;
constexpr int VERSION_REVISION	= 1;

QString getVersionString();
