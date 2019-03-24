/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	SimulationResultsWindow.cpp
// 
//  ---------------------------------------------

#include "SimulationResultsWindow.h"

#include <qboxlayout.h>
#include <qstandarditemmodel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlabel.h>

// goddamnit fold expressions are sexy
// unfold me daddy uwu
template<typename ...Args>
void addRow(QTableWidget* table, Args... args) {

	table->insertRow(table->rowCount());

	int i = 0;
	(table->setItem(table->rowCount() - 1, i++, new QTableWidgetItem(args)), ...);

};

void SimulationResultsWindow::displayResults(bool displayNodeVoltages) {

	table->setRowCount(0);

	for (auto& it : _circuit.components) {
		if (it->hasSimulationResult()) {

			const SimulationResult result = it->getSimulationResult();

			addRow(table, result.name, result.unit, QString::number(result.value));

		}
	}

	// TODO fix out of bounds bug
	// it is because solutions are passed by reference, and solutions in mainWindow goes out of scope,
	// so here _solutions is empty
	if (displayNodeVoltages) 
		for (int i = 0; i < _circuit.voltageCount; ++i) 
			addRow(table, "N" + QString::number(i + 1), "Node Voltage [V]", QString::number(_solutions(i)));


	table->sortByColumn(0, Qt::AscendingOrder);

}

SimulationResultsWindow::SimulationResultsWindow(Circuit& circuit, Eigen::VectorXd solutions)
	: _circuit(circuit),
	_solutions(solutions)
{

	setWindowTitle("Simulation results | Circuit Simulator");

	table = new QTableWidget;
	
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setColumnCount(3);
	table->setHorizontalHeaderLabels({ "Element", "Value Type", "Value" });

	table->show();
	displayResults();

	QVBoxLayout* mainLayout = new QVBoxLayout;
	QHBoxLayout* layoutBottom = new QHBoxLayout;
	QHBoxLayout* layoutTop = new QHBoxLayout;

	QSpinBox* precisionSpin = new QSpinBox;
	precisionSpin->setMinimum(1);
	precisionSpin->setMaximum(16);
	precisionSpin->setValue(6);

	QLabel* precisionLabel = new QLabel("Precision:");

	checkboxNodeVoltages = new QCheckBox("Show node voltages");
	QObject::connect(checkboxNodeVoltages, SIGNAL(clicked(bool)), this, SLOT(displayResults(bool)));

	layoutTop->addWidget(checkboxNodeVoltages);
	layoutTop->addWidget(precisionLabel);
	layoutTop->addWidget(precisionSpin);
	layoutTop->setAlignment(precisionLabel, Qt::AlignRight);

	QPushButton* buttonSave = new QPushButton("Save to file...");
	buttonSave->setIcon(QIcon("data//icons//fileSave.ico"));

	layoutBottom->addWidget(buttonSave);
	layoutBottom->setAlignment(buttonSave, Qt::AlignRight);

	mainLayout->addLayout(layoutTop);
	mainLayout->addWidget(table);
	mainLayout->addLayout(layoutBottom);

	setLayout(mainLayout);

}