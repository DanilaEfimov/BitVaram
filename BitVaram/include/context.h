#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <vector>

namespace compiler {

    // Compilation phases, can be extended
    enum class Phase {
        Preprocessing,
        Parsing,
        SemanticAnalysis,
        CodeGeneration,
        Optimization,
        Finished
    };

    // Compilation status
    enum class Status {
        Success,
        Started,
        ErrorOccurred,
        Aborted,
        Finished
    };

    // Position in source code: filename, line, and column
    struct Position {
        std::string filename;
        int line;
        int column;

        Position() : filename(), line(-1), column(-1) {};
        Position(const std::string& file, int l, int c)
            : filename(file), line(l), column(c) {};
    };

    // Error detail with message and position
    struct ErrorDetail {
        std::string message;
        Position position;

        ErrorDetail() : message(), position() {};
        ErrorDetail(const std::string& msg, const Position& pos) 
            : message(msg), position(pos){};
    };

    // Simple logger with severity levels
    class Logger {
    public:
        enum class Level {
            Info,
            Warning,
            Error
        };

        void log(Level level, const std::string& message, const Position& pos = Position());
    };

    // Main compilation context storing state, errors, source, and logger
    class Context {
    public:
        Context() : currentPhase(), currentStatus(), errors(), logger(), sourceCode() {};
        ~Context() = default;

        Phase getPhase() const;
        void setPhase(Phase phase);

        Status getStatus() const;
        void setStatus(Status status);

        void addError(const std::string& message, const Position& pos);
        const std::vector<ErrorDetail>& getErrors() const;

        Logger& getLogger();

        void setSourceCode(const std::string& code);
        const std::string& getSourceCode() const;

    private:
        Phase currentPhase;
        Status currentStatus;

        std::vector<ErrorDetail> errors;
        Logger logger;

        std::string sourceCode;
    };

} // namespace compiler

#endif // CONTEXT_H
