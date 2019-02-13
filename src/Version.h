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

namespace VersionInfo {

	struct VersionRecord {

		int Major;
		int Minor;
		int Revision;

	};

	constexpr VersionRecord Version = { 0, 1, 1 };

	QString getVersionString();

}


