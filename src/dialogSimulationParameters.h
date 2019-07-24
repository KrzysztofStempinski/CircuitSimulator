/*

	This file is part of CircuitSimulator
	Copyright (C) 2018-20189 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	DialogSimulationPamaters.h
//
//  ---------------------------------------------

#pragma once

#include <qdialog.h>
#include <qtabwidget.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>

#include "Circuit.h"
#include "SimulationParameters.h"

class DialogSimulationParameters : public QDialog {
	Q_OBJECT
private:

	const Circuit& _circuit;

	QComboBox* comboComponent;
	QComboBox* comboParameter;

	QLabel* labelComponent;
	QLabel* labelParameter;

	QTabWidget* tabs;

	QLineEdit* editInitialValue;
	QLineEdit* editFinalValue;
	QLineEdit* editDelta;

public:

	DialogSimulationParameters(Circuit& circuit);

	SimulationParameters parameters;

public slots:

	void buttonRunClick();

	void componentComboChanged();
};
