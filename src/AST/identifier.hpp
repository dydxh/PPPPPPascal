#ifndef yapc_ast_identifer
#define yapc_ast_identifer

#include <algorithm>
#include "basicast.hpp"

namespace yapc {
    class IdentifierAST : public ExprAST {
    public:
        IdentifierAST(const std::string& IdentifierName) {
            std::transform(IdentifierName.begin(), IdentifierName.end(), name.begin(), ::tolower);
        }
        ~IdentifierAST() = default;

        genValue codegen(genContext context) override {}

    protected:
        std::string name;
    };
}

#endif