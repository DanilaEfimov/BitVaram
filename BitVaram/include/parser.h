#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <config.h>
#include <lexer.h>
#include <common.hpp>
#include <AST.h>

namespace compiler {

	class Parser {
		using Key = std::string;
		using Expression = std::vector<Token>;

		enum class Statemate {
			Block,				// operator
			Condition,
			FunctionDeclaration,
			VariableDeclaration,
			Cycle,
			Undeclaration,
			Assignment,
			BinaryExpr,
			FunctionCall,
			Return,
			SystemCall
		};

	private:
		mutable Context context;
		Position pos;

		AST tree;

		Statemate getOperatorBasedType(const Expression& expr) const;
		Statemate getIdentiferBasedType(const Expression& expr) const;
		Statemate getKeywordBasedType(const Expression& expr) const;
		Statemate getType(const Expression& expr) const;

		statemates::ASTnode* buildAST(const std::vector<Expression>& expressions,
			statemates::ASTnode* curRoot);
		statemates::ASTnode* buildASTNil(const Expression& expression,
										statemates::ASTnode* parent);

		int getBlockBound(const std::vector<Expression>& expressions, int start);
		statemates::ASTnode* buildStatemate(const std::vector<Expression>& expressions,
											statemates::ASTnode* parent, Statemate type,
											int start, int end);

		void occureUndefinedStatemate(const Expression& expr) const;
		void occureMissedSymbol(const Token& token, std::string expected) const;

	public:
		Parser() : context(), pos(), tree(nullptr){};
		~Parser() {};

		void process(const std::vector<Expression>& expressions, varam::Config& config);

		const Context& getContext() const;
	};

}	// namespace compiler

#endif	// PARSER_H
