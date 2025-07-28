#ifndef LEXER_H
#define LEXER_H

#include <context.h>
#include <token.h>
#include <config.h>
#include <string>

namespace compiler {

	using key = std::string;
	using expression = std::vector<Token>;

	class Lexer {

	private:
		std::vector<expression> expressions;

		Context context;
		Position pos;

		void trim(std::string& line, size_t& col);
		void split(std::string& line, expression& seq, Position pos);

	public:
		explicit Lexer() {};
		~Lexer() = default;

		void process(varam::Config& config);

		const std::vector<expression>& getExpressions() const;
		const Context& getContext() const;
	};

}	// namespace compiler

#endif	// LEXER_H