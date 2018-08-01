cmake_minimum_required (VERSION 3.2)
project ("Streamlabs")

add_executable (client client.cpp)
add_executable (server server.cpp streamableobject.cpp)

if (MSVC)
    add_definitions ("/W4")
endif()