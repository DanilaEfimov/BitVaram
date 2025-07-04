#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "context.h"
#include "config.h"

#include <string>

namespace compiler {

	inline constexpr const char* precompiledPrefix = ".i";
	inline constexpr const char* singleCommentPattern = "::";
	inline constexpr const char* multiCommentOpeningPattern = ":;";
	inline constexpr const char* multiCommentClosingPattern = ";:";

	class Preprocessor {
		enum class State {
			pass,
			singleCommentOccured,
			multistringCommentOpened,
			waitingCommentClosing,
			multistringCommentClosed,
			finished
			// conditional blocks
			// define directives and so on
		};
	private:
		State state;
		Position pos;
		Context context;

		void solveSingleComment(std::string& line);
		void solveMultiCommentOpening(std::string& line);
		void solveMultiCommentClosing(std::string& line);

		// Check state accessors
		bool isCorrectSwitch(State last, State next);
		bool singleCommentinLine(const std::string& line) const;
		bool multistringCommentOpeninginLine(const std::string& line) const;
		bool multistringCommentClosinginLine(const std::string& line) const;

		State checkState(const std::string& line);
		void occureErrorStateSwitch(State last, State next, const std::string& line);
		void occureFinishState();

	public:
		Preprocessor() : context(), state(State::pass), pos() {};
		~Preprocessor() = default;

		static std::string replacedPrefixName(const std::string& filename);

		void process(const varam::Config& config);

		const Context& getContext() const;
	};

}	// namespace compiler

#endif	// PREPROCESSOR_H