#include "Parser.h"
#include <fstream>
#include <memory>
#include <iostream>
#include <common.hpp>

using namespace compiler;


// here would looks cool some statemate fabric
Parser::StatemateType Parser::getType(const expression& expr) const {
    try {
        if (statemates::identifier_decl::isValidSyntax(expr))
            return StatemateType::Identifier_decl;
        if (statemates::undeclaration::isValidSyntax(expr))
            return StatemateType::Undeclaration;
        if (statemates::keyword::isValidSyntax(expr))
            return StatemateType::Keyword;
    }
    catch (std::exception& e) {
        this->context.addError(e.what(), expr[0].getPosition());
        this->context.setSourceCode(expr[0].getValue());
        this->context.setStatus(Status::ErrorOccurred);
        throw std::runtime_error("invalid syntax");
    }
}

int Parser::getBound(const std::vector<expression>& source, StatemateType type, int start) const {
    try {
        std::vector<expression> code = 
            std::vector<expression>(source.begin() + start, source.end());
        int end = start;
        switch (type) {
        case StatemateType::Identifier_decl: 
            end += statemates::identifier_decl::getStatemateBound(code);
            break;
        case StatemateType::Undeclaration:
            end += statemates::undeclaration::getStatemateBound(code);
            break;
        case StatemateType::Keyword:
            end += statemates::keyword::getStatemateBound(code);
            break;
        case StatemateType::Block:
            end += statemates::block::getStatemateBound(code);
            break;
        case StatemateType::Function_decl:
        case StatemateType::Assignment:
        case StatemateType::System_call:
        default:
        {
            this->context.addError(varam::messages[invalidSyntax], source[0][0].getPosition());
            this->context.setStatus(Status::ErrorOccurred);
            return 0;
        }
        }

        return end;
    }
    catch (std::exception& e) {
        this->context.addError(e.what(), source[0][0].getPosition());
        this->context.setSourceCode(source[0][0].getValue());
        this->context.setStatus(Status::ErrorOccurred);
        return -1;
    }
}

std::unique_ptr<statemates::statemate> Parser::makeStatemate(
    statemates::block* parent,
    StatemateType type,
    std::vector<expression>& code
) const {

    try {
        switch (type) {
        case StatemateType::Identifier_decl:
            return std::unique_ptr<statemates::statemate>
                (new statemates::identifier_decl(parent, code));
        case StatemateType::Undeclaration:
            return std::unique_ptr<statemates::statemate>
                (new statemates::undeclaration(parent, code));
        case StatemateType::Keyword:
            return std::unique_ptr<statemates::statemate>
                (new statemates::keyword(parent, code));
        case StatemateType::Block:
            return std::unique_ptr<statemates::statemate>
                (new statemates::block(parent));
        case StatemateType::Function_decl:
            return std::unique_ptr<statemates::statemate>
                (new statemates::function_decl(parent));
        case StatemateType::Assignment:
            return std::unique_ptr<statemates::statemate>
                (new statemates::assignment(parent));
        case StatemateType::System_call:
            return std::unique_ptr<statemates::statemate>
                (new statemates::system_call(parent));
        default:
        {
            this->context.addError(varam::messages[invalidSyntax], code[0][0].getPosition());
            this->context.setSourceCode(code[0][0].getValue());
            this->context.setStatus(Status::ErrorOccurred);
            return nullptr;
        }
        }
    }
    catch (std::exception& e) {
        this->context.addError(e.what(), code[0][0].getPosition());
        this->context.setSourceCode(code[0][0].getValue());
        this->context.setStatus(Status::ErrorOccurred);
        return nullptr;
    }
}

statemates::block* Parser::buildAST(
    std::vector<expression>& code,
    statemates::block* root
) const {

    if (static_cast<int>(code.size()) == 0) {
        return nullptr;
    }

    for (int i = 0; i < static_cast<int>(code.size());) {
        StatemateType type = this->getType(code[i]);
        int start = i;
        int end = this->getBound(code, type, i) + 1;

        if (start >= end) {
            throw std::runtime_error(varam::messages[undefinedError]);
            break;
        }
        std::vector<expression> source = 
            std::vector<expression>(code.begin() + start, code.begin() + end);

        std::unique_ptr<statemates::statemate> child = this->makeStatemate(root, type, source);
        root->instructions.push_back(std::move(child));

        i = end;
    }

    return root;
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
    std::cout << boost::json::serialize(this->ASTroot->toJson()) << std::endl;
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

void Parser::process(varam::Config& config, std::vector<expression>& code) {

    this->context.setPhase(Phase::Parsing);
    this->context.setStatus(Status::Started);

    config.set("input", replacedPrefixName(config.getInputFile()));

    try {
        this->ASTroot = this->buildAST(code, this->ASTroot);
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
    
}

const Context& Parser::getContext() const {
    return this->context;
}
