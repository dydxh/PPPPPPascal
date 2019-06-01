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

<<<<<<< HEAD
        genValue codegen(genContext context) override;
        llvm::Value *GetPtr(CodeGenUtils &context);
        std::string &GetName() {return name;}
=======
        genValue codegen(genContext context) override {}
        std::string &GetName() {std::cout<<"hello--" << name <<std::endl;return name;}
>>>>>>> b2855db0bb9c4949fd76a9cba81a6ca458f05d6b

    protected:
        std::string name;
    };
}

#endif