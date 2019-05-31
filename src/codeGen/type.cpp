//
// Created by zuhxs on 2019/5/30.
//

#include "utils/ast.hpp"
#include "codeGen_utils.hpp"

namespace yapc {
    genValue BooleanAST::codegen(CodeGenUtils &context) {
        return val ? context.GetBuilder().getTrue() : context.GetBuilder().getFalse();
    }

    genValue IntegerAST::codegen(CodeGenUtils &context) {
        auto *type = context.GetBuilder().getInt32Ty();
        return llvm::ConstantInt::getSigned(type, val);
    }

    genValue RealAST::codegen(CodeGenUtils &context) {
        auto *type = context.GetBuilder().getDoubleTy();
        return llvm::ConstantFP::get(type, val);
    }

    genValue StringAST::codegen(CodeGenUtils &context) {
        return context.GetBuilder().CreateGlobalStringPtr(val);
    }

}