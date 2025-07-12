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

    return 0;
}
