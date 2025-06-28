#include <iostream>

#include "config.h"

using namespace varam;

int main(int argc, char* argv[]) {

    Config cfg(argc, argv);
    std::cout << cfg.getInputFile() << std::endl;
    std::cout << cfg.get<std::string>("out") << std::endl;

    return 0;
}
