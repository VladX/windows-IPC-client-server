#include "server.hpp"

using Streamlabs::ArgParser;
using Streamlabs::Server;
using Streamlabs::StreamableObjectType;

void Interaction(Server &server) {
    for (;;) {
        Streamlabs::EventHeader header;
        try {
            auto data = server.WaitEvent(header);
            switch (header.type) {
                case Streamlabs::SEND_OBJECT:
                {
                    const uint8_t* ptr = &data[0];
                    StreamableObjectType type = *reinterpret_cast<const StreamableObjectType*>(ptr);
                    auto &object = server.AddObject(type);
                    object.Deserialize(ptr + sizeof(type));
                    std::cout << "Added the new object: " << object << std::endl;
                    break;
                }
                case Streamlabs::CALL_METHOD:
                {
                    const uint8_t* ptr = &data[0];
                    const uint64_t index = *reinterpret_cast<const uint64_t*>(ptr);
                    auto method = Streamlabs::StreamableObject::Create(Streamlabs::STRING);
                    method->Deserialize(ptr + sizeof(index));
                    std::stringstream methodstr;
                    methodstr << (*method);
                    try {
                        server.CallMethod(index, methodstr.str());
                    } catch (const std::out_of_range &e) {
                        fmt::print_colored(fmt::red, "Cant call the given method {}\n", methodstr.str());
                        break;
                    }
                    fmt::print_colored(fmt::magenta, "Calling the method {} on an object with index {}\n", methodstr.str(), index);
                    break;
                }
                case Streamlabs::INSPECT_OBJECT:
                {
                    const uint64_t index = *reinterpret_cast<const uint64_t*>(&data[0]);
                    fmt::print_colored(fmt::cyan, "Requested an object with index {}\n", index);
                    try {
                        server.SendObject(index);
                    } catch (const std::out_of_range &e) {
                        fmt::print_colored(fmt::red, "Unable to get an object with index {}\n", index);
                        server.SendObject(Streamlabs::StreamableObject::Create(Streamlabs::STRING));
                    }
                    break;
                }
                default:
                    throw std::logic_error("Unknown event type");
            }
            server.PrintSummary();
        } catch (const std::exception& e) {
            fmt::print_colored(fmt::red, "{}\n", e.what());
            break;
        }
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