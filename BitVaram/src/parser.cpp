#include <parser.h>
#include <iostream>

using namespace varam;
using namespace compiler;

/*
Функция buildAST(список выражений, текущая вершина):

    для каждого выражения в списке:
        тип = getType(выражение)

        если тип == VariableDeclaration:
            создать StVariableDeclaration node
            node.identifier = выражение.имя
            node.expression = выражение.значение
            обернуть node в ASTnode и добавить к текущей вершине

        иначе если тип == Assignment:
            создать StAssignment node
            node.identifier = выражение.имя
            node.expression = выражение.значение
            обернуть node в ASTnode и добавить к текущей вершине

        иначе если тип == FunctionDeclaration:
            создать StFunctionDeclaration node
            node.name = выражение.имя
            node.parameters = выражение.параметры
            node.isglobal = выражение.глобальность
            создать новую вершину bodyNode для тела функции
            buildAST(выражение.тело, bodyNode)
            node.body = содержимое bodyNode
            обернуть node в ASTnode и добавить к текущей вершине

        иначе если тип == Condition:
            создать StConditionBlock node
            node.expression = выражение.условие
            buildAST(выражение.trueBranch, node.trueBranch)
            buildAST(выражение.falseBranch, node.falseBranch)
            обернуть node в ASTnode и добавить к текущей вершине

        иначе если тип == Cycle:
            создать StCycle node
            node.parameterIdentifier = выражение.итератор
            buildAST(выражение.тело, node.body)
            обернуть node в ASTnode и добавить к текущей вершине

        иначе если тип == FunctionCall:
            создать StFunctionCall node
            node.name = выражение.имя
            node.arguments = выражение.аргументы
            обернуть node в ASTnode и добавить к текущей вершине

        иначе если тип == BinaryExpression:
            создать StBinaryExpression node
            node.op = выражение.оператор
            node.left = выражение.лево
            node.right = выражение.право
            обернуть node в ASTnode и добавить к текущей вершине

        иначе:
            вывести ошибку "Неизвестный тип выражения"
*/

Parser::Statemate Parser::getOperatorBasedType(const Expression& expr) const {
    if (expr.size() == 1) {
        std::string header = expr[0].getValue();
        if (LangFrame::isOpening(header)) { return Statemate::Block; }
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

statemates::ASTnode* Parser::buildAST(const std::vector<Expression>& expressions,
	statemates::ASTnode* curRoot) {
    if (expressions.size() == 1) {  // terminal branch
        return this->buildASTNil(expressions[0], curRoot);
    }

    for (int i = 0; i < expressions.size(); i++) {
        Statemate type = this->getType(expressions[i]);
        int end = this->getBlockBound(expressions, i);
        
    }
    return curRoot;
}

statemates::ASTnode* Parser::buildASTNil(const Expression& expression,
    statemates::ASTnode* parent) {
    Statemate type = this->getType(expression);
    return nullptr;
}

int Parser::getBlockBound(const std::vector<Expression>& expressions, int start) {
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
    
    Token last = expressions.back().back();
    this->occureMissedSymbol(last, BLOCK_CLOSE_BRACKET);
}

statemates::ASTnode* Parser::buildStatemate(const std::vector<Expression>& expressions,
    statemates::ASTnode* parent, Statemate type,
    int start, int end) {
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
        this->buildAST(expressions, this->tree.getRoot());
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

const Context& Parser::getContext() const {
    return this->context;
}