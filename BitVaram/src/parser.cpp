#include "Parser.h"
#include <fstream>
#include <memory>
#include <iostream>
#include <common.hpp>

using namespace compiler;


// here would looks cool some statemate fabric
statemates::StatemateType Parser::getTypeImpl(const expression& expr) const {
    try {
        return statemates::getType(expr);
    }
    catch (std::exception& e) {
        this->context.addError(e.what(), expr[0].getPosition());
        this->context.setSourceCode(expr[0].getValue());
        this->context.setStatus(Status::ErrorOccurred);
        throw std::runtime_error("invalid syntax");
    }
}

int Parser::getBoundImpl(const std::vector<expression>& source, statemates::StatemateType type, int start) const {
    try {
        return statemates::getBound(source, type, start);
    }
    catch (std::exception& e) {
        this->context.addError(e.what(), source[0][0].getPosition());
        this->context.setSourceCode(source[0][0].getValue());
        this->context.setStatus(Status::ErrorOccurred);
        return -1;
    }
}

std::unique_ptr<statemates::statemate> Parser::makeStatemateImpl(
    statemates::block* parent,
    statemates::StatemateType type,
    const std::vector<expression>& code
) const {
    try {
        return statemates::makeStatemate(parent, type, code);
    }
    catch (std::exception& e) {
        this->context.addError(e.what(), code[0][0].getPosition());
        this->context.setSourceCode(code[0][0].getValue());
        this->context.setStatus(Status::ErrorOccurred);
        return nullptr;
    }
}

statemates::block* Parser::buildASTImpl(
    const std::vector<expression>& code,
    statemates::block* root
) const {

    if (static_cast<int>(code.size()) == 0) {
        return nullptr;
    }

    return statemates::buildAST(code, root);
}

void Parser::occureSuccessState(varam::Config& config) const {

    std::string target = config.getInputFile();
    ofstream output(target, std::ios::ate);
    if (!output.is_open()) {
        this->context.addError(varam::messages[failedtoOpenFile], Position());
        this->context.setStatus(Status::Aborted);
        throw std::runtime_error("failed to save output");
    }

    boost::json::object AST = this->ASTroot->toJson();
    output << boost::json::serialize(AST);
    output.close();
}

Parser::Parser()
    : context(), ASTroot(new statemates::block())
{
}

std::string Parser::replacedPrefixName(const std::string& filename) {
    std::string newName = filename;

    size_t dotpoint = filename.find_last_of('.');
    if (dotpoint == std::string::npos) {
        newName += parsedPrefix;
    }
    else {
        newName.replace(dotpoint, filename.size() - dotpoint, parsedPrefix);
    }

    return newName;
}

void Parser::process(varam::Config& config, const std::vector<expression>& code) {

    this->context.setPhase(Phase::Parsing);
    this->context.setStatus(Status::Started);

    config.set("input", replacedPrefixName(config.getInputFile()));

    try {
        this->ASTroot = this->buildASTImpl(code, this->ASTroot);
        this->occureSuccessState(config);
    }
    catch (std::runtime_error& e) {
        this->context.addError(e.what(), Position(config.getInputFile(), -1, -1));
        this->context.setStatus(Status::Aborted);
    }
    catch (...) {
        this->context.addError(varam::messages[undefinedError], Position());
        this->context.setStatus(Status::Aborted);
    }

    for (const auto& error : this->context.getErrors()) {
        std::cerr << error.message << " at " 
            << std::string(error.position) << std::endl;
    }
    
}

const Context& Parser::getContext() const {
    return this->context;
}
