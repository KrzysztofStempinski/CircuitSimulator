/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Simulation.h
// 
//  ---------------------------------------------

// it is customary to name engines 
// so 
// this is
//
// juliE numerical circuit simulation engine
//
// (name because of 
// various reasons)

#pragma once

#include "Circuit.h"
#include "SimulationErrors.h"

enum class SimulationMode {

	DCOP,
	DCsweep,
	Transient

};


void markAdjacentNodes(Node* nodeBegin, int voltageIndex);

SimulationResult prepareSimulation(Circuit& circuit, Eigen::VectorXd& solutions, LogWindow* logWindow);

SimulationResult simulate(Circuit& circuit, SimulationMode mode, Eigen::VectorXd& solutions, LogWindow* logWindow);