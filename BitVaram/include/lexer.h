#ifndef LEXER_H
#define LEXER_H

#include <context.h>
#include <token.h>
#include <config.h>
#include <string>

namespace compiler {

	class Lexer {
		using key = std::string;
		using expression = std::vector<Token>;

	private:
		std::vector<expression> expressions;

		Context context;
		Position pos;

	public:
		explicit Lexer(const varam::Config& config) {};
		~Lexer() = default;

		std::string trimmed(const std::string& line);
		void split(std::string& line, std::vector<key>& seq);

		void process(varam::Config& config);
	};

}

#endif