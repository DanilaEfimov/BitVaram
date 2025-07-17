#include <parser.h>
#include <statemate.inl>
#include <iostream>

using namespace varam;
using namespace compiler;


const std::set<Parser::Statemate> Parser::primes = {   // statemates, which cannot have child in AST
    Statemate::Assignment,
    Statemate::FunctionCall,
    Statemate::Return,
    Statemate::SystemCall,
    Statemate::Undeclaration,
    Statemate::UnarExpr
};

Parser::Statemate Parser::getOperatorBasedType(const Expression& expr) const {
    std::string header = expr[0].getValue();
    if (expr.size() == 2) {
        TokenType type = expr[1].getType();
        if (header == NOT && type == TokenType::IDENTIFIER) 
            { return Statemate::UnarExpr; }
    }

    if (expr.size() == 1) {
        if (LangFrame::isOpening(header)) 
            { return Statemate::Block; }
    }
    this->occureUndefinedStatemate(expr);
}

Parser::Statemate Parser::getIdentiferBasedType(const Expression& expr) const {
    if (expr.size() > 2) {
        if (expr[1].getValue() == EQUALS) { return Statemate::Assignment; }
    }
    this->occureUndefinedStatemate(expr);
}

Parser::Statemate Parser::getKeywordBasedType(const Expression& expr) const {
    std::string header = expr[0].getValue();

    if (expr.size() > 2) {
        TokenType type = expr[1].getType();
        if ((header == COREDEF || header == CURDEF) 
            && type == TokenType::IDENTIFIER)
            { return Statemate::FunctionDeclaration; }
    }

    if (expr.size() == 2) {
        TokenType type = expr[1].getType();
        if (header == COREUNDEF && type == TokenType::IDENTIFIER) 
            { return Statemate::Undeclaration; }
        if (header == CURUNDEF  && type == TokenType::IDENTIFIER) 
            { return Statemate::Undeclaration; }
        if (header == IF        && type == TokenType::IDENTIFIER) 
            { return Statemate::Condition; }
        if (header == FOR       && type == TokenType::IDENTIFIER) 
            { return Statemate::Cycle; }
        if (header == STORE     && type == TokenType::IDENTIFIER)
            { return Statemate::SystemCall; }
        if (header == RETURN    && type == TokenType::IDENTIFIER) 
            { return Statemate::Return; }
        if (header == EXIT      && type == TokenType::NUMBER 
            || type == TokenType::IDENTIFIER) 
            { return Statemate::SystemCall; }
    }

    if (expr.size() == 1) {
        if (header == SHOW) { return Statemate::SystemCall; }
    }

    this->occureUndefinedStatemate(expr);
}

Parser::Statemate Parser::getType(const Expression& expr) const {
    Token header = expr[0];
    if (header.getType() == TokenType::KEYWORD) {
        return this->getKeywordBasedType(expr);
    }

    if (header.getType() == TokenType::IDENTIFIER) {
        return this->getIdentiferBasedType(expr);
    }

    if (header.getType() == TokenType::OPERATOR) {
        return this->getOperatorBasedType(expr);
    }

    this->occureUndefinedStatemate(expr);
}

inline bool Parser::isASTNil(Statemate type) const {
    return Parser::primes.contains(type);
}

statemates::ASTnode* Parser::buildAST(
    const std::vector<Expression>& expressions, // source tokenized code
	statemates::ASTnode* curRoot,               // current AST root
    int start, int end) {                       // start & end of scope
    if (end - start == 1) {  
        // terminal branch
        // every prime statemate have be writen in one line
        Statemate type = this->getType(expressions[0]);
        statemates::ASTnode* child = new statemates::ASTnode(curRoot);
        child->statemate = this->buildStatemate(expressions, type, 0, 0);
        return child;
    }

    for (int i = start; i < end; i++) {  // reccursion branch
        
        Statemate type = this->getType(expressions[i]);
        int closing = this->getStatemateBound(expressions, type, i);

        // building AST node
        statemates::ASTnode* child = this->buildAST(expressions, curRoot, i, closing + 1);
        child->statemate = this->buildStatemate(expressions, type, i, closing);
        curRoot->append(child);

        i = closing;
    }

    return curRoot;
}

statemates::statemate* Parser::buildASTNil(
    const Expression& expression,
    Statemate type) {
    return nullptr;
}

int Parser::getStatemateBound(const std::vector<Expression>& expressions, Statemate type, int start) {
    if (this->isASTNil(type)) {
        return start;
    }

    if (expressions[start][0].getValue() != BLOCK_OPEN_BRACKET) {
        this->occureMissedSymbol(expressions[start][0], BLOCK_OPEN_BRACKET);
    }

    int opened = 1;
    for (int i = start + 1; i < expressions.size(); i++) {
        std::string value = expressions[i][0].getValue();
        if (opened == 1 && value == BLOCK_CLOSE_BRACKET) {
            return i;
        }
        if (value == BLOCK_OPEN_BRACKET) {
            opened++;
        }
        else if (value == BLOCK_CLOSE_BRACKET){
            opened--;
        }
    }
    
    Token last = expressions.back().back(); // last token in source
    this->occureMissedSymbol(last, BLOCK_CLOSE_BRACKET);
}

statemates::statemate* Parser::buildStatemate(
    const std::vector<Expression>& expressions,
    Statemate type,
    int start, int end) {
    if (this->isASTNil(type)) {
        return this->buildASTNil(expressions[start], type);
    }
    return nullptr;
}

void Parser::occureUndefinedStatemate(const Expression& expr) const {
    Position pos = expr[0].getPosition();   // statemate position
    this->context.setStatus(Status::Aborted);
    this->context.addError(varam::messages[invalidSyntax], pos);

    throw std::runtime_error("invalid syntax, parsing aborted");
}

void Parser::occureMissedSymbol(const Token& token, std::string expected) const {
    this->context.setStatus(Status::Aborted);
    this->context.addError(varam::messages[missingSymbol] 
        + " expected: '" + expected + "'", token.getPosition());

    throw std::runtime_error("missing symbol, parsing aborted");
}

void Parser::process(const std::vector<Expression>& expressions, varam::Config& config) {
	this->context.setPhase(Phase::Parsing);
	this->context.setStatus(Status::Started);

	this->tree = AST(nullptr);

    try {
        // default calling with all-scope args
        this->buildAST(expressions, this->tree.getRoot(), 0, expressions.size());
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

const Context& Parser::getContext() const {
    return this->context;
}

// prime statemates

statemates::statemate* Parser::makeAssigment(const std::vector<Expression>& expressions,
    Statemate type,
    int start, int end) {
    return nullptr;
}

statemates::statemate* Parser::makeFunctionCall(const std::vector<Expression>& expressions,
    Statemate type,
    int start, int end) {
    return nullptr;
}

statemates::statemate* Parser::makeReturn(const std::vector<Expression>& expressions,
    Statemate type,
    int start, int end) {
    return nullptr;
}

statemates::statemate* Parser::makeSystemCall(const std::vector<Expression>& expressions,
    Statemate type,
    int start, int end) {
    return nullptr;
}

statemates::statemate* Parser::makeUndeclaration(const std::vector<Expression>& expressions,
    Statemate type,
    int start, int end) {
    return nullptr;
}

statemates::statemate* Parser::makeUnarExpr(const std::vector<Expression>& expressions,
    Statemate type,
    int start, int end) {
    return nullptr;
}