cmake_minimum_required(VERSION 3.16)
project(EncryptedP2PChat VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Source files - Credentials modules
set(CREDENTIALS_SOURCES
    src/credentials/sdjwt.cpp
)

# Source files - Privacy modules
set(PRIVACY_SOURCES
    src/privacy/pir.cpp
    src/privacy/differential_privacy.cpp
    src/privacy/metadata_protection.cpp
)

# Source files - Enclave modules
set(ENCLAVE_SOURCES
    src/enclave/secure_enclave.cpp
    src/enclave/pq3_protocol.cpp
)

# Include directories
include_directories(include)

# Executable
add_executable(p2p_chat 
    src/main.cpp
    ${CREDENTIALS_SOURCES}
    ${PRIVACY_SOURCES}
    ${ENCLAVE_SOURCES}
)

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(p2p_chat PRIVATE Threads::Threads)
endif()

if(WIN32)
    target_compile_options(p2p_chat PRIVATE /W4 /WX)
else()
    target_compile_options(p2p_chat PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS p2p_chat DESTINATION bin)
