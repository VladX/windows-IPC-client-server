#include "client.hpp"

using Streamlabs::ArgParser;
using Streamlabs::Client;

void PrintHelp() {
    std::cout << R"HELP(Use the following commands
   
    h                 - Print this help message
    t                 - Print supported object types
    q                 - Quit
    create TYPE       - Create an empty object on server with the given
                        type
    send TYPE OBJECT  - Send the given object to the server;
                        Examples:
                          send integer 7 (sends 7)
                          send string abcde (sends "abcde")
                          send float 3.14 (sends 3.14)
                          send vector 3 1 1 1 (sends vector {1, 1, 1})
    call INDEX METHOD - Call a method with the given name on object with
                        the given index on server
                        Examples:
                          create vector
                          call 0 PushBackRandom
    m TYPE            - List possible methods to call
    i INDEX           - Inspect an object with the given index on server
)HELP" << std::endl;
}

void Interaction(Client &client) {
    PrintHelp();
    for (;;) {
        fmt::print_colored(fmt::white, "> ");
        std::string command;
        std::cin >> command;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        try {
            if (command == "h") {
                PrintHelp();
                continue;
            }
            if (command == "t") {
                for (const auto &name : Streamlabs::kObjectNames)
                    std::cout << name << ' ';
                std::cout << std::endl;
                continue;
            }
            if (command == "q") {
                break;
            }
            if (command == "m") {
                std::string type_name;
                std::cin >> type_name;
                std::transform(type_name.begin(), type_name.end(), type_name.begin(), ::tolower);
                auto object = Streamlabs::StreamableObject::Create(type_name);
                if (object->GetSchema().GetMethodNames().empty())
                    fmt::print_colored(fmt::yellow, "No methods registred on this object type");
                for (const auto &name : object->GetSchema().GetMethodNames())
                    fmt::print_colored(fmt::white, "{} ", name);
                std::cout << std::endl;
                continue;
            }
            if (command == "create") {
                std::string type_name;
                std::cin >> type_name;
                std::transform(type_name.begin(), type_name.end(), type_name.begin(), ::tolower);
                client.CreateObject(type_name);
                continue;
            }
            if (command == "send") {
                std::string type_name;
                std::cin >> type_name;
                std::transform(type_name.begin(), type_name.end(), type_name.begin(), ::tolower);
                auto object = Streamlabs::StreamableObject::Create(type_name);
                std::cin >> (*object);
                client.SendObject(object);
                continue;
            }
            if (command == "call") {
                uint64_t index;
                std::cin >> index;
                auto method = Streamlabs::StreamableObject::Create(Streamlabs::STRING);
                std::cin >> (*method);
                client.CallMethod(index, method);
                continue;
            }
            if (command == "i") {
                uint64_t index;
                std::cin >> index;
                std::cout << (*client.InspectObject(index)) << std::endl;
                continue;
            }
            fmt::print_colored(fmt::red, "Unknown command\n");
        } catch (const std::exception& e) {
            fmt::print_colored(fmt::red, "{}\n", e.what());
            continue;
        }
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