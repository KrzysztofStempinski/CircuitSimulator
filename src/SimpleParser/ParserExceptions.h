#pragma once

#include <exception>
#include <string>

namespace SimpleParser {

	class ParserException : public std::exception {

	public:

		explicit ParserException(const std::string& message)
			: _message(message) {}

		virtual ~ParserException() throw () {}

		std::string description() {
			return "parser error: " + _message;
		}

	protected:

		std::string _message;

	};

};