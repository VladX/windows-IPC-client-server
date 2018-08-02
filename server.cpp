#include "server.hpp"

using Streamlabs::ArgParser;
using Streamlabs::Server;

void Interaction(Server &server) {
    for (;;) {
        Streamlabs::EventHeader header;
        auto data = server.WaitEvent(header);
        fmt::print("Got event: {} {}\n", header.type, header.length);
    }
}

int main(int argc, char** argv) {
    ArgParser parser = ArgParser::Parse(argc, argv, "server", "This is the server part which is storing all the objects");
    Server server(parser.path);
    fmt::print_colored(fmt::yellow, "Creating pipe '{}' and waiting for the client to connect (use --pipe command line argument to use a different name)...\n", parser.path);
    try {
        server.Connect();
    } catch (const std::exception& e) {
        fmt::print_colored(fmt::red, "{}", e.what());
        return 1;
    }
    Interaction(server);

    return 0;
}