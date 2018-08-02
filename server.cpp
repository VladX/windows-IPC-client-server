#include "server.hpp"

using Streamlabs::ArgParser;
using Streamlabs::Server;

int main(int argc, char** argv) {
    ArgParser parser = ArgParser::Parse(argc, argv, "server", "This is the server part which is storing all the objects");
    Server server(parser.path);
    std::cout << "Creating pipe and waiting for client to connect..." << std::endl;
    try {
        server.Connect();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}