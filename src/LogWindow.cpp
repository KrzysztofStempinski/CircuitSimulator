/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	LogWindow.cpp
// 
//  ---------------------------------------------

#include "Log.h"
#include "LogWindow.h"

#include <qboxlayout.h>
#include <qpushbutton.h>

#include "Version.h"

void LogWindow::log(QString entry, LogEntryType type) {

	// discard debug messages
	if (type == LogEntryType::Debug && !checkboxDebug->isChecked())
		return;

	QString text;
	QIcon icon;

	switch (type) {

		case LogEntryType::Error:
			icon = QIcon("data\\icons\\error.ico");
			text = entry; 
		break;

		case LogEntryType::Info:
			icon = QIcon("data\\icons\\info.ico");			
			text = entry; // or Info, log or sth ???
		break;

		case LogEntryType::Warning:
			icon = QIcon("data\\icons\\warning.ico");
			text = entry; // or Warning...?
		break;

		case LogEntryType::Debug:
			icon = QIcon("data\\icons\\debug.ico");
			text = entry; // as above?
		break;

	}

	QListWidgetItem *newItem = new QListWidgetItem;
	newItem->setText(text);
	newItem->setIcon(icon);

	list->addItem(newItem);
	
	list->scrollToBottom();

}

LogWindow::LogWindow(QWidget* parent){

	setParent(parent);
	setWindowTitle("Debug | Circuit Simulator");

	list = new QListWidget(this);
	list->setSelectionMode(QAbstractItemView::NoSelection);
	// TODO maybe we need list.show in here? check.

	QVBoxLayout* mainLayout = new QVBoxLayout;

	mainLayout->addWidget(list);

	if (DEBUG) {
		checkboxDebug = new QCheckBox("Show debug messages");
		checkboxDebug->setChecked(true);
	}

	QHBoxLayout* layoutBottom = new QHBoxLayout;

	layoutBottom->addWidget(checkboxDebug);

	QPushButton* buttonClear = new QPushButton("Clear log");
	buttonClear->setIcon(QIcon("data//icons//clear.ico"));
	buttonClear->setToolTip("Clear log");
	QObject::connect(buttonClear, SIGNAL(clicked()), this, SLOT(slot_clearLog()));

	layoutBottom->addWidget(buttonClear);
	layoutBottom->setAlignment(buttonClear, Qt::AlignRight);

	mainLayout->addLayout(layoutBottom);

	setLayout(mainLayout);

}

void LogWindow::slot_clearLog() {

	list->clear();

	log("CircuitSimulator v." + VersionInfo::getVersionString() + "\ncopyright (C) 2018 by Krzysztof Stempinski", LogEntryType::Info);
	log("This is free software, but comes with ABSOLUTELY NO WARRANTY. You are welcome to redistribute it, subject to certain conditions. See license.md for more details.");

}

