#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <config.h>
#include <lexer.h>
#include <common.hpp>

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
			Undeclaration
		};

	private:
		Context context;
		Position pos;

	public:
		Parser() = default;
		~Parser() = default;

		void process(const std::vector<Expression>& expressions, varam::Config& config);
	};

}	// namespace compiler

#endif	// PARSER_H
