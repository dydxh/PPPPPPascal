set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

project(yapc)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Scanner.cpp" AND
        EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp" AND
        EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Parser.hpp")
        message(STATUS "Found Scanner and Parser exists")
        set(FLEX_yapc_lexer_OUTPUTS "${CMAKE_CURRENT_BINARY_DIR}/Scanner.cpp")
        set(BISON_yapc_parser_OUTPUTS "${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp;${CMAKE_CURRENT_BINARY_DIR}/Parser.hpp" src/codeGen/basiccast.cpp src/codeGen/statement.cpp src/codeGen/identifier.cpp)
else()
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
endif()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/src/ ${CMAKE_BINARY_DIR} ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
#add_executable(simple-tool tool.cpp)


file(GLOB SOURCE_FILES
        "src/*.cpp"
        "src/*.hpp"
        "src/AST/*.hpp"
        "src/codeGen/*.hpp"
        "src/codeGen/*.cpp"
        "src/utils/*.hpp"
        "src/utils/*.cpp"
        )

add_executable(
        ${CMAKE_PROJECT_NAME}
        ${SOURCE_FILES}

        ${FLEX_yapc_lexer_OUTPUTS}
        ${BISON_yapc_parser_OUTPUTS}
        src/codeGen/frame.cpp src/codeGen/decleration.cpp src/utils/ast.hpp src/codeGen/type.cpp)
#        src/codeGen/expr.cpp src/codeGen/codeGen_utils.hpp)
target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE
        .
        ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET ${CMAKE_PROJECT_NAME}
        PROPERTY CXX_STANDARD 14)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(yapc ${llvm_libs})