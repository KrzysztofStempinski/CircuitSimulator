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

	if (type == LogEntryType::Debug && !checkboxDebug->isChecked())
		return;

	QString text;
	QIcon icon;

	switch (type) {

		case LogEntryType::Error:
			icon = QIcon("data\\icons\\error.ico");
			text = entry; //"ERROR: ";
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
	//list->setWordWrap(true);

	QVBoxLayout *mainLayout = new QVBoxLayout;

	mainLayout->addWidget(list);

	checkboxDebug = new QCheckBox("Show debug messages");

	if (DEBUG)
		checkboxDebug->setChecked(true);

//	mainLayout->addWidget(checkboxDebug);

	QHBoxLayout *layout2 = new QHBoxLayout;

	layout2->addWidget(checkboxDebug);

	QPushButton* buttonClear = new QPushButton("Clear log");
	buttonClear->setIcon(QIcon("data//icons//clear.ico"));
	//buttonClear->setToolTip("Clear log");

	QObject::connect(buttonClear, SIGNAL(clicked()), this, SLOT(slot_clearLog()));

	layout2->addWidget(buttonClear);
	layout2->setAlignment(buttonClear, Qt::AlignRight);

	mainLayout->addLayout(layout2);

	setLayout(mainLayout);

	list->setSelectionMode(QAbstractItemView::NoSelection);

	list->show();

}

void LogWindow::slot_clearLog() {
	list->clear();

	log("CircuitSimulator v." + VersionInfo::getVersionString() + "\ncopyright (C) 2018 by Krzysztof Stempinski", LogEntryType::Info);
	log("This is free software, but comes with ABSOLUTELY NO WARRANTY. You are welcome to redistribute it, subject to certain conditions. See license.md for more details.");

}

// TODO fix acess violation problem
void LogWindow::exprTkError(exprtk::parser<double>& parser) {

	for (std::size_t i = 0; i < parser.error_count(); ++i){

		QString msg = "exprTk ";

		exprtk::parser_error::type error = parser.get_error(i);

		msg += QString::fromStdString(exprtk::parser_error::to_str(error.mode)).toLower() + " at position " + QString::number(error.token.position);
		msg += ": " + QString::fromStdString(error.diagnostic);

		log(msg, LogEntryType::Error);

	}

}
