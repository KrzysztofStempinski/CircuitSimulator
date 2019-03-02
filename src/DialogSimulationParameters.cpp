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

	setWindowFlags(Qt::Dialog);

	setWindowTitle("Simulation parameters");

	_groupBoxDCOP = new QGroupBox();
	_groupBoxDCSweep = new QGroupBox();

	_tabs = new QTabWidget;
	//_tabs->show();
	_tabs->addTab(_groupBoxDCOP, "DC operating point");
	_tabs->addTab(_groupBoxDCSweep, "DC sweep");

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


	// TODO switch back to iterators
	for (int i = 0; i < _circuit.components.size(); ++i)
		if (!_circuit.components[i]->properties.empty())
			comboComponent->addItem(_circuit.components[i]->letterIdentifierBase() + QString::number(_circuit.components[i]->serialNumber));

	QObject::connect(comboComponent, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(componentComboChanged()));

		QLabel* label2 = new QLabel("parameter:");
		comboParameter = new QComboBox();

		sweepLayout->addWidget(label1);
		sweepLayout->addWidget(comboComponent);
		sweepLayout->addWidget(label2);
		sweepLayout->addWidget(comboParameter);

		QVBoxLayout *sweepMainLayout = new QVBoxLayout;
		sweepMainLayout->addLayout(sweepLayout);

		_groupBoxDCSweep->setLayout(sweepMainLayout);

	QHBoxLayout *layoutBottom = new QHBoxLayout;
	//QHBoxLayout *layoutTop = new QHBoxLayout;

	QPushButton* buttonRun = new QPushButton("Run simulation");
	buttonRun->setIcon(QIcon("data//icons//simulationRun.ico"));

	QObject::connect(buttonRun, SIGNAL(clicked()), this, SLOT(buttonRunClick()));

	layoutBottom->addWidget(buttonRun);
	layoutBottom->setAlignment(buttonRun, Qt::AlignRight);

	//mainLayout->addLayout(layoutTop);
	mainLayout->addWidget(_tabs);
	mainLayout->addLayout(layoutBottom);

	setLayout(mainLayout);

}

void DialogSimulationParameters::buttonRunClick() {

	//setResult(QDialog::Accepted);
	//close();
	QDialog::accept();

}

void DialogSimulationParameters::componentComboChanged() {
	
	comboParameter->clear();
	
	for (auto& it : _circuit.components[comboComponent->currentIndex()]->properties)
		comboParameter->addItem(it.second.displayName);

}