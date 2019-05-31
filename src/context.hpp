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
        std::shared_ptr<ProgramAST> program;
    };
}

#endif //YAPC_CONTEXT_HPP
