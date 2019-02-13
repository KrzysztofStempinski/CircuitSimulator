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

QString getVersionString(){

	return	QString::number(VERSION_MAJOR)			
				+ "." + QString::number(VERSION_MINOR)	
				+ "." + QString::number(VERSION_REVISION);

}