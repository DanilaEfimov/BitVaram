#include <lexer.h>
#include <common.hpp>
#include <fstream>
#include <iostream>

#define NPOS -1

using namespace compiler;
using namespace varam;


void Lexer::trim(std::string& line, size_t& col) {
	const auto not_space = [](char c) { return !std::isspace(static_cast<unsigned char>(c)); };
	auto start = std::find_if(line.begin(), line.end(), not_space);
	auto end = std::find_if(line.rbegin(), line.rend(), not_space).base();

	if (start < end) {
		line = std::string(start, end);
	}
	else {
		line.clear();
		return;
	}

	auto first_non_space = std::find_if(line.begin(), line.end(), not_space);
	col += std::distance(line.begin(), first_non_space);
}

void Lexer::split(std::string& line, expression& seq, Position pos) {
	if (line.empty()) return;

	size_t col = 0;
	this->trim(line, col);

	while (!line.empty()) {
		size_t sep = line.find(' ');
		std::string word;

		if (sep == std::string::npos) {
			word = line;
			pos.column = col;
			seq.emplace_back(word, pos);
			break;
		}

		word = line.substr(0, sep);
		pos.column = col;
		seq.emplace_back(word, pos);

		col += sep + 1;
		line.erase(0, sep + 1);
		this->trim(line, col);
	}
}

void Lexer::process(varam::Config& config) {
	this->context.setPhase(Phase::Preprocessing);
	this->context.setStatus(Status::Started);

	const std::string filename = config.getInputFile();
	std::ifstream preprocessedFile(filename);
	if (!preprocessedFile.is_open()) {
		Position pos(filename, NPOS, NPOS);
		this->context.addError(varam::messages[error::failedtoOpenFile], pos);
		this->context.setStatus(Status::Aborted);
		return;
	}

	std::string line = "";
	expression tokens = {};
	int l = 1;
	while (std::getline(preprocessedFile, line)) {
		tokens = {};
		this->split(line, tokens, Position(filename, l, 0));
		this->expressions.push_back(tokens);
		l++;
	}

	if (!preprocessedFile.eof()) {
		Position pos(filename, l, line.size());
		this->context.addError(varam::messages[error::failedtoReadAll], pos);
	}

	preprocessedFile.close();
	for (auto& error : this->context.getErrors()) {
		context.getLogger().log(Level::Warning, error.message, error.position);
	}
}

const std::vector<expression>& Lexer::getExpressions() const {
	return this->expressions;
}

const Context& Lexer::getContext() const {
	return this->context;
}