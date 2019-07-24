#pragma once

#include <exception>
#include <qmessagebox.h>
#include <qapplication.h>

#include <qstring.h>

class SimulationException : public std::exception {

	private:

		const QString message;

	public:

		QString what() {
			return message;
		}

		SimulationException(const QString message)
			: message(message) {}



};