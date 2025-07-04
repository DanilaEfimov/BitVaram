#ifndef COMMON_HPP
#define COMMON_HPP

#include <map>
#include <string>

using namespace std;

namespace varam {

	enum error {
		// general
		failedtoOpenFile,

		// preprocessing
		missedCommentClosing,
		missedCommentOpening
	};

	using hint = string;
	using errorKey = pair<const error, const hint>;

	map<const error, const hint> messages = {
		errorKey(failedtoOpenFile, "failed to open file"),
		errorKey(missedCommentClosing, "missing closing multilines comment pattern"),
		errorKey(missedCommentOpening, "missing opening multilines comment pattern")
	};

};	// namespace varam

#endif	// COMMON_HPP