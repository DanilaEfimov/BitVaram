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
			Block,              // operator
			Condition,
			FunctionDeclaration,
			VariableDeclaration,
			Cycle,
			Undeclaration,
			Assignment,
			UnarExpr,
			BinaryExpr,
			FunctionCall,
			Return,
			SystemCall
		};

		inline constexpr std::string to_string(Statemate s) const {
			switch (s) {
				case Statemate::Block:					return "Block";
				case Statemate::Condition:				return "Condition";
				case Statemate::FunctionDeclaration:	return "FunctionDeclaration";
				case Statemate::VariableDeclaration:	return "VariableDeclaration";
				case Statemate::Cycle:					return "Cycle";
				case Statemate::Undeclaration:			return "Undeclaration";
				case Statemate::Assignment:				return "Assignment";
				case Statemate::UnarExpr:				return "UnarExpr";
				case Statemate::BinaryExpr:				return "BinaryExpr";
				case Statemate::FunctionCall:			return "FunctionCall";
				case Statemate::Return:					return "Return";
				case Statemate::SystemCall:				return "SystemCall";
			default:								return "Unknown";
			}
		}


		static const std::set<Statemate> primes;

	private:
		mutable Context context;
		Position pos;

		AST tree;

		Statemate getOperatorBasedType(const Expression& expr) const;
		Statemate getIdentiferBasedType(const Expression& expr) const;
		Statemate getKeywordBasedType(const Expression& expr) const;
		Statemate getType(const Expression& expr) const;

		inline bool isASTNil(Statemate type) const;

		statemates::ASTnode* buildAST(const std::vector<Expression>& expressions,
											statemates::ASTnode* curRoot,
											int start, int end);
		statemates::statemate* buildASTNil(const Expression& expression,
											Statemate type);

		int getStatemateBound(const std::vector<Expression>& expressions, Statemate type, int start);
		statemates::statemate* buildStatemate(const std::vector<Expression>& expressions,
											Statemate type,
											int start, int end);

		void occureUndefinedStatemate(const Expression& expr) const;
		void occureMissedSymbol(const Token& token, std::string expected) const;
		void occureInvalidSyntax(Position pos, Statemate type) const;

	public:
		Parser() : context(), pos(), tree(nullptr){};
		~Parser() {};

		void process(const std::vector<Expression>& expressions, varam::Config& config);

		const Context& getContext() const;

	protected:

		// prime statemates

		statemates::statemate* makeAssigment(const Expression& expression);

		statemates::statemate* makeFunctionCall(const Expression& expression);

		statemates::statemate* makeReturn(const Expression& expression);

		statemates::statemate* makeSystemCall(const Expression& expression);

		statemates::statemate* makeUndeclaration(const Expression& expression);

		statemates::statemate* makeUnarExpr(const Expression& expression);

	};

}	// namespace compiler

#endif	// PARSER_H
