#include <fstream>

#include "Driver.hpp"

yapc::Driver::Driver(yapc::Context &context) : context(context),scanner(nullptr),parser(nullptr) {}

yapc::Driver::~Driver() {
    delete(scanner);
    scanner = nullptr;
    delete(parser);
    parser = nullptr;
}

bool yapc::Driver::Parse(const std::string& filename) {
    std::ifstream fin(filename);
    if(!fin.good()) {
        std::cout << "Cann't open file" << std::endl;
        return false;
    }
    delete(scanner);
    try {
        scanner = new yapc::Scanner(&fin);
    }
    catch(std::bad_alloc& msg) {
        std::cerr << "Failed to allocate scanner: (" << msg.what() << "), exit." << std::endl;
        return false;
    }

    delete parser;
    try {
        parser = new yapc::Parser(*scanner, *this);
    }
    catch(std::bad_alloc& msg) {
        std::cerr << "Failed to allocate parser: (" << msg.what() << "), exit." << std::endl;
        return false;
    }

    const int accept = 0;
    if(parser->parse() != accept) {
        std::cerr << "Parse failed." << std::endl;
        return false;
    }
    std::cout << "\n\n>>>>>>>>>>>>>>>==========parse over!==========<<<<<<<<<<<<<<<" << std::endl;
    return true;
}