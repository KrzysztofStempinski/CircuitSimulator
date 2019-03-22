#pragma once

#include <exception>
#include <qmessagebox.h>
#include <qapplication.h>

#include <qstring.h>

class SimulationException : public std::exception {

	private:

		QString message;

	public:

		void show(QWidget* parent) {
		
			QMessageBox msgBox(parent);

			msgBox.setText("Simulation failed: " + message);
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Icon::Critical);

			QApplication::beep();
			msgBox.exec();
		
		}

		QString what() {
			return message;
		}

		SimulationException(const QString message)
			: message(message) {}



};