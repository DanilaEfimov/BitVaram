#include "config.h"

#include <cstdlib>
#include <iostream>

struct varam::Config::ConfigData {
    ConfigData() 
        : inputFile(), outputFile(), environmentFile(), verbose(false) {};
    virtual ~ConfigData() = default;

    std::string inputFile;
    std::string outputFile;
    std::string environmentFile;

    bool verbose;

    void init(const po::variables_map& vm) noexcept(false) {
        if (!vm.count("input")) {
            throw std::runtime_error("Missing required option: --input");
        }

        if (vm.count("input")) {
            this->inputFile = vm["input"].as<std::string>();
        }
        if (vm.count("out")) {
            this->outputFile = vm["out"].as<std::string>();
        }
        if (vm.count("env")) {
            this->environmentFile = vm["env"].as<std::string>();
        }
        this->verbose = vm.count("verbose");
    };

    bool isValid() const noexcept {
        return this->inputFile != "";
    }
};

varam::Config::Config(int argc, char* argv[])
	: desc("Allowed options")
{
    this->desc.add_options()
        ("help,h", "Print this message")
        ("input,I", po::value<std::string>(), "Input a .bv file")
        ("out,o", po::value<std::string>(), "Output name of .ir file")
        ("env,e", po::value<std::string>(), "Set core environment .bv file")
        ("verbose,v", po::bool_switch(), "Enable verbose output");

    this->parse(argc, argv);
}

varam::Config::~Config() {
    delete this->data;
}

void varam::Config::parse(int argc, char* argv[])
{
    try {
        po::parsed_options vmap = po::parse_command_line(argc, argv, this->desc);
        po::store(vmap, this->vm);
        po::notify(this->vm);

        if (this->vm.count("help")) {
            std::cout << this->desc << std::endl;
            std::exit(EXIT_SUCCESS);
        }

        this->data = new ConfigData();
        this->data->init(this->vm);

        if (!this->data->isValid()) {
            throw std::runtime_error("Invalid configuration of execution");
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

std::string varam::Config::getInputFile() const
{
	return this->data->inputFile;
}