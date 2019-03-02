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

#include "Circuit.h"

class DialogSimulationParameters : public QDialog {

		Q_OBJECT

		private:

			QTabWidget* _tabs;
			QGroupBox* _groupBoxDCOP;
			QGroupBox* _groupBoxDCSweep;
			const Circuit& _circuit;


			QComboBox* comboComponent;
			QComboBox* comboParameter;
	public:

		DialogSimulationParameters(Circuit& circuit);

	public slots:

		void buttonRunClick();

		void componentComboChanged();


};
