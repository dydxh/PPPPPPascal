#ifndef yapc_driver
#define yapc_driver

#include "Scanner.hpp"
#include "Parser.hpp"
#include "context.hpp"

namespace yapc {

class Driver {
public:
    Context& context;
    Scanner* scanner;
    Parser* parser;
    explicit Driver(Context& contextval);
    ~Driver();
    bool Parse(const std::string& filename);

};

}
#endif