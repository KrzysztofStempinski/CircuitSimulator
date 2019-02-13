/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	LogWindow.h
// 
//  ---------------------------------------------

#pragma once

#include "Log.h"

#include "..//exprtk//exprtk.hpp"

#include <qdialog.h>
#include <qlistwidget.h>

#include <qcheckbox.h>

class LogWindow : public QDialog {

	Q_OBJECT

	private:

		QListWidget* list;
		QCheckBox* checkboxDebug;

	public:

		LogWindow(QWidget* parent = nullptr);

		void log(QString entry, LogEntryType type = LogEntryType::Info);
		void exprTkError(exprtk::parser<double>& parser);

	public slots:

		void slot_clearLog();

};