/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	ComponentPropertiesWindow.cpp
//
//  ---------------------------------------------

#include "ComponentPropertiesWindow.h"

#include "qvalidator.h"
#include <qpushbutton.h>

void ComponentPropertiesWindow::buttonAccept() {
	// save component properties
	for (int i = 0; i < _formLayout->count(); i++) {
		QWidget* widget = _formLayout->itemAt(i)->widget();
		if (widget != nullptr) {
			if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget)) {
				// TODO optimize, refactor etc.

				const auto it = _component.properties.find(widget->property("key").toString());

				it->second.value = lineEdit->text().toDouble(); // TODO this is an awful hack again
				//	_component->properties[it, lineEdit->text().toDouble());
			}
		}
	}

	parentWidget()->hide(); // TODO ugly hack

	close();
}

void ComponentPropertiesWindow::buttonReject() {
	parentWidget()->hide(); // TODO ugly hack

	close();
}

ComponentPropertiesWindow::ComponentPropertiesWindow(Component& component) : _component(component) {
	setWindowTitle("Component properties");

	QVBoxLayout* mainLayout = new QVBoxLayout;

	// buttons - OK and cancel
	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon("data/icons/ok.ico"));
	buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon("data/icons/cancel.ico"));

	connect(buttonBox, &QDialogButtonBox::accepted, this, &ComponentPropertiesWindow::buttonAccept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &ComponentPropertiesWindow::buttonReject);

	_formLayout = new QFormLayout;

	// TODO dedicated routines in a component for getting various forms of its name
	QGroupBox* formGroupBox = new QGroupBox(component.letterIdentifierBase() + QString::number(component.serialNumber) + " (" + component.displayNameBase() + ")");

	for (const auto& it : component.properties) {
		QString label = it.second.displayName + " [" + it.second.unit + "]";

		QLineEdit* lineEdit = new QLineEdit(QString::number(it.second.value, 'g', 10)); // TODO precision, notation settings itd.
		lineEdit->setProperty("key", it.first);

		QDoubleValidator* dv = new QDoubleValidator(lineEdit);
		lineEdit->setValidator(dv);

		_formLayout->addRow(new QLabel(label), lineEdit);
	}

	if (component.properties.empty())
		_formLayout->addRow(new QLabel("No properties available."));

	formGroupBox->setLayout(_formLayout);

	mainLayout->addWidget(formGroupBox);
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);

	formGroupBox->show();
}
