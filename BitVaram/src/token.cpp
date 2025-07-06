#include <token.h>

using namespace compiler;

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