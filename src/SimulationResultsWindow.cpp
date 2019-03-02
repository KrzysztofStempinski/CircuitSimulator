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

void SimulationResultsWindow::displayResults(bool displayNodeVoltages) {

	table->setRowCount(0);

	for (auto& it : _circuit.components) {
		if (it->hasSimulationResult()) {

			const std::tuple<QString, QString, double> result = it->getSimulationResult();

			table->insertRow(table->rowCount());
			table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(std::get<0>(result)));
			table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(std::get<1>(result)));
			table->setItem(table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(std::get<2>(result), 'g', 15)));

		}
	}

	if (displayNodeVoltages) {
		for (int i = 0; i < _voltageCount; ++i) {

			table->insertRow(table->rowCount());
			table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("N" + QString::number(i + 1)));
			table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem("Node Voltage [V]"));
			table->setItem(table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(_circuit.nodes[i]->voltageValue, 'g', 15)));

		}
	}

	table->sortByColumn(0, Qt::AscendingOrder);

}

SimulationResultsWindow::SimulationResultsWindow(Circuit& circuit, Eigen::VectorXd solutions, int voltageCount)
	: _circuit(circuit),
	_solutions(solutions),
	_voltageCount(voltageCount){

	setWindowTitle("Simulation results | Circuit Simulator");

	table = new QTableWidget;
	
	//table->setEditTriggers(QAbstractItemView::NoEditTriggers);

	table->setColumnCount(3);
	QStringList header = { "Element", "Value Type", "Value" };

	table->setHorizontalHeaderLabels(header);	

	table->show();
	displayResults();

	QVBoxLayout* mainLayout = new QVBoxLayout;

	QHBoxLayout *layoutBottom = new QHBoxLayout;
	QHBoxLayout *layoutTop = new QHBoxLayout;

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