/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	ComponentPropertiesWindow.h
//
//  ---------------------------------------------

#pragma once

#include "Component.h"

#include <qdialog.h>
#include <qboxlayout.h>
#include <qgroupbox.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qdialogbuttonbox.h>

class ComponentPropertiesWindow : public QDialog {
Q_OBJECT

	QFormLayout* _formLayout;

	Component& _component;

public:

	ComponentPropertiesWindow(Component& component);

public slots:

	// TODO rename to slotButtonAccept, or something like that?
	void buttonAccept();
	void buttonReject();
};
