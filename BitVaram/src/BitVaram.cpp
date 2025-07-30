#include <BitVaram.h>

using namespace varam;
using namespace compiler;

int main(int argc, char* argv[]) {

    Config config(argc, argv);

    Preprocessor preprocessor;
    preprocessor.process(config);
    Context context = preprocessor.getContext();

    Lexer lexer;
    lexer.process(config);
    auto code = lexer.getExpressions();

    Parser parser;
    parser.process(config, code);

    return 0;
}
