#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        std::string filename;
        bool verbose = false;

        // Define available options
        po::options_description desc("Available options");
        desc.add_options()
            ("help,h", "Show help message")
            ("file,f", po::value<std::string>(&filename), "Specify filename")
            ("verbose,v", po::bool_switch(&verbose), "Enable verbose output");

        // Parse command-line arguments
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        // Handle options
        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("file")) {
            std::cout << "File: " << filename << "\n";
        }
        else {
            std::cout << "No file specified.\n";
        }

        if (verbose) {
            std::cout << "Verbose mode enabled.\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
