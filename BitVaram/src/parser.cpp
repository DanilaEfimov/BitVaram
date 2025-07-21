#include <parser.h>
#include <iostream>

using namespace varam;
using namespace compiler;


const std::set<Parser::Statemate> Parser::primes = {   // statemates, which cannot have child in AST
    Statemate::Assignment,          // [identifier] [=] [binexpr]
    Statemate::FunctionCall,        // [identifier] [(] [parameters] [)]
    Statemate::Return,              // [return] [identifier/number]
    Statemate::SystemCall,          // [keyword] (parameter)
    Statemate::Undeclaration,       // [curundef/coreundef] [identifier]
    Statemate::UnarExpr             // [operator] [identifier]
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
    switch (type) {
        case Statemate::Assignment: return this->makeAssigment(expression); break;
        case Statemate::FunctionCall: break;
        case Statemate::Return: break;
        case Statemate::SystemCall: break;
        case Statemate::Undeclaration: break;
        case Statemate::UnarExpr: break;
    default:
        this->occureUndefinedStatemate(expression);
    }
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
    if (this->isASTNil(type)) { // terminal branch
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

void Parser::occureInvalidSyntax(Position pos, Statemate type) const {
    this->context.setStatus(Status::ErrorOccurred);
    this->context.addError(varam::messages[invalidSyntax]
        + " " + this->to_string(type), pos);
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

statemates::statemate* Parser::makeAssigment(const Expression& expression) {
    int tokensCount = expression.size();
    statemates::StAssignment* assignment = new statemates::StAssignment();

    if (tokensCount < 3) {
        this->occureInvalidSyntax(expression[0].getPosition(), Statemate::Assignment);
        delete assignment;
        return nullptr;
    }

    if (expression[0].getType() == TokenType::IDENTIFIER
        && expression[1].getValue() == EQUALS) {
        assignment->identifier = expression[0].getValue();

        if (tokensCount == 3    // identifier = identifier
            && (expression[2].getType() == TokenType::IDENTIFIER
            || expression[2].getType()  == TokenType::NUMBER)) {
            
            assignment->expression = {expression[2].getValue()};
            return assignment;
        }

        if (tokensCount == 5    // identifier = binexpr
            && (expression[2].getType() == TokenType::IDENTIFIER
            || expression[2].getType()  == TokenType::NUMBER)
            && expression[3].getType()  == TokenType::OPERATOR
            && (expression[4].getType() == TokenType::IDENTIFIER
            || expression[4].getType()  == TokenType::NUMBER)) {   
            
            assignment->expression = {  expression[2].getValue(), 
                                        expression[3].getValue(), 
                                        expression[4].getValue()  };
            return assignment;
        }

        if (tokensCount >= 3) {
            this->occureInvalidSyntax(expression[2].getPosition(), Statemate::Assignment);
        }
    }

    this->occureInvalidSyntax(expression[0].getPosition(), Statemate::Assignment);
    delete assignment;
    return nullptr;
}

statemates::statemate* Parser::makeFunctionCall(const Expression& expression) {
    int tokensCount = expression.size();
    statemates::StFunctionCall* call = new statemates::StFunctionCall();

    if (tokensCount < 3) {
        this->occureInvalidSyntax(expression[0].getPosition(), Statemate::FunctionCall);
        goto fail;
    }

    if (expression[0].getType()             == TokenType::IDENTIFIER
        && expression[1].getValue()         == FUNC_OPEN_BRACKET
        && expression.back().getValue()     == FUNC_CLOSE_BRACKET) {

        boost::json::array args{};
        for (int i = 2; i < tokensCount - 1; i++) {
            if (expression[i].getType()     == TokenType::IDENTIFIER
                || expression[i].getType()  == TokenType::NUMBER) {

                args.push_back(boost::json::value_from(expression[i].getValue()));

                if (++i < tokensCount - 1) {
                    expression[i].getType() == TokenType::SEPARATOR;    // ',' comma
                }
            }
            else {
                this->occureInvalidSyntax(expression[i].getPosition(), Statemate::FunctionCall);
                goto fail;
            }
        }

        call->arguments = args;
        call->name = expression[0].getValue();

        return call;
    }

    this->occureInvalidSyntax(expression[2].getPosition(), Statemate::FunctionCall);

    fail:
    delete call;
    return nullptr;
}

statemates::statemate* Parser::makeReturn(const Expression& expression) {
    int tokensCount = expression.size();
    statemates::StReturn* ret = new statemates::StReturn();

    if (tokensCount != 2) {
        this->occureInvalidSyntax(expression[0].getPosition(), Statemate::Return);
        goto fail;
    }

    if (expression[0].getValue() == RETURN
        && expression[1].getType() == TokenType::IDENTIFIER) {
        ret->identifier = expression[1].getValue();
        return ret;
    }

    fail:
    delete ret;
    return nullptr;
}

statemates::statemate* Parser::makeSystemCall(const Expression& expression) {
    int tokensCount = expression.size();
    statemates::StSystemCall* syscall = new statemates::StSystemCall();

    if (tokensCount < 2) {
        this->occureInvalidSyntax(expression[0].getPosition(), Statemate::Return);
        goto fail;
    }

    

    fail:
    delete syscall;
    return nullptr;
}

statemates::statemate* Parser::makeUndeclaration(const Expression& expression) {
    int tokensCount = expression.size();

    if (tokensCount == 2) {
        if (expression[0].getValue() == CURUNDEF 
            || expression[0].getValue() == COREUNDEF
            && expression[1].getType() == TokenType::IDENTIFIER) {
            statemates::StUndeclaration* undeclaration = new statemates::StUndeclaration();
            undeclaration->identifier = expression[1].getValue();
            return undeclaration;
        }
    }

    this->occureInvalidSyntax(expression[0].getPosition(), Statemate::Undeclaration);
    return nullptr;
}

statemates::statemate* Parser::makeUnarExpr(const Expression& expression) {
    int tokensCount = expression.size();

    if (tokensCount == 2) {
        if (expression[0].getValue() == NOT
            && expression[1].getType() == TokenType::IDENTIFIER) {
            statemates::StUnarExpression* unarexpr = new statemates::StUnarExpression();
            unarexpr->identifier = expression[1].getValue();
            return unarexpr;
        }
    }

    this->occureInvalidSyntax(expression[0].getPosition(), Statemate::UnarExpr);
    return nullptr;
}