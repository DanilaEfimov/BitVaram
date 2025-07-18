#ifndef STATEMATES_AST
#define STATEMATES_AST

#define TYPE "type"

#include <string>
#include <vector>
#include <set>
#include <boost/json.hpp>

// Statemates

using namespace boost::json;

namespace compiler::statemates {	// CXX17+ relational namespace declaration

	struct statemate {
		virtual const object& toJson() const = 0;
		virtual ~statemate() = default;
	};

	struct StBlock : statemate {
		bool isinternal = false;	// for scope including
		boost::json::array operators;		// array of json obj-s
		boost::json::array localSymbols;

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "block";
			this->obj["is_internal"] = this->isinternal;
			this->obj["operators"] = this->operators;
			this->obj["symbols"] = this->localSymbols;

			return this->obj;
		}
	};

	struct StConditionBlock : statemate {
		boost::json::array expression; // array of tokens
		StBlock trueBranch = {};
		StBlock falseBranch = {};

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "condition";
			this->obj["expression"] = this->expression;
			this->obj["then"] = this->trueBranch.toJson();
			this->obj["else"] = this->falseBranch.toJson();

			return this->obj;
		}
	};

	struct StFunctionDeclaration : statemate {
		StBlock body;
		std::string name;
		boost::json::array parameters;
		bool isglobal = false;

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "funcdefinition";
			this->obj["body"] = this->body.toJson();
			this->obj["name"] = this->name;
			this->obj["parameters"] = this->parameters;
			this->obj["is_global"] = this->isglobal;

			return this->obj;
		}
	};

	struct StVariableDeclaration : statemate {
		std::string identifier;
		boost::json::array expression; // array of tokens

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "vardefinition";
			this->obj["identifier"] = this->identifier;
			this->obj["expression"] = this->expression;

			return this->obj;
		}
	};

	struct StCycle : statemate {
		std::string parameterIdentifier;
		StBlock body;

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "cycle";
			this->obj["identifier"] = this->parameterIdentifier;
			this->obj["expression"] = this->body.toJson();

			return this->obj;
		}
	};

	struct StUndeclaration : statemate {
		std::string identifier;

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "undeclaration";
			this->obj["identifier"] = this->identifier;

			return this->obj;
		}
	};

	struct StAssignment : statemate {
		std::string identifier;
		boost::json::array expression;	// array of tokens

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "assignment";
			this->obj["identifier"] = this->identifier;
			this->obj["expression"] = this->expression;

			return this->obj;
		}
	};

	struct StUnarExpression : statemate {
		std::string op;							// "~", ""
		std::string identifier;

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "unarop";
			this->obj["operator"] = this->op;
			this->obj["identifier"] = this->identifier;

			return this->obj;
		}
	};

	struct StBinaryExpression : statemate {
		std::string op;							// "+", "-", "*", "/"
		std::string left;						// lvalue identifier
		std::string right;						// rvalue identifier

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "binexpr";
			this->obj["operator"] = this->op;
			this->obj["left"] = this->left;
			this->obj["right"] = this->right;

			return this->obj;
		}
	};

	struct StFunctionCall : statemate {
		std::string name;
		boost::json::array arguments;	// array of tokens

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "funccall";
			this->obj["name"] = this->name;
			this->obj["arguments"] = this->arguments;

			return this->obj;
		}
	};

	struct StReturn : statemate {
		std::string identifier;

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "return";
			this->obj["return_value"] = this->identifier;

			return this->obj;
		}
	};

	struct StSystemCall : statemate {
		std::string name;
		boost::json::array arguments;	// array of tokens

	private:
		mutable object obj;

	public:
		virtual const object& toJson() const override {
			this->obj.clear();

			this->obj[TYPE] = "syscall";
			this->obj["name"] = this->name;
			this->obj["arguments"] = this->arguments;

			return this->obj;
		}
	};

	struct ASTnode {
		ASTnode* parent = nullptr;
		std::vector<ASTnode*> childs = {};

		statemate* statemate;

		void append(ASTnode* child) {
			this->childs.push_back(child);
		}

		explicit ASTnode(ASTnode* parent)
			: parent(parent), childs(), statemate(nullptr) {};

		virtual ~ASTnode() {
			if (this->parent) {
				delete this->parent;
				for (auto child : this->childs) {
					delete child;
				}
			}
		}
	};

}

#endif
