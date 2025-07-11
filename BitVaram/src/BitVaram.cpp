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

    for (auto& expression : code) {
        for (auto& token : expression) {
            std::cout << token.getValue() << " "
                << std::string(token.getPosition()) << std::endl;
        }
    }

    for (auto& error : context.getErrors()) {
        context.getLogger().log(Level::Warning, error.message, error.position);
    }

    return 0;
}
