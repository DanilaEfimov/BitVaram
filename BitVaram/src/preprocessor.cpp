#include <preprocessor.h>
#include <common.hpp>
#include <fstream>
#include <cstring>

#define NPOS -1

using namespace compiler;
using namespace varam;

std::string Preprocessor::replacedPrefixName(const std::string& filename) {
	std::string newName = filename;
	
	size_t dotpoint = filename.find_last_of('.');
	if (dotpoint == std::string::npos) {
		newName += precompiledPrefix;
	}
	else {
		newName.replace(dotpoint, filename.size() - dotpoint, precompiledPrefix);
	}

	return newName;
}

void Preprocessor::solveSingleComment(std::string& line) {
	size_t from = line.find_first_of(singleCommentPattern);
	line.erase(from);
	this->state = State::pass;
}

void Preprocessor::solveMultiCommentOpening(std::string& line) {
	size_t from = line.find_first_of(multiCommentOpeningPattern);

	if (this->multistringCommentClosinginLine(line)) {
		this->state = State::pass;
	}
	else {
		this->state = State::waitingCommentClosing;
	}

	line.erase(from);
}

void Preprocessor::solveMultiCommentClosing(std::string& line) {
	size_t from = 0;
	size_t to = line.find_last_of(multiCommentClosingPattern) 
		+ std::strlen(multiCommentClosingPattern) - 1;

	line.erase(from, to);

	if (this->multistringCommentOpeninginLine(line)) {
		this->state = State::multistringCommentOpened;
		this->solveMultiCommentOpening(line);
	}
	else {
		this->state = State::pass;
	}
}

// State checking vvv
bool Preprocessor::isCorrectSwitch(State last, State next) {
	bool flag = true;
	if (last != State::waitingCommentClosing
		&& last != State::multistringCommentOpened
		&& next == State::multistringCommentClosed) {
		flag = false;	// Missing closing of a multilines comment
	}
	if ((last == State::multistringCommentOpened
		|| last == State::waitingCommentClosing)
		&& next == State::finished) {
		flag = false;	// Missing opening of a multilines comment
	}
	
	return flag;
}

bool Preprocessor::singleCommentinLine(const std::string& line) const {
	return line.find(singleCommentPattern) != std::string::npos;
}

bool Preprocessor::multistringCommentOpeninginLine(const std::string& line) const {
	return line.find(multiCommentOpeningPattern) != std::string::npos;
}

bool Preprocessor::multistringCommentClosinginLine(const std::string& line) const {
	return line.find(multiCommentClosingPattern) != std::string::npos;
}
// State checking ^^^

Preprocessor::State Preprocessor::checkState(const std::string& line) {
	if (this->singleCommentinLine(line)) {
		return State::singleCommentOccured;
	}
	if (this->multistringCommentOpeninginLine(line)) {
		return State::multistringCommentOpened;
	}
	if (this->multistringCommentClosinginLine(line)) {
		return State::multistringCommentClosed;
	}

	if (this->state == State::waitingCommentClosing
		|| this->state == State::multistringCommentOpened) {
		return State::waitingCommentClosing;
	}
	return this->state;
}

void Preprocessor::occureErrorStateSwitch(State last, State next, const std::string& line) {
	if (last == State::multistringCommentOpened
		&& next == State::finished) {
		this->pos.column = line.size();	// last symbol
		this->context.setSourceCode(line);
		this->context.addError(varam::messages[error::missedCommentClosing], this->pos);
	}
	if (last != State::multistringCommentOpened
		&& last != State::waitingCommentClosing
		&& next == State::multistringCommentClosed) {
		this->pos.column = line.find_first_of(multiCommentClosingPattern);
		this->context.setSourceCode(line);
		this->context.addError(varam::messages[error::missedCommentOpening], this->pos);
	}
}

void Preprocessor::occureFinishState(varam::Config& config) {
	this->context.setStatus(Status::Finished);

	const std::string filename = config.getInputFile();
	const std::string preprocessedFileName = this->replacedPrefixName(filename);
	config.set("input", preprocessedFileName);

	for (auto& error : this->context.getErrors()) {
		context.getLogger().log(Level::Warning, error.message, error.position);
	}
}

void Preprocessor::process(varam::Config& config) {
	this->context.setPhase(Phase::Preprocessing);
	this->context.setStatus(Status::Started);

	const std::string filename = config.getInputFile();
	const std::string preprocessedFileName = this->replacedPrefixName(filename);
	this->pos.filename = filename;

	std::ifstream sourceFile(filename);
	if (!sourceFile.is_open()) {
		Position pos(filename,NPOS, NPOS);
		this->context.addError(varam::messages[error::failedtoOpenFile], pos);
		this->context.setStatus(Status::Aborted);
		return;
	}

	std::ofstream preprocessedFile(preprocessedFileName);
	if (!preprocessedFile.is_open()) {
		Position pos(preprocessedFileName, NPOS, NPOS);
		this->context.addError(varam::messages[error::failedtoOpenFile], pos);
		this->context.setStatus(Status::Aborted);
		return;
	}

	this->pos.line = 1;
	std::string line = "";
	while (std::getline(sourceFile, line)) {
		this->pos.column = 1;

		State currentState = checkState(line);
		if (this->isCorrectSwitch(this->state, currentState)) {
			this->state = currentState;
			switch (this->state) {
				case State::singleCommentOccured: this->solveSingleComment(line); break;
				case State::multistringCommentOpened: this->solveMultiCommentOpening(line); break;
				case State::waitingCommentClosing: this->pos.line++; continue;
				case State::multistringCommentClosed: this->solveMultiCommentClosing(line); break;
				case State::finished: goto finish; break;
			default:
				break;
			}
		}
		else {
			this->occureErrorStateSwitch(this->state, currentState, line);
			break;
		}

		if (!line.empty()) {
			preprocessedFile << line << std::endl;
		}
			this->pos.line++;  
	}

	finish:

	sourceFile.close();
	preprocessedFile.close();

	this->occureFinishState(config);
}

const Context& Preprocessor::getContext() const {
	return this->context;
}
