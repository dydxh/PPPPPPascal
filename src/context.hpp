//
// Created by dydxh on 5/28/19.
//

#ifndef yapc_context_header
#define yapc_context_header

#include <memory>

#include "AST/basicast.hpp"

namespace yapc {
    class Context {
    public:
        std::unique_ptr<BasicAST> program;
    };
}

#endif //YAPC_CONTEXT_HPP
