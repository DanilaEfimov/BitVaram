#ifndef STATEMATES_INL
#define STATEMATES_INL

#include <vector>
#include <memory>
#include <string>
#include <iostream>
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
		Assignment,
		Binexpr,
		Condition,
		Cycle
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

	struct binexpr : statemate {

		std::string lhs;
		std::string rhs;
		std::string op;

		binexpr(parent_ptr parent = nullptr)
			: statemate(parent), lhs(), rhs(), op() {}
		binexpr(parent_ptr parent, const std::vector<expression>& source) {
			if (source.size() == 0) {
				throw std::runtime_error("binexpr::empty source code");
			}

			expression expr = source[0];
			if (expr.size() != 3) {
				throw std::runtime_error("binexpr::invalid syntax : expected 3 tokens");
			}
			if (expr[0].getType() == TokenType::IDENTIFIER &&
				expr[1].getType() == TokenType::OPERATOR &&
				expr[2].getType() == TokenType::IDENTIFIER ||
				expr[2].getType() == TokenType::NUMBER) {

				this->root = parent;
				this->lhs = expr[0].getValue();
				this->op = expr[1].getValue();
				this->rhs = expr[2].getValue();	// identifier or number literal
			}
			else {
				throw std::runtime_error("binexpr::invalid syntax");
			}
		};
		virtual ~binexpr() = default;

		virtual bool isNil() const override {
			return true;
		}

		virtual const char* name() const override {
			return "binexpr";
		}

		static bool isNumber(const std::string& str) {
			try {
				size_t pos;
				std::stoi(str, &pos);
				return pos == str.size();  // ensures full conversion
			}
			catch (...) {
				return false;
			}
		}

		virtual const boost::json::object& toJson() const override {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["left"] = this->lhs;
			this->obj["operator"] = this->op;
			if (binexpr::isNumber(this->rhs)) {
				this->obj["right"] = std::stoi(this->rhs);
			}
			else {
				this->obj["right"] = this->rhs;
			}

			return this->obj;
		}

		static bool isValidSyntax(const expression& expr) {
			return expr.size() == 3 &&
				(expr[0].getType() == TokenType::IDENTIFIER || expr[0].getType() == TokenType::NUMBER) &&
				(expr[1].getType() == TokenType::OPERATOR || expr[1].getType() == TokenType::KEYWORD) &&
				(expr[2].getType() == TokenType::IDENTIFIER || expr[2].getType() == TokenType::NUMBER);
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int line = 0;
			for (const auto& expr : source) {
				if (binexpr::isValidSyntax(expr)) {
					return line;
				}
				line++;
			}

			throw std::runtime_error("binexpr::invalid syntax");
		}

	};	// binexpr

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
					std::vector<expression>(source.begin() + 1, source.begin() + end);
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
			else
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
		function_decl(parent_ptr parent, const std::vector<expression>& source) {
			if (source.empty()) {
				throw std::invalid_argument("function_decl::source code is empty");
			}

			this->root = parent;
			expression expr = source[0];
			if (expr.size() >= 3) {
				int size = expr.size();
				bool valid = false;
				if (expr[size - 1].getType() == TokenType::NUMBER) {
					this->signature = std::stoi(expr[size - 1].getValue());
					valid = true;
				}
				if (expr[size - 2].getType() == TokenType::IDENTIFIER && valid) {
					this->identifier = expr[size - 2].getValue();
				}
				if (!valid) {
					throw std::runtime_error("function_decl::invalid syntax : unexpected lexema");
				}

				boost::json::array details = {};	// COMMING SOON
				for (int i = 0; i < size - 2; i++) {
					if (expr[i].getType() == TokenType::KEYWORD) {
						details.push_back(boost::json::string(expr[i].getValue()));
					}
					else {
						throw std::runtime_error("function_decl::invalid syntax : unexpected lexema");
					}
				}
				this->qualifiers = details;

				this->body = std::make_unique<statemates::block>
					(this, std::vector<expression>(source.begin() + 1, source.end()));
			}
			else {
				invalid_syntax:
				throw std::runtime_error("function_decl::invalid syntax");
			}
		};
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

		static bool isValidSyntax(const expression& expr) {
			if (expr.size() >= 3) {
				int size = expr.size();
				if (expr[size - 1].getType() == TokenType::NUMBER &&
					expr[size - 2].getType() == TokenType::IDENTIFIER) {
					for (int i = 0; i < size - 2; i++) {
						if (expr[i].getType() != TokenType::KEYWORD) {
							return false;
						}
					}
					return true;
				}
			}
			return false;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int start = 0;
			for (int i = 0; i < source.size(); i++) {
				if (function_decl::isValidSyntax(source[i])) {
					start = i;
					break;
				}
			}
			std::vector<expression> code =
				std::vector<expression>(source.begin() + start, source.end());
			return start + block::getBlockBound(code);
		}

	}; // function_decl

	struct assignment : statemate {

		std::string left;
		child_ptr right;

		assignment(parent_ptr parent = nullptr)
			: statemate(parent), left(), right(nullptr) {}
		assignment(parent_ptr parent, const std::vector<expression>& source) {
			if (source.size() < 2) {
				throw std::runtime_error("assignment::invalid syntax: expected header and body");
			}

			const expression& header = source[0];
			const expression& body = source[1];

			if (header.size() != 2 ||
				header[0].getType() != TokenType::IDENTIFIER ||
				header[1].getValue() != EQUALS) {
				throw std::runtime_error("assignment::invalid header syntax");
			}

			this->root = parent;
			this->left = header[0].getValue();

			if (binexpr::isValidSyntax(body)) {
				this->right = std::make_unique<binexpr>(this, std::vector<expression>{ body });
			}
			else {
				throw std::runtime_error("assignment::invalid body expression");
			}
		};
		virtual ~assignment() = default;

		virtual bool isNil() const override {
			return true;
		}

		virtual const char* name() const override {
			return "assignment";
		}

		virtual const boost::json::object& toJson() const override {
			this->obj.clear();
			this->obj["type"] = this->name();
			this->obj["left"] = this->left;
			this->obj["right"] = this->right ? this->right->toJson() : boost::json::object{};
			return this->obj;
		}

		static bool isValidSyntax(const expression& expr) {
			if (expr.size() == 2) {
				return expr[0].getType() == TokenType::IDENTIFIER &&
					expr[1].getValue() == EQUALS;
			}
			return false;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			for (int i = 0; i + 1 < static_cast<int>(source.size()); ++i) {
				const expression& header = source[i];

				if (isValidSyntax(header)) {
					return i + 1;
				}
			}
			throw std::runtime_error("assignment::header not found");
		}

	};	//	assignment

	struct condition : statemate {

		std::string cond_var;
		child_ptr body;

		condition(parent_ptr parent = nullptr)
			: statemate(parent), cond_var(), body(nullptr) {}
		condition(parent_ptr parent, const std::vector<expression>& source) : statemate(parent) {
			if (source.size() < 2) {
				throw std::runtime_error("condition::not enough lines for if statement");
			}

			const expression& header = source[0];
			if (header.size() != 2 ||
				header[0].getType() != TokenType::KEYWORD ||
				header[0].getValue() != IF ||
				header[1].getType() != TokenType::IDENTIFIER) {
				throw std::runtime_error("condition::invalid if header syntax");
			}

			this->cond_var = header[1].getValue();

			const expression& body_expr = source[1];
			if (!block::isValidSyntax(body_expr)) {
				throw std::runtime_error("condition::expected block after if header");
			}

			std::vector<expression> code =
				std::vector<expression>(source.begin() + 1, source.end());
			this->body = std::make_unique<block>(this, code);
		};
		virtual ~condition() = default;

		virtual bool isNil() const override {
			return false;
		}

		virtual const char* name() const override {
			return "condition";
		}

		virtual const boost::json::object& toJson() const override {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["condition"] = this->cond_var;
			this->obj["body"] = this->body ? this->body->toJson() : boost::json::object{};

			return this->obj;
		}

		static bool isValidSyntax(const expression& expr) {
			return expr.size() == 2 &&
				expr[0].getType() == TokenType::KEYWORD &&
				expr[0].getValue() == IF &&
				expr[1].getType() == TokenType::IDENTIFIER;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int start = 0;
			for (int i = 0; i < source.size(); i++) {
				if (condition::isValidSyntax(source[i])) {
					start = i;
					break;
				}
			}
			std::vector<expression> code =
				std::vector<expression>(source.begin() + start, source.end());
			return start + block::getBlockBound(code);
		}

	};	// condition

	struct cycle : statemate {

		std::string cond_var;
		child_ptr body;

		cycle(parent_ptr parent = nullptr)
			: statemate(parent), cond_var(), body(nullptr) {}

		cycle(parent_ptr parent, const std::vector<expression>& source)
			: statemate(parent) {
			if (source.size() < 2) {
				throw std::runtime_error("cycle::not enough lines for for statement");
			}

			const expression& header = source[0];
			if (header.size() != 2 ||
				header[0].getType() != TokenType::KEYWORD ||
				header[0].getValue() != FOR ||
				header[1].getType() != TokenType::IDENTIFIER) {
				throw std::runtime_error("cycle::invalid for header syntax");
			}

			this->cond_var = header[1].getValue();

			const expression& body_expr = source[1];
			if (!block::isValidSyntax(body_expr)) {
				throw std::runtime_error("cycle::expected block after for header");
			}

			std::vector<expression> code =
				std::vector<expression>(source.begin() + 1, source.end());
			this->body = std::make_unique<block>(this, code);
		};

		virtual ~cycle() = default;

		virtual bool isNil() const override {
			return false;
		}

		virtual const char* name() const override {
			return "cycle";
		}

		virtual const boost::json::object& toJson() const override {
			this->obj.clear();

			this->obj["type"] = this->name();
			this->obj["condition"] = this->cond_var;
			this->obj["body"] = this->body ? this->body->toJson() : boost::json::object{};

			return this->obj;
		}

		static bool isValidSyntax(const expression& expr) {
			return expr.size() == 2 &&
				expr[0].getType() == TokenType::KEYWORD &&
				expr[0].getValue() == FOR &&
				expr[1].getType() == TokenType::IDENTIFIER;
		}

		static int getStatemateBound(const std::vector<expression>& source) {
			int start = 0;
			for (int i = 0; i < source.size(); i++) {
				if (cycle::isValidSyntax(source[i])) {
					start = i;
					break;
				}
			}
			std::vector<expression> code =
				std::vector<expression>(source.begin() + start, source.end());
			return start + block::getBlockBound(code);
		}

	};	// cycle

	StatemateType getType(const expression& expr) {	// shit vvv / shit ^^^
		if (function_decl::isValidSyntax(expr))
			return StatemateType::Function_decl;
		if (identifier_decl::isValidSyntax(expr))
			return StatemateType::Identifier_decl;
		if (undeclaration::isValidSyntax(expr))
			return StatemateType::Undeclaration;
		if (condition::isValidSyntax(expr))
			return StatemateType::Condition;
		if (cycle::isValidSyntax(expr))
			return StatemateType::Cycle;
		if (system_call::isValidSyntax(expr))
			return StatemateType::System_call;
		if (block::isValidSyntax(expr))
			return StatemateType::Block;
		if (binexpr::isValidSyntax(expr))
			return StatemateType::Binexpr;
		if (assignment::isValidSyntax(expr))
			return StatemateType::Assignment;
		if (keyword::isValidSyntax(expr))
			return StatemateType::Keyword;
		throw std::runtime_error("invalid syntax : undefined statemate");
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
		case StatemateType::Function_decl:
			return std::unique_ptr<statemate>
				(new function_decl(parent, code));
		case StatemateType::Binexpr:
			return std::unique_ptr<statemate>
				(new binexpr(parent, code));
		case StatemateType::System_call:
			return std::unique_ptr<statemate>
				(new system_call(parent, code));
		case StatemateType::Block:
			return std::unique_ptr<statemate>
				(new block(parent, code));
		case StatemateType::Assignment:
			return std::unique_ptr<statemate>
				(new assignment(parent, code));
		case StatemateType::Condition:
			return std::unique_ptr<statemate>
				(new condition(parent, code));
		case StatemateType::Cycle:
			return std::unique_ptr<statemate>
				(new cycle(parent, code));
		default:
		{
			return nullptr;
		}
		}

	};	// make statemate

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
		case StatemateType::Function_decl:
			end += function_decl::getStatemateBound(code);
			break;
		case StatemateType::System_call:
			end += system_call::getStatemateBound(code);
			break;
		case StatemateType::Binexpr:
			end += binexpr::getStatemateBound(code);
			break;
		case StatemateType::Block:
			end += block::getStatemateBound(code);
			break;
		case StatemateType::Assignment:
			end += assignment::getStatemateBound(code);
			break;
		case StatemateType::Condition:
			end += condition::getStatemateBound(code);
			break;
		case StatemateType::Cycle:
			end += cycle::getStatemateBound(code);
			break;
		default:
			{
				std::cerr << "undefined statemate " << std::endl;
				return 0;
			}
		}

		return end;

	}	// get statemate bounds

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

	}	// build AST

} // namespace statemates

#endif	// STATEMATES_INL
