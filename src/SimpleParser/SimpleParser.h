#pragma once

#include <map>
#include <string>
#include <iostream>
#include <limits>

#include "ParserExceptions.h"

// behold the cleverest C++17 sexy template thingy 
template<typename First, typename ... T>
bool in(First&& first, T&& ... t) {
	return ((first == t) || ...);
}

namespace SimpleParser {

	struct VarTable {

		std::map<std::string, int> variables;

		int operator[](const std::string variableName) {

			auto it = variables.find(variableName);

			if (it == std::end(variables))
				throw ParserException("variable " + variableName + " not defined");
			else
				return (*it).second;

		}

		void addVariable(const std::string name, const int value) {

			variables[name] = value;

		}

		void clear() {
			variables.clear();
		}

	};

	enum class TokenType {

		None,
		Variable,
		Plus,
		Minus,
		EOI

	};

	class Token {

	public:

		TokenType _type;
		int _value;

		Token(const TokenType type, const int value = 0) : _type(type), _value(value) {}

		Token() : _type(TokenType::None), _value(0) { }

	};

	class Interpreter {

	private:

		std::string _expr;
		int _pos;
		Token _currentToken;
		char _currentChar;

	public:

		VarTable* varTable;

		void advance() {

			_pos++;

			if (_pos > _expr.length() - 1)
				_currentChar = '\0';
			else
				_currentChar = _expr[_pos];

		}

		void skipWhitespace() {

			while (_currentChar != '\0' && isspace(_currentChar))
				advance();

		}

		int getVariable() {

			std::string name = "";

			while (_currentChar != '\0' && (isalnum(_currentChar) || _currentChar == '.')) {
				name += _currentChar;
				advance();
			}

			return (*varTable)[name];

		}

		Token getNextToken() {

			while (_currentChar != '\0') {

				if (isspace(_currentChar)) {
					skipWhitespace();
					continue;
				}

				if (isalnum(_currentChar))
					return Token(TokenType::Variable, getVariable());

				if (_currentChar == '+') {
					advance();
					return Token(TokenType::Plus, 0);
				}

				if (_currentChar == '-') {
					advance();
					return Token(TokenType::Minus, 0);
				}

				throw ParserException("invalid token");

			}

			return Token(TokenType::EOI);

		}

		void eat(const TokenType TokenType) {

			if (_currentToken._type == TokenType)
				_currentToken = getNextToken();
			else
				throw ParserException("expected another token");

		}

		int term() {

			Token t = _currentToken;
			eat(TokenType::Variable);

			return t._value;

		}

		int evaluate(const std::string expression) {

			_expr = expression;
			_pos = 0,
			_currentToken = Token(TokenType::None); //  TODO cleanup etc.
			_currentChar = _expr[_pos];

			_currentToken = getNextToken();

			int result = term();

			while (in(_currentToken._type, TokenType::Plus, TokenType::Minus)) {

				Token token = _currentToken;

				if (token._type == TokenType::Plus) {
					eat(TokenType::Plus);
					result += term();
				}
				else if (token._type == TokenType::Minus) {
					eat(TokenType::Minus);
					result -= term();

				}

			}

			return result;

		}

	};

}