/*

	WidgetComponentProperties.cpp

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

#include "WidgetComponentProperties.h"

#include "Config.h"

#include <qvalidator.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qlineedit.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qlabel.h>

WidgetComponentProperties::WidgetComponentProperties(Component* component) : _component(component) {

	setWindowTitle("Component properties");

	const auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(Config::Icons::OK));
	connect(buttonBox, &QDialogButtonBox::accepted, this, &WidgetComponentProperties::slotButtonAccept);

	buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(Config::Icons::cancel));
	connect(buttonBox, &QDialogButtonBox::rejected, this, &WidgetComponentProperties::slotButtonReject);

	_formLayout = new QFormLayout;

	if (component->properties.empty())
		_formLayout->addRow(new QLabel("No properties available."));
	else {
		for (const auto& it : component->properties) {
			// TODO precision, notation settings itd.
			QLineEdit* lineEdit = new QLineEdit(QString::number(it.second.value));
			lineEdit->setProperty("key", it.first);

			const auto dv = new QDoubleValidator(lineEdit);
			lineEdit->setValidator(dv);

			_formLayout->addRow(it.second.displayName + " [" + it.second.unit + "]", lineEdit);
		}
	}

	// TODO dedicated routines for getting various forms of a component's name
	auto formGroupBox = new QGroupBox(component->letterIdentifierBase() + QString::number(component->serialNumber) + " (" + component->displayNameBase() + ")");
	formGroupBox->setLayout(_formLayout);

	auto mainLayout = new QVBoxLayout;
	mainLayout->addWidget(formGroupBox);
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);

}

void WidgetComponentProperties::slotButtonAccept() {

	for (int i = 0; i < _formLayout->count(); ++i) {
		QWidget* widget = _formLayout->itemAt(i)->widget();
		if (widget != nullptr)
			if (const auto lineEdit = qobject_cast<QLineEdit*>(widget))
				_component->properties[widget->property("key").toString()] = lineEdit->text().toDouble();
	}

	emit signalFinished();

	close();
}

void WidgetComponentProperties::slotButtonReject() {

	emit signalFinished();

	close();

}
