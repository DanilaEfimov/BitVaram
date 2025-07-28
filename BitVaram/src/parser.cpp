#include "Parser.h"
#include <fstream>
#include <memory>
#include <iostream>
#include <common.hpp>

using namespace compiler;


// here would looks cool some statemate fabric
Parser::StatemateType Parser::getType(std::vector<expression>& source, int start) const {
    expression header = source[0];
    return StatemateType::Identifier_decl;
}

int Parser::getBound(std::vector<expression>& source, int start) const {
    return 0;
}

std::unique_ptr<statemates::statemate> Parser::makeStatemate(
    statemates::block* parent,
    StatemateType type,
    std::vector<expression>& code
) const {

    switch (type) {
    case StatemateType::Identifier_decl:
        return std::unique_ptr<statemates::statemate>
            (new statemates::identifier_decl(parent, code));
    case StatemateType::Undeclaration:
        return std::unique_ptr<statemates::statemate>
            (new statemates::identifier_decl(parent, code));
    case StatemateType::Keyword:
        return std::unique_ptr<statemates::statemate>
            (new statemates::identifier_decl(parent, code));
    case StatemateType::Block:
        return this->buildAST(code, new statemates::block(parent));
    case StatemateType::Function_decl:
        return std::unique_ptr<statemates::statemate>
            (new statemates::identifier_decl(parent, code));
    case StatemateType::Assignment:
        return std::unique_ptr<statemates::statemate>
            (new statemates::identifier_decl(parent, code));
    case StatemateType::System_call:
        return std::unique_ptr<statemates::statemate>
            (new statemates::identifier_decl(parent, code));
    default:
        {
            this->context.addError(varam::messages[invalidSyntax], code[0][0].getPosition());
            this->context.setSourceCode(code[0][0].getValue());
            this->context.setStatus(Status::ErrorOccurred);
            return nullptr;
        }
    }
    
}

std::unique_ptr<statemates::block> Parser::buildAST(
    std::vector<expression>& code,
    statemates::block* root
) const {

    if (static_cast<int>(code.size()) == 0) {
        return nullptr;
    }

    for (int i = 0; i < static_cast<int>(code.size()); i++) {
        StatemateType type = this->getType(code, i);
        int bound = this->getBound(code, i);
        std::vector<expression> source = 
            std::vector<expression>(code.begin() + i, code.begin() + bound);

        std::unique_ptr<statemates::statemate> child = this->makeStatemate(root, type, source);
        root->instructions.push_back(std::move(child));

        i = bound;
    }

    return std::unique_ptr<statemates::block>(root);
}

Parser::Parser()
    : context(), ASTroot(std::make_unique<statemates::block>())
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

    try {
        this->ASTroot = std::move(this->buildAST(code, this->ASTroot.get()));
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
