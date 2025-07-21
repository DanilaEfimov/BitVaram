#ifndef TOKEN_H
#define TOKEN_H

#include <common.hpp>
#include <context.h>
#include <vector>
#include <string>
#include <set>
#include <map>

namespace compiler {

	enum class TokenType {
		KEYWORD,
		IDENTIFIER,
		OPERATOR,	// =, +, -, *, /
		NUMBER,		// no more literal types
		SEPARATOR
	};

	struct LangFrame {
		using key = std::string;
	private:
		static const std::set<key> keywords;
		
		static const std::set<key> operators;

		static const std::set<key> separators;

		static const std::set<key> systemCalls;

		static const std::map<key, key> borders;

		bool isNumber(const std::string& word) const {
			try {
				size_t ind;
				std::stoi(word, &ind);
				return ind == word.size();
			}
			catch (...) {
				return false;
			}
		};

	public:
		TokenType checkLangContext(const std::string& word) const {
			if (this->keywords.count(word)) {
				return TokenType::KEYWORD;
			}
			if (this->operators.count(word)) {
				return TokenType::OPERATOR;
			}
			if (this->separators.count(word)) {
				return TokenType::SEPARATOR;
			}
			if (this->isNumber(word)) {
				return TokenType::NUMBER;
			}
			return TokenType::IDENTIFIER;
		}

		static bool isBorders(const std::string& open,
			const std::string& close);
		static bool isSysCall(const std::string& value);

		static bool isOpening(const std::string& opening);
		static bool isClosing(const std::string& closing);
	};

	class Token {
	public:
		Token(const std::string& word, const Position& pos);
		explicit Token(const std::string& word);
		explicit Token(TokenType type) : type(type), pos(), value() {};
		~Token() = default;

		TokenType getType() const;
		std::string getValue() const;
		Position getPosition() const;

		static TokenType checkWord(const std::string& word,
								const LangFrame& lang);

	private:
		Position pos;
		std::string value;
		TokenType type;
	};

}	// namespace compiler

#endif	// TOKEN_H