cmake_minimum_required (VERSION 3.2)
project ("Streamlabs")

set (SHARED_SRC_FILES streamableobject.cpp fmt/format.cc)

add_executable (client client.cpp  ${SHARED_SRC_FILES})
add_executable (server server.cpp ${SHARED_SRC_FILES})

add_definitions ("-DASIO_STANDALONE=1")

if (MSVC)
    add_definitions ("/W4")
endif()