cmake_minimum_required(VERSION 3.16)
project(EncryptedP2PChat VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(p2p_chat src/main.cpp)

if(UNIX)
    target_link_libraries(p2p_chat pthread)
endif()
