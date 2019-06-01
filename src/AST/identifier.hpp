#ifndef yapc_ast_identifer
#define yapc_ast_identifer

#include <algorithm>
#include "basicast.hpp"

namespace yapc {
    class IdentifierAST : public ExprAST {
    public:
        IdentifierAST(const std::string& IdentifierName) : name(IdentifierName){
            std::transform(IdentifierName.begin(), IdentifierName.end(), name.begin(), ::tolower);
        }
        ~IdentifierAST() = default;

        genValue codegen(genContext context) override {}
        std::string &GetName() {std::cout<<"hello--" << name <<std::endl;return name;}

    protected:
        std::string name;
    };
}

#endif