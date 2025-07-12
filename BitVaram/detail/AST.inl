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

			this->obj[TYPE] = "operator";
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

}

#endif
