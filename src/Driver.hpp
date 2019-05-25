#ifndef yapc_driver
#define yapc_driver

#include "Scanner.hpp"
#include "Parser.hpp"

namespace yapc {

class Driver {
public:
    Driver() = default;
    ~Driver();
    bool Parse(const std::string& filename);

private:
    Scanner* scanner;
    Parser* parser;
};

}
#endif