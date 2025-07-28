#include <token.h>
#include <common.hpp>

using namespace compiler;

const std::set<LangFrame::key> LangFrame::keywords = {
		COREDEF,		// coredef [function definition]
		COREUNDEF,		// coreundef [function identifier]
		CURDEF,			// curdef [function definition] / [variable definition] [=] [number]
		CURUNDEF,		// curundef [function identifier] / [variable identifier] [=] [number]
		DEFINED,		// defined [identifier]
		IF,				// if [variable identifier]
		FOR,			// for [variable identifier]
		STORE,			// store [identifier], [value]
		SHOW,			// show {prints all from registry}
		AND,			// [identifier] and [identifier]
		OR,				// [identifier] or [identifier]
		XOR,			// [identifier] xor [identifier]
		SUP,			// sup [(] {separated identifiers} [)]
		READ,			// [identifier] [=] read
		INF,			// inf [(] {separated identifiers} [)]
		RETURN,			// return [identifier]
		EXIT			// exit [number]
};

const std::set<LangFrame::key> LangFrame::operators = {
		EQUALS,
		PLUS,
		MINUS,
		MUL,
		DIV,
		NOT,
		LESS,
		FUNC_OPEN_BRACKET, FUNC_CLOSE_BRACKET,
		BLOCK_OPEN_BRACKET, BLOCK_CLOSE_BRACKET,
		MULTY_ARG_FUNC_OPEN_BRACKET, MULTY_ARG_FUNC_CLOSE_BRACKET
};

const std::set<LangFrame::key> LangFrame::separators = {
		COMMA
};

const std::set<LangFrame::key> LangFrame::systemCalls = {
	SHOW,
	EXIT
};

const std::map<LangFrame::key, LangFrame::key> LangFrame::borders = {
		{FUNC_OPEN_BRACKET, FUNC_CLOSE_BRACKET},
		{BLOCK_OPEN_BRACKET, BLOCK_CLOSE_BRACKET},
		{MULTY_ARG_FUNC_OPEN_BRACKET, MULTY_ARG_FUNC_CLOSE_BRACKET}
};


Token::Token(const std::string& word, const Position& pos) {
	LangFrame lang;
	this->type = lang.checkLangContext(word);
	this->value = word;
	this->pos = pos;
}

Token::Token(const std::string& word) {
	LangFrame lang;
	this->type = lang.checkLangContext(word);
	this->value = word;
	this->pos = Position();
}

TokenType Token::getType() const {
	return this->type;
}

std::string Token::getValue() const {
	return this->value;
}

Position Token::getPosition() const {
	return this->pos;
}

TokenType Token::checkWord(const std::string& word, const LangFrame& lang) {
	return lang.checkLangContext(word);
}

bool LangFrame::isBorders(const std::string& open,
	const std::string& close) {
	if (LangFrame::borders.count(open)) {
		return LangFrame::borders.at(open) == close;
	}
	return false;
}

bool LangFrame::isSysCall(const std::string& value) {
	return LangFrame::systemCalls.contains(value);
};

bool LangFrame::isOpening(const std::string& opening) {
	return LangFrame::borders.contains(opening);
}

bool LangFrame::isClosing(const std::string& closing) {
	for (auto& p : LangFrame::borders) {
		if (p.second == closing) {
			return true;
		}
	}
	return false;
}