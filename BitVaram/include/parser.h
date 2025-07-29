#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <AST.inl>
#include <context.h>
#include <config.h>
#include <lexer.h>

namespace compiler {

	inline constexpr const char* parsedPrefix = ".json";

	class Parser {

		mutable Context context;

		statemates::block* ASTroot;

		statemates::StatemateType getTypeImpl(const expression& source) const;

		int getBoundImpl(const std::vector<expression>& source, statemates::StatemateType type, int start) const;

		std::unique_ptr<statemates::statemate> makeStatemateImpl(statemates::block* parent,
			statemates::StatemateType type, const std::vector<expression>& code) const;

		statemates::block* buildASTImpl(const std::vector<expression>& code,
			statemates::block* root) const;

		void occureSuccessState(varam::Config& config) const;

	public:

		explicit Parser();
		virtual ~Parser() = default;

		static std::string replacedPrefixName(const std::string& filename);

		void process(varam::Config& config, const std::vector<expression>& code);

		const Context& getContext() const;

	};	// class Parser

}	// namespace compiler

#endif	// PARSER_H