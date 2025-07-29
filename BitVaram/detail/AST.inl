#ifndef STATEMATES_INL
#define STATEMATES_INL

#include <vector>
#include <memory>
#include <string>
#include <boost/json.hpp>
#include <lexer.h>
#include <common.hpp>

using namespace varam;

namespace compiler::statemates {

	enum class StatemateType {
		Identifier_decl,
		Undeclaration,
		Keyword,
		Block,
		Function_decl,
		System_call,
		Assignment
	};

	struct statemate;
	struct block;

	inline StatemateType getType(const expression& expr);

	inline std::unique_ptr<statemate> makeStatemate(
		block* parent,
		StatemateType type,
		const std::vector<expression>& code
	);

	inline int getBound(const std::vector<expression>& source, StatemateType type, int start);

	inline block* buildAST(const std::vector<expression>& code, block* root);

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
		identifier_decl(parent_ptr parent, const std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("identifier_decl::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (expr.back().getType() == TokenType::IDENTIFIER) {
				this->identifier = expr.back().getValue();

				boost::json::array details = {};	// COMMING SOON
				for (int i = 1; i < expr.size() - 1; i++) {
					const auto token = expr[i];
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

		static bool isValidSyntax(const expression& expr) noexcept {
			if (expr.size() >= 2) {
				std::string header = expr[0].getValue();
				if (header == COREDEF || header == CURDEF
					&& expr.back().getType() == TokenType::IDENTIFIER) {
					return true;
				}
			}
			return false;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int line = 0;
			for (const auto& expr : source) {
				if (identifier_decl::isValidSyntax(expr)) {
					return line;
				}
				line++;
			}

			throw std::runtime_error("identifier_decl::invalid syntax");
		}

	};	// identifier_decl

	struct undeclaration : statemate {

		std::string identifier;
		boost::json::array qualifiers;	// global | local | ...

		undeclaration(parent_ptr parent = nullptr)
			: statemate(parent), qualifiers(), identifier() {};
		undeclaration(parent_ptr parent, const std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("undeclaration::source code is empty");
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

		static bool isValidSyntax(const expression& expr) noexcept {
			if (expr.size() >= 2) {
				std::string header = expr[0].getValue();
				if (header == COREUNDEF || header == CURUNDEF
					&& expr.back().getType() == TokenType::IDENTIFIER) {
					return true;
				}
			}
			return false;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int line = 0;
			for (const auto& expr : source) {
				if (expr.size() == 2) {
					std::string header = expr[0].getValue();
					if (header == COREUNDEF || header == CURUNDEF
						&& expr[1].getType() == TokenType::IDENTIFIER) {
						return line;
					}
				}
				line++;
			}

			throw std::runtime_error("undeclaration::invalid syntax");
		}

	}; // undeclaration

	struct keyword : statemate {

		std::string word;

		keyword(parent_ptr parent = nullptr)
			: statemate(parent), word() {};
		keyword(parent_ptr parent, const std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("keyword::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (keyword::isValidSyntax(expr)) {
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

		static int isValidSyntax(const expression& expr) {
			if (expr.size() == 1) {
				if (expr[0].getType() == TokenType::KEYWORD) {
					return true;
				}
			}
			return false;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int line = 0;
			for (const auto& expr : source) {
				if (keyword::isValidSyntax(expr)) {
					return line;
				}
				line++;
			}

			throw std::runtime_error("keyword::invalid syntax");
		}

	};	// keyword

	struct system_call : keyword {

		boost::json::array params;

		system_call(parent_ptr parent = nullptr)
			: keyword(parent), params() {};
		system_call(parent_ptr parent,const std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("system_call::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (expr[0].getType() == TokenType::KEYWORD) {
				this->word = expr[0].getValue();
			}

			boost::json::array args = {};
			for (int i = 1; i < expr.size(); i++) {
				if (expr[i].getType() == TokenType::IDENTIFIER ||
					expr[i].getType() == TokenType::NUMBER) {
					args.push_back(boost::json::string(expr[i].getValue()));
				}
				else {
					throw std::runtime_error("system_call::unexpected lexema");
				}
			}
			this->params = args;
		};
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

		static int isValidSyntax(const expression& expr) {
			if (expr.size() > 1 &&
				expr[0].getType() == TokenType::KEYWORD) {
				TokenType type;
				for (int i = 1; i < expr.size(); i++) {
					type = expr[i].getType();
					if (type != TokenType::IDENTIFIER &&
						type != TokenType::NUMBER) {
						return false;
					}
				}
				return true;
			}
			return false;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int line = 0;
			for (const auto& expr : source) {
				if (system_call::isValidSyntax(expr)) {
					return line;
				}
				line++;
			}

			throw std::runtime_error("system_call::invalid syntax");
		}

	};	// system_call

	// ^^^ PRIME AST NODES / NON PRIME STATEMATES vvv

	struct block : statemate {

		std::vector<child_ptr> instructions;

		block(parent_ptr parent = nullptr)
			: statemate(parent), instructions() {};
		block(parent_ptr parent, const std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("block::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (block::isValidSyntax(expr)) {
				int end = block::getBlockBound(source);
				std::vector<expression> code =
					std::vector<expression>(source.begin(), source.begin() + end);
				for (int i = 0; i < static_cast<int>(code.size());) {
					StatemateType type = getType(code[i]);
					int start = i;
					int end = getBound(code, type, i) + 1;

					if (start >= end) {
						throw std::runtime_error("undefined error : bound searching crashed");
						break;
					}
					std::vector<expression> source =
						std::vector<expression>(code.begin() + start, code.begin() + end);

					std::unique_ptr<statemate> child = makeStatemate(this, type, source);
					this->instructions.push_back(std::move(child));

					i = end;
				}
			}
			throw std::runtime_error("block::invalid syntax : missed open bracket");
		};
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

		static int getBlockBound(const std::vector<expression>& source) {
			int opened = 0;

			std::string header;
			int line = 0;
			for (const auto& expr : source) {
				header = expr[0].getValue();
				if (header == BLOCK_OPEN_BRACKET) {
					opened++;
				}
				if (header == BLOCK_CLOSE_BRACKET) {
					opened--;
					if (opened == 0) {
						return line;
					}
					if (opened < 0) {
						throw std::runtime_error("block::missed opening block bracket");
					}
				}
				line++;
			}

			throw std::runtime_error("block::missed closing block bracket");
		}

		static bool isValidSyntax(const expression& expr) {
			return expr[0].getValue() == BLOCK_OPEN_BRACKET
				&& expr.size() == 1;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			return block::getBlockBound(source);
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

	StatemateType getType(const expression& expr) {
		if (statemates::identifier_decl::isValidSyntax(expr))
			return StatemateType::Identifier_decl;
		if (statemates::undeclaration::isValidSyntax(expr))
			return StatemateType::Undeclaration;
		if (statemates::keyword::isValidSyntax(expr))
			return StatemateType::Keyword;
		if (statemates::system_call::isValidSyntax(expr)) {
			return StatemateType::System_call;
		}
		throw std::runtime_error("invalid syntax");
	};

	std::unique_ptr<statemate> makeStatemate(
		block* parent,
		StatemateType type,
		const std::vector<expression>& code
	) {
		switch (type) {
		case StatemateType::Identifier_decl:
			return std::unique_ptr<statemate>
				(new identifier_decl(parent, code));
		case StatemateType::Undeclaration:
			return std::unique_ptr<statemate>
				(new undeclaration(parent, code));
		case StatemateType::Keyword:
			return std::unique_ptr<statemate>
				(new keyword(parent, code));
		case StatemateType::System_call:
			return std::unique_ptr<statemate>
				(new system_call(parent, code));
		case StatemateType::Block:
			return std::unique_ptr<statemate>
				(new block(parent));
		case StatemateType::Function_decl:
			return std::unique_ptr<statemate>
				(new function_decl(parent));
		case StatemateType::Assignment:
			return std::unique_ptr<statemate>
				(new assignment(parent));
		default:
		{
			return nullptr;
		}
		}
	};

	int getBound(const std::vector<expression>& source, StatemateType type, int start) {
		std::vector<expression> code =
			std::vector<expression>(source.begin() + start, source.end());
		int end = start;
		switch (type) {
		case StatemateType::Identifier_decl:
			end += identifier_decl::getStatemateBound(code);
			break;
		case StatemateType::Undeclaration:
			end += undeclaration::getStatemateBound(code);
			break;
		case StatemateType::Keyword:
			end += keyword::getStatemateBound(code);
			break;
		case StatemateType::System_call:
			end += system_call::getStatemateBound(code);
			break;
		case StatemateType::Block:
			end += block::getStatemateBound(code);
			break;
		case StatemateType::Function_decl:
		case StatemateType::Assignment:
		default:
		{
			return 0;
		}
		}

		return end;
	}

	block* buildAST(
		const std::vector<expression>& code,
		block* root
	) {

		if (static_cast<int>(code.size()) == 0) {
			return nullptr;
		}

		for (int i = 0; i < static_cast<int>(code.size());) {
			StatemateType type = statemates::getType(code[i]);
			int start = i;
			int end = getBound(code, type, i) + 1;

			if (start >= end) {
				throw std::runtime_error("undefined error : bound searching crashed");
				break;
			}
			std::vector<expression> source =
				std::vector<expression>(code.begin() + start, code.begin() + end);

			std::unique_ptr<statemate> child = makeStatemate(root, type, source);
			root->instructions.push_back(std::move(child));

			i = end;
		}

		return root;
	}

} // namespace statemates


#endif