/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	DialogSimulationParameters.cpp
// 
//  ---------------------------------------------

#include "dialogSimulationParameters.h"

#include <qboxlayout.h>
#include <qstandarditemmodel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlabel.h>


DialogSimulationParameters::DialogSimulationParameters(Circuit& circuit)
	: _circuit(circuit) {

	setWindowTitle("");

	table = new QTableWidget;

	//table->setEditTriggers(QAbstractItemView::NoEditTriggers);

	table->setColumnCount(3);
	QStringList header = { "Element", "Value Type", "Value" };

	table->setHorizontalHeaderLabels(header);

	table->show();

	QVBoxLayout* mainLayout = new QVBoxLayout;

	QHBoxLayout *layoutBottom = new QHBoxLayout;
	QHBoxLayout *layoutTop = new QHBoxLayout;

	QPushButton* buttonRun = new QPushButton("Run simulation");
	buttonRun->setIcon(QIcon("data//icons//simulationRun.ico"));

	layoutBottom->addWidget(buttonRun);
	layoutBottom->setAlignment(buttonRun, Qt::AlignRight);

	mainLayout->addLayout(layoutTop);
	mainLayout->addWidget(table);
	mainLayout->addLayout(layoutBottom);

	setLayout(mainLayout);

}