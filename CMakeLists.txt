set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")

project(yapc CXX)

cmake_minimum_required(VERSION 3.1)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(yapc_parser
             ${CMAKE_SOURCE_DIR}/src/pascal.y
             ${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp
             COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/Parser.hpp --verbose")
FLEX_TARGET(yapc_lexer
            ${CMAKE_SOURCE_DIR}/src/pascal.l
            ${CMAKE_CURRENT_BINARY_DIR}/Scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(yapc_lexer yapc_parser)

include_directories(${CMAKE_SOURCE_DIR}/src/ ${CMAKE_BINARY_DIR})

file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCE_FILES}

    ${FLEX_yapc_lexer_OUTPUTS}
    ${BISON_yapc_parser_OUTPUTS}
)
target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
    .
    ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET ${CMAKE_PROJECT_NAME}
    PROPERTY CXX_STANDARD 14)
