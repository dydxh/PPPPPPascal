#ifndef yapc_driver
#define yapc_driver

#include "Scanner.hpp"
#include "Parser.hpp"
#include "context.hpp"

namespace yapc {

class Driver {
public:
    Context& context;
    explicit Driver(Context& context);
    ~Driver();
    bool Parse(const std::string& filename);

private:
    Scanner* scanner;
    Parser* parser;
};

}
#endif