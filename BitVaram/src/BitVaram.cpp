#include <BitVaram.h>

using namespace varam;
using namespace compiler;

int main(int argc, char* argv[]) {

    Config config(argc, argv);

    Preprocessor preprocessor;
    preprocessor.process(config);
    Context context = preprocessor.getContext();

    for (auto& error : context.getErrors()) {
        context.getLogger().log(Level::Warning, error.message, error.position);
    }

    Lexer lexer;
    lexer.process(config);
    context = lexer.getContext();
    auto code = lexer.getExpressions();

    for (auto& error : context.getErrors()) {
        context.getLogger().log(Level::Warning, error.message, error.position);
    }

    Parser parser;
    parser.process(code, config);
    context = parser.getContext();

    bool fatal = false;
    for (auto& error : context.getErrors()) {
        context.getLogger().log(Level::Error, error.message, error.position);
        fatal = true;
    }

    if (fatal) {
        std::exit(EXIT_FAILURE);    // system calling
    }

    return 0;
}
