#ifndef AST_H
#define AST_H

#include <AST.inl>

namespace compiler {

	class AST {
	private:
		statemates::ASTnode* root = nullptr;

	public:
		explicit AST(statemates::ASTnode* node) : root(node) {};
		virtual ~AST() { delete this->root; };

		statemates::ASTnode* getRoot() const {
			return this->root;
		};
		boost::json::value toJson() const;
	};

}

#endif
