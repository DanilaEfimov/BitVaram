#include <lexer.h>
#include <common.hpp>
#include <fstream>

#define NPOS -1

using namespace compiler;
using namespace varam;

std::string Lexer::trimmed(const std::string& line) {

}

void Lexer::split(std::string& line, std::vector<key>& seq) {

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
}