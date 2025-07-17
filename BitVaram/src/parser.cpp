#include <parser.h>
#include <iostream>

using namespace varam;
using namespace compiler;

/*
������� buildAST(������ ���������, ������� �������):

    ��� ������� ��������� � ������:
        ��� = getType(���������)

        ���� ��� == VariableDeclaration:
            ������� StVariableDeclaration node
            node.identifier = ���������.���
            node.expression = ���������.��������
            �������� node � ASTnode � �������� � ������� �������

        ����� ���� ��� == Assignment:
            ������� StAssignment node
            node.identifier = ���������.���
            node.expression = ���������.��������
            �������� node � ASTnode � �������� � ������� �������

        ����� ���� ��� == FunctionDeclaration:
            ������� StFunctionDeclaration node
            node.name = ���������.���
            node.parameters = ���������.���������
            node.isglobal = ���������.������������
            ������� ����� ������� bodyNode ��� ���� �������
            buildAST(���������.����, bodyNode)
            node.body = ���������� bodyNode
            �������� node � ASTnode � �������� � ������� �������

        ����� ���� ��� == Condition:
            ������� StConditionBlock node
            node.expression = ���������.�������
            buildAST(���������.trueBranch, node.trueBranch)
            buildAST(���������.falseBranch, node.falseBranch)
            �������� node � ASTnode � �������� � ������� �������

        ����� ���� ��� == Cycle:
            ������� StCycle node
            node.parameterIdentifier = ���������.��������
            buildAST(���������.����, node.body)
            �������� node � ASTnode � �������� � ������� �������

        ����� ���� ��� == FunctionCall:
            ������� StFunctionCall node
            node.name = ���������.���
            node.arguments = ���������.���������
            �������� node � ASTnode � �������� � ������� �������

        ����� ���� ��� == BinaryExpression:
            ������� StBinaryExpression node
            node.op = ���������.��������
            node.left = ���������.����
            node.right = ���������.�����
            �������� node � ASTnode � �������� � ������� �������

        �����:
            ������� ������ "����������� ��� ���������"
*/

Parser::Statemate Parser::getOperatorBasedType(const Expression& expr) const {
    return Statemate::Block;
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
    for (int i = 0; i < expressions.size(); i++) {
        Statemate type = this->getType(expressions[i]);
        // get statemate bounds
        // build statemate node and append to curRoot.childs
    }
    return curRoot;
}

void Parser::occureUndefinedStatemate(const Expression& expr) const {
    Position pos = expr[0].getPosition();   // statemate position
    this->context.setStatus(Status::Aborted);
    this->context.addError(varam::messages[invalidSyntax], pos);

    throw std::runtime_error("invalid syntax, parsing aborted");
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