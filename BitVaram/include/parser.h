#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <AST.inl>
#include <context.h>
#include <config.h>
#include <lexer.h>

using namespace varam;

namespace compiler {

	inline constexpr const char* parsedPrefix = ".json";

	class Parser {

		enum class StatemateType {
			Identifier_decl,
			Undeclaration,
			Keyword,
			Block,
			Function_decl,
			System_call,
			Assignment
		};

		mutable Context context;

		std::unique_ptr<statemates::block> ASTroot;

		StatemateType getType(std::vector<expression>& source, int start) const;

		int getBound(std::vector<expression>& source, int start) const;

		std::unique_ptr<statemates::statemate> makeStatemate(statemates::block* parent,
			StatemateType type, std::vector<expression>& code) const;

		std::unique_ptr<statemates::block> buildAST(std::vector<expression>& code,
			statemates::block* root) const;

	public:

		explicit Parser();
		virtual ~Parser() = default;

		static std::string replacedPrefixName(const std::string& filename);

		void process(varam::Config& config, std::vector<expression>& code);

		const Context& getContext() const;

	};	// class Parser

}	// namespace compiler

#endif	// PARSER_H