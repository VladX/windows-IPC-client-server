#pragma once

#include "cxxopts.hpp"
#include <cstdlib>

namespace Streamlabs {

class ArgParser {
private:
    ArgParser() {}

public:
    std::string path;

    static ArgParser Parse(int argc, char** argv, const char* name, const char* description) {
        ArgParser parser;
        try {
            cxxopts::Options argparser(name, description);
            argparser.show_positional_help();
            argparser.add_options()
                ("p,pipe", "Pipe path", cxxopts::value<std::string>()->default_value("\\\\.\\pipe\\Streamlabs"))
                ("help", "Show help");
            auto arguments = argparser.parse(argc, argv);
            if (arguments.count("help")) {
                std::cout << argparser.help() << std::endl;
                exit(0);
            }
            //if (arguments.count("pipe") == 0)
            //    throw cxxopts::option_required_exception("pipe");
            parser.path = arguments["pipe"].as<std::string>();
        } catch (const cxxopts::OptionException& e) {
            std::cerr << "Error while parsing arguments: " << e.what() << std::endl;
            exit(1);
        }
        return parser;
    }
};

}