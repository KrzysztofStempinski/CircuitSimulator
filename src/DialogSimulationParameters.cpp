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

	_groupBoxDCOP = new QGroupBox();
	_groupBoxDCSweep = new QGroupBox();

	_tabs = new QTabWidget;
	//_tabs->show();
	_tabs->addTab(_groupBoxDCOP, "DC operating point");
	_tabs->addTab(_groupBoxDCSweep, "Linear DC sweep");

	QVBoxLayout* mainLayout = new QVBoxLayout;

	QLabel* label = new QLabel("TODO description + maybe some settings go here?");

	//	_groupBoxDCOP.
	QVBoxLayout *DCOPl = new QVBoxLayout;
	DCOPl->addWidget(label);

	_groupBoxDCOP->setLayout(DCOPl);

	// DC sweep
	QHBoxLayout* sweepLayout = new QHBoxLayout;

	QLabel* label1 = new QLabel("Component:");
	comboComponent = new QComboBox();

	for (int i = 0; i < _circuit.components.size(); ++i)
		if (!_circuit.components[i]->properties.empty())
			comboComponent->addItem(_circuit.components[i]->letterIdentifierBase() + QString::number(_circuit.components[i]->serialNumber), i);

	QObject::connect(comboComponent, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(componentComboChanged()));

	labelParameter = new QLabel("Parameter"); 
	labelParameter->hide();

	comboParameter = new QComboBox();
	comboParameter->hide();

	sweepLayout->addWidget(label1);
	sweepLayout->addWidget(comboComponent);
	sweepLayout->addWidget(labelParameter);
	sweepLayout->addWidget(comboParameter);

	sweepLayout->setAlignment(Qt::AlignLeft);

	QVBoxLayout* sweepMainLayout = new QVBoxLayout;
	sweepMainLayout->addLayout(sweepLayout);

	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow("Initial value:", new QLineEdit());
	formLayout->addRow("Final value:", new QLineEdit());
	formLayout->addRow("Delta:", new QLineEdit());

	sweepMainLayout->addLayout(formLayout);
	_groupBoxDCSweep->setLayout(sweepMainLayout);

	QHBoxLayout* layoutBottom = new QHBoxLayout;

	QPushButton* buttonRun = new QPushButton("Run simulation");
	buttonRun->setIcon(QIcon("data//icons//simulationRun.ico"));
		
	QObject::connect(buttonRun, SIGNAL(clicked()), this, SLOT(buttonRunClick()));

	layoutBottom->addWidget(buttonRun);
	layoutBottom->setAlignment(buttonRun, Qt::AlignRight);

	mainLayout->addWidget(_tabs);
	mainLayout->addLayout(layoutBottom);

	setLayout(mainLayout);

}

void DialogSimulationParameters::buttonRunClick() {

	QDialog::accept();

}

void DialogSimulationParameters::componentComboChanged() {
	
	if (comboParameter->isHidden()) {
		labelParameter->show();
		comboParameter->show();
	}

	comboParameter->clear();
	
	for (auto& it : _circuit.components[comboComponent->currentData().toInt()]->properties)
		comboParameter->addItem(it.second.displayName);

}