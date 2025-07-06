#include <context.h>
#include <iostream>
#include <boost/regex.hpp>

using namespace compiler;

constexpr std::string Logger::levelToStr(Level level) const {
	switch (level) {
		case Level::Info: return "INFO";
		case Level::Warning: return "WARNING";
		case Level::Error: return "ERROR";
	default:
		return "INFO";
	}
}

void Logger::log(Level level, const std::string& message, const Position& pos) {
	const std::string logMessage = "[" + this->levelToStr(level) + "] " + message + " at " + std::string(pos);
	
	boost::regex frame(R"(\[\w*\]\s.*\sat\s.*?\d+,\s*\d+)");
	if (boost::regex_search(logMessage, frame)) {
		std::cout << logMessage << std::endl;
	}
	else {
		std::cout << "[Failed to parse log message]" << std::endl;
	}
};

Phase Context::getPhase() const {
	return this->currentPhase;
};

void Context::setPhase(Phase phase) {
	this->currentPhase = phase;
}

Status Context::getStatus() const {
	return this->currentStatus;
}

void Context::setStatus(Status status) {
	this->currentStatus = status;
}

void Context::addError(const std::string& msg, const Position& pos) {
	this->errors.push_back(ErrorDetail(msg, pos));
}

const std::vector<ErrorDetail>& Context::getErrors() const {
	return this->errors;
}

Logger& Context::getLogger() {
	return this->logger;
}

void Context::setSourceCode(const std::string& code) {
	this->sourceCode = code;
}

const std::string& Context::getSourceCode() const {
	return this->sourceCode;
}