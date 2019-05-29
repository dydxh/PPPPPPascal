//
// Created by zuhxs on 2019/5/29.
//

#include "../AST/expr.hpp"
#include "codeGen_utils.hpp"



namespace yapc {
    llvm::Value *BinaryExprAST::codegen(CodeGenUtils &context) {
            auto *lhs = this->lhs->codegen(context);
    }



}