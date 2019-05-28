#include <iostream>
#include <cstdio>
#include <string>
#include <utility>
#include "Driver.hpp"

int main(const int argc, const char** argv) {
    if(argc < 2) {
        std::cout << "[Usage]: ./[elf] [source]" << std::endl;
        return 0;
    }
    
    yapc::Driver* driver;
    yapc::Context context;
    try {
        driver = new yapc::Driver(context);
    }
    catch(std::bad_alloc& msg) {
        std::cerr << "Failed to allocate driver: (" << msg.what() << "), exit." << std::endl;
        return false;
    }
    driver->Parse(argv[1]);
    return 0;
}