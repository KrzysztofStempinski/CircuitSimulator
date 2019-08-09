/*
 
	WidgetComponentProperties.h

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

#pragma once

#include "Component.h"

#include <qwidget.h>
#include <qformlayout.h>

class WidgetComponentProperties : QWidget {
Q_OBJECT

	QFormLayout* _formLayout;

	Component& _component;

public:

	WidgetComponentProperties(Component& component);

public slots:

	void slotButtonAccept();
	void slotButtonReject();

signals:
	void signalFinished();

};
