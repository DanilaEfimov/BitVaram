#include <token.h>

using namespace compiler;

const std::set<LangFrame::key> LangFrame::keywords = {
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

const std::set<LangFrame::key> LangFrame::operators = {
			"(", ")",
			"=",
			"+",
			"-",
			"*",
			"/",
			"~"
};

const std::set<LangFrame::key> LangFrame::separators = {
			","
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