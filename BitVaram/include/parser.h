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
			UnarExpr,
			BinaryExpr,
			FunctionCall,
			Return,
			SystemCall
		};

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

	public:
		Parser() : context(), pos(), tree(nullptr){};
		~Parser() {};

		void process(const std::vector<Expression>& expressions, varam::Config& config);

		const Context& getContext() const;

	protected:

		// prime statemates

		statemates::statemate* makeAssigment(const std::vector<Expression>& expressions,
											Statemate type,
											int start, int end);

		statemates::statemate* makeFunctionCall(const std::vector<Expression>& expressions,
											Statemate type,
											int start, int end);

		statemates::statemate* makeReturn(const std::vector<Expression>& expressions,
											Statemate type,
											int start, int end);

		statemates::statemate* makeSystemCall(const std::vector<Expression>& expressions,
											Statemate type,
											int start, int end);

		statemates::statemate* makeUndeclaration(const std::vector<Expression>& expressions,
											Statemate type,
											int start, int end);

		statemates::statemate* makeUnarExpr(const std::vector<Expression>& expressions,
											Statemate type,
											int start, int end);

	};

}	// namespace compiler

#endif	// PARSER_H
