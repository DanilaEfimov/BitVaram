#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

namespace varam {

    namespace po = boost::program_options;

    class Config {
        struct ConfigData;
    public:
        Config(int argc, char* argv[]);
        virtual ~Config();

        std::string getInputFile() const;
        template<typename T>
        T get(const std::string& key) const;

    private:
        po::options_description desc;
        po::variables_map vm;

        ConfigData* data;

        void parse(int argc, char* argv[]);
    };

} // namespace varam

#include <config.inl>

#endif // CONFIG_H
