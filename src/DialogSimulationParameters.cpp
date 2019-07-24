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
#include <qformlayout.h>
#include <Qlineedit.h>

DialogSimulationParameters::DialogSimulationParameters(Circuit& circuit)
	: _circuit(circuit) {
	setWindowFlags(Qt::Dialog);
	setWindowTitle("Simulation parameters");

	// create interface
	tabs = new QTabWidget;

	QWidget* tabDCOP = new QWidget(tabs);
	QWidget* tabDCSweep = new QWidget(tabs);

	tabs->addTab(tabDCOP, "DC operating point");
	tabs->addTab(tabDCSweep, "Linear DC sweep");

	// create the DCOP tab (currently, contains only a simple description)
	QVBoxLayout* layoutDCOPdescription = new QVBoxLayout;
	layoutDCOPdescription->addWidget(new QLabel("Computes the DC operating points of the circuit, treating capacitors as open circuits and inductors as short circuits."));

	tabDCOP->setLayout(layoutDCOPdescription);

	// create the DC sweep tab
	// we start with HBox for component name and parameter
	QHBoxLayout* layoutComponentParameter = new QHBoxLayout;

	labelComponent = new QLabel("Component:");
	comboComponent = new QComboBox();

	for (int i = 0; i < _circuit.components.size(); ++i)
		if (!_circuit.components[i]->properties.empty())
			comboComponent->addItem(_circuit.components[i]->letterIdentifierBase() + QString::number(_circuit.components[i]->serialNumber), i);

	QObject::connect(comboComponent, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(componentComboChanged()));

	labelParameter = new QLabel("Parameter");
	labelParameter->hide();

	comboParameter = new QComboBox();
	comboParameter->hide();

	layoutComponentParameter->addWidget(labelComponent);
	layoutComponentParameter->addWidget(comboComponent);
	layoutComponentParameter->addWidget(labelParameter);
	layoutComponentParameter->addWidget(comboParameter);

	layoutComponentParameter->setAlignment(Qt::AlignLeft);

	// form layout for sweep numeric parameters
	QFormLayout* layoutSweepParameters = new QFormLayout;

	editInitialValue = new QLineEdit();
	editFinalValue = new QLineEdit();
	editDelta = new QLineEdit();

	layoutSweepParameters->addRow("Initial value:", editInitialValue);
	layoutSweepParameters->addRow("Final value:", editFinalValue);
	layoutSweepParameters->addRow("Delta:", editDelta);

	// main layout for DC sweep tab
	QVBoxLayout* sweepMainLayout = new QVBoxLayout;
	sweepMainLayout->addLayout(layoutComponentParameter);
	sweepMainLayout->addLayout(layoutSweepParameters);

	tabDCSweep->setLayout(sweepMainLayout);

	// create "Simulation Run" button and corresponding layout
	QPushButton* buttonRun = new QPushButton("Run simulation");
	buttonRun->setIcon(QIcon("data//icons//simulationRun.ico"));
	QObject::connect(buttonRun, SIGNAL(clicked()), this, SLOT(buttonRunClick()));

	QHBoxLayout* layoutBottom = new QHBoxLayout;
	layoutBottom->addWidget(buttonRun);
	layoutBottom->setAlignment(buttonRun, Qt::AlignRight);

	// bring everything together
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(tabs);
	mainLayout->addLayout(layoutBottom);

	setLayout(mainLayout);
}

void DialogSimulationParameters::buttonRunClick() {
	switch (tabs->currentIndex()) {
		// standard DCOP
	case 0:

		parameters.mode = SimulationMode::DCOP;

		QDialog::accept();
		//return;

		break;

		// DC sweep
	case 1:

		parameters.mode = SimulationMode::DCSweep;
		// TODO consistent variable naming
		// TODO error handling
		parameters.start = editInitialValue->text().toDouble();
		parameters.end = editFinalValue->text().toDouble();
		parameters.delta = editDelta->text().toDouble();

		parameters.componentToSweep = _circuit.components[comboComponent->currentData().toInt()];
		parameters.propertyToSweep = comboParameter->currentData().toString();
		QDialog::accept();

		break;
	}
}

void DialogSimulationParameters::componentComboChanged() {
	if (comboParameter->isHidden()) {
		labelParameter->show();
		comboParameter->show();
	}

	comboParameter->clear();

	for (auto& it : _circuit.components[comboComponent->currentData().toInt()]->properties)
		comboParameter->addItem(it.second.displayName, it.first);
}