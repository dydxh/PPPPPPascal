//
// Created by zuhxs on 2019/5/30.
//

#include "utils/ast.hpp"

namespace yapc {
    genValue ProgramAST::codegen(CodeGenUtils &context) {
        printf("eee");
        progblock->constpart->codegen(context);
    }
}