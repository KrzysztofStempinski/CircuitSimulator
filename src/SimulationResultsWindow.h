/*

	This file is part of CircuitSimulator
	Copyright (C) 2018-20189 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	SimulationResultsWindow.h
//
//  ---------------------------------------------

#pragma once

#include <qdialog.h>
#include <qtablewidget.h>
#include <qcheckbox.h>

#include "Circuit.h"

#include "..//eigen//Dense"

class SimulationResultsWindow : public QDialog {
	Q_OBJECT

private:

	QTableWidget* table;
	QCheckBox* checkboxNodeVoltages;

	const Circuit& _circuit;

public:

	SimulationResultsWindow(Circuit& circuit);

public slots:

	void displayResults(bool displayNodeVoltages = false);
};
