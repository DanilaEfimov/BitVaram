#ifndef COMMON_HPP
#define COMMON_HPP

#include <map>
#include <string>

#define COREDEF		"coredef"		// coredef [function definition]
#define COREUNDEF	"coreundef"		// coreundef [function identifier]
#define CURDEF		"curdef"		// curdef [function definition] / [variable definition] [=] [number]
#define CURUNDEF	"curundef"		// curundef [functtion identifier] / [variable identifier] [=] [number]
#define DEFINED		"defined"		// defined [identifier]
#define IF			"if"			// if [variable identifier]
#define FOR			"for"			// for [variable identifier]
#define STORE		"store"			// store [identifier]
#define SHOW		"show"			// show {prints all from registry}
#define AND			"and"			// [identifier] and [identifier]
#define OR			"or"			// [identifier] or [identifier]
#define XOR			"xor"			// [identifier] xor [identifier]
#define SUP			"sup"			// sup [(] {separated identifiers} [)]
#define INF			"inf"			// inf [(] {separated identifiers} [)]
#define RETURN		"return"		// return [identifier]
#define EXIT		"exit"			// exit [number]

#define FUNC_OPEN_BRACKET				"("		// function call syntax :: [name] [(] [)] 
#define FUNC_CLOSE_BRACKET				")"		// function call syntax :: [name] [(] [)]
#define BLOCK_OPEN_BRACKET				"["		// body-based statemates synntax [header] [[] [body] []]
#define BLOCK_CLOSE_BRACKET				"]"		// body-based statemates synntax [header] [[] [body] []]
#define MULTY_ARG_FUNC_OPEN_BRACKET		"{"		// BitVaram standart functions sup{...} & inf {...}
#define MULTY_ARG_FUNC_CLOSE_BRACKET	"}"		// BitVaram standart functions sup{...} & inf {...}
#define EQUALS							"="		// arithmetic syntax id = expr
#define PLUS							"+"		// arithmetic syntax id + num
#define MINUS							"-"		// arithmetic syntax id - num
#define MUL								"*"		// arithmetic syntax id * num
#define DIV								"/"		// arithmetic syntax id / num
#define NOT								"~"		// boolean arithmetic syntax ~id
#define LESS							"<"		// comparing syntax id1 < id2 = 1 or 0

#define COMMA							","		// arguments separation syntax (id1 , id2 , id3 ...)

using namespace std;

namespace varam {

	enum error {
		// general
		failedtoOpenFile,
		failedtoReadAll,

		// preprocessing
		missedCommentClosing,
		missedCommentOpening,

		// parser
		missedBlockClosing,
		missedBlockOpening,
		invalidSyntax
	};

	using hint = string;
	using errorKey = pair<error, hint>;

	inline map<error, hint> messages = {
		errorKey(failedtoOpenFile, "failed to open file"),
		errorKey(missedCommentClosing, "missing closing multilines comment pattern"),
		errorKey(missedCommentOpening, "missing opening multilines comment pattern"),
		errorKey(missedBlockClosing, "missing closing operator pattern"),
		errorKey(missedBlockOpening, "missing opening operator pattern"),
		errorKey(invalidSyntax, "invalid syntax, undefined statemate")
	};

};	// namespace varam

#endif	// COMMON_HPP