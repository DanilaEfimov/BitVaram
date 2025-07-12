#ifndef AST_H
#define AST_H

#include <AST.inl>

namespace compiler {

	class AST {
	private:
		statemates::StBlock main;

	public:
		AST() = default;
		~AST() = default;

		const statemates::StBlock& getMain() const { return this->main; }

	};

}	// namespace compiler

#endif	// AST_H
