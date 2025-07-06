#ifndef TOKEN_H
#define TOKEN_H

#include <common.hpp>
#include <context.h>
#include <vector>
#include <string>
#include <set>

namespace compiler {

	enum class TokenType {
		KEYWORD,
		IDENTIFIER,
		OPERATOR,	// =, +, -, *, /
		NUMBER,
		SEPARATOR
	};

	struct LangFrame {
		using key = std::string;
	private:
		const std::set<key> keywords = {
			"coredef",		// coredef [function definition]
			"coreundef",	// coreundef [function identifier]
			"curdef",		// curdef [function definition] / [variable definition] [=] [number]
			"curundef",		// curundef [functtion identifier] / [variable identifier] [=] [number]
			"defined",		// defined [identifier]
			"if",			// if [variable identifier]
			"for",			// for [variable identifier]
			"store",		// store [identifier], [value]
			"show",			// show {prints all from registry}
			"and",			// [identifier] and [identifier]
			"or",			// [identifier] or [identifier]
			"xor",			// [identifier] xor [identifier]
			"sup",			// sup [(] {separated identifiers} [)]
			"inf",			// inf [(] {separated identifiers} [)]
			"return",		// return [identifier]
			"exit"			// exit [number]
		};
		
		const std::set<key> operators = {
			"(", ")",
			"=",
			"+",
			"-",
			"*",
			"/",
			"~"
		};

		const std::set<key> separators = {
			","
		};

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