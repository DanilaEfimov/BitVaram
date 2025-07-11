#ifndef COMMON_HPP
#define COMMON_HPP

#include <map>
#include <string>

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
		undefinedIdentifer
	};

	using hint = string;
	using errorKey = pair<error, hint>;

	inline map<error, hint> messages = {
		errorKey(failedtoOpenFile, "failed to open file"),
		errorKey(missedCommentClosing, "missing closing multilines comment pattern"),
		errorKey(missedCommentOpening, "missing opening multilines comment pattern"),
		errorKey(undefinedIdentifer, "undefined identifier")
	};

};	// namespace varam

#endif	// COMMON_HPP