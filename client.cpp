#include "client.hpp"

using Streamlabs::ArgParser;
using Streamlabs::Client;

void Interaction(Client &client) {
    for (;;) {
        client.SendEvent({Streamlabs::SEND_OBJECT, 2}, "ok");
        Sleep(500);
    }
}

int main(int argc, char** argv) {
    ArgParser parser = ArgParser::Parse(argc, argv, "client", "This is the client part which can communicate with the server");
    Client client(parser.path);
    fmt::print_colored(fmt::yellow, "Waiting for the server and opening pipe '{}' (use --pipe command line argument to use a different name)...\n", parser.path);
    try {
        client.Connect();
    } catch (const std::exception& e) {
        fmt::print_colored(fmt::red, "{}", e.what());
        return 1;
    }
    Interaction(client);

    return 0;
}