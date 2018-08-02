#include "client.hpp"

using Streamlabs::ArgParser;
using Streamlabs::Client;

int main(int argc, char** argv) {
    ArgParser parser = ArgParser::Parse(argc, argv, "client", "This is the client part which can communicate with the server");
    Client client(parser.path);
    std::cout << "Opening pipe..." << std::endl;
    try {
        client.Connect();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}