#ifndef STATEMATES_INL
#define STATEMATES_INL

#include <vector>
#include <memory>
#include <string>
#include <boost/json.hpp>
#include <lexer.h>
#include <common.hpp>

namespace compiler::statemates {

	struct statemate {	// ast node

		using child = statemate;
		using parent = child;
		using child_ptr = std::unique_ptr<child>;
		using parent_ptr = parent*;

		mutable boost::json::object obj;
		parent_ptr root;

		statemate(parent_ptr parent = nullptr)
			: root(parent), obj() {};
		virtual ~statemate() = default;

		virtual bool isNil() const = 0;
		virtual const char* name() const = 0;
		virtual const boost::json::object& toJson() const = 0;
	};

	// PRIME STATEMATES vvv

	struct identifier_decl : statemate {

		std::string identifier;
		boost::json::array qualifiers;	// global | local | ...

		identifier_decl(parent_ptr parent = nullptr)
			: statemate(parent), qualifiers(), identifier() {};
		identifier_decl(parent_ptr parent, std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("identifier_decl::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (expr.back().getType() == TokenType::IDENTIFIER) {
				this->identifier = expr.back().getValue();

				boost::json::array details = {};
				for (const auto& token : expr) {
					if (token.getType() == TokenType::KEYWORD) {
						details.push_back(boost::json::string(token.getValue()));
					}
					else {
						throw std::runtime_error("identifier_decl::invalid syntax : unexpected lexema");
					}
				}
				this->qualifiers = details;
			}
			else {
				throw std::runtime_error("identifier_decl::invalid syntax : missed identifier");
			}
		};
		virtual ~identifier_decl() = default;

		virtual bool isNil() const {
			return true;
		}

		virtual const char* name() const {
			return "identifier_decl";
		}

		virtual const boost::json::object& toJson() const {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["identifier"] = this->identifier;
			this->obj["qualifiers"] = this->qualifiers;

			return this->obj;
		}

	};	// identifier_decl

	struct undeclaration : statemate {

		std::string identifier;
		boost::json::array qualifiers;	// global | local | ...

		undeclaration(parent_ptr parent = nullptr)
			: statemate(parent), qualifiers(), identifier() {};
		undeclaration(parent_ptr parent, std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("identifier_decl::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (expr.size() > 2) {
				throw std::runtime_error("undeclaration::too many arguments");
			}

			if (expr.back().getType() == TokenType::IDENTIFIER) {
				this->identifier = expr.back().getValue();
				std::string keyword = expr.front().getValue();
				if (keyword == COREUNDEF || keyword == CURUNDEF) {
					this->qualifiers = { keyword };
				}
				else {
					throw std::runtime_error("undeclaration::invalid syntax : unexpected lexema");
				}
			}
			else {
				throw std::runtime_error("undeclaration::invalid syntax : missed identifier");
			}
		};
		virtual ~undeclaration() = default;

		virtual bool isNil() const {
			return true;
		}

		virtual const char* name() const {
			return "undeclaration";
		}

		virtual const boost::json::object& toJson() const {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["identifier"] = this->identifier;
			this->obj["qualifiers"] = this->qualifiers;

			return this->obj;
		}

	}; // undeclaration

	struct keyword : statemate {

		std::string word;

		keyword(parent_ptr parent = nullptr)
			: statemate(parent), word() {};
		keyword(parent_ptr parent, std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("identifier_decl::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (expr[0].getType() == TokenType::KEYWORD) {
				this->word = expr[0].getValue();
			}
			else {
				throw std::runtime_error("keyword::ivalid syntax : unexpected lexema");
			}
		};
		virtual ~keyword() = default;

		virtual bool isNil() const {
			return true;
		}

		virtual const char* name() const {
			return "keyword";
		}

		virtual const boost::json::object& toJson() const {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["word"] = this->word;

			return this->obj;
		}

	};	// keyword

	// ^^^ PRIME AST NODES / NON PRIME STATEMATES vvv

	struct block : statemate {

		std::vector<child_ptr> instructions;

		block(parent_ptr parent = nullptr)
			: statemate(parent), instructions() {};
		virtual ~block() = default;

		virtual bool isNil() const {
			return false;
		}

		virtual const char* name() const {
			return "block";
		}

		virtual const boost::json::object& toJson() const {
			this->obj.clear();

			this->obj["type"] = this->name();

			boost::json::array ops{};
			for (auto& op : this->instructions) {
				ops.push_back(op->toJson());
			}
			this->obj["operators"] = ops;

			return this->obj;
		}



	};	// block

	struct function_decl : statemate {

		std::string identifier;
		boost::json::array qualifiers;	// global | local | ...

		child_ptr body;	// block statemate
		int signature;	// count of arguments

		function_decl(parent_ptr parent = nullptr)
			: statemate(parent), identifier(), qualifiers(), body(), signature() {};
		virtual ~function_decl() = default;

		virtual bool isNil() const {
			return false;
		}

		virtual const char* name() const {
			return "function_decl";
		}

		virtual const boost::json::object& toJson() const {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["identifier"] = this->identifier;
			this->obj["qualifiers"] = this->qualifiers;
			this->obj["signature"] = this->signature;
			this->obj["body"] = this->body->toJson();

			return this->obj;
		}

	}; // function_decl

	struct system_call : keyword {

		boost::json::array params;

		system_call(parent_ptr parent = nullptr)
			: keyword(parent), params() {};
		virtual ~system_call() = default;

		virtual bool isNil() const {
			return false;
		}

		virtual const char* name() const {
			return "system_call";
		}

		virtual const boost::json::object& toJson() const {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["word"] = this->word;
			this->obj["parameters"] = this->params;

			return this->obj;
		}

	};	// system_call

	struct assignment : statemate {

		std::string left;	// identifier only
		child_ptr right;	// identifier | binexpr

		assignment(parent_ptr parent = nullptr)
			: statemate(parent), left() {};
		virtual ~assignment() = default;

		virtual bool isNil() const {
			return false;
		}

		virtual const char* name() const {
			return "assignment";
		}

		virtual const boost::json::object& toJson() const {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["left"] = this->left;
			this->obj["right"] = this->right->toJson();

			return this->obj;
		}

	};	// assignment

} // namespace statemates

#endif