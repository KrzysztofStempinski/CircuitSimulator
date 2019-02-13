/*

	This file is part of CircuitSimulator
	Copyright (C) 2018-2019 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Version.cpp
// 
//  ---------------------------------------------

#include "Version.h"

namespace VersionInfo {

	QString getVersionString() {

		return	QString::number(Version.Major)
			+ "." + QString::number(Version.Minor)
			+ "." + QString::number(Version.Revision);

	}

}
