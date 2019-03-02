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
#include <qtablewidget.h>
#include <qcheckbox.h>

#include "Circuit.h"

class DialogSimulationParameters : public QDialog {

		Q_OBJECT

		private:

			QTableWidget* table;

			const Circuit& _circuit;

	public:

		DialogSimulationParameters(Circuit& circuit);

	public slots:


};
