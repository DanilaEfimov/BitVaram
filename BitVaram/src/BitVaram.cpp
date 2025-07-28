#include <BitVaram.h>

using namespace varam;
using namespace compiler;

int main(int argc, char* argv[]) {

    const char* fake_argv[] = { "sd", "-I", "source.bv" };
    int fake_argc = 3;

    Config config(fake_argc, const_cast<char**>(fake_argv));

    Preprocessor preprocessor;
    preprocessor.process(config);
    Context context = preprocessor.getContext();

    Lexer lexer;
    lexer.process(config);
    context = lexer.getContext();
    auto code = lexer.getExpressions();

    Parser parser;
    parser.process(config, code);

    return 0;
}
