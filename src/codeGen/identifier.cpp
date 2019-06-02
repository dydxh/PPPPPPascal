//
// Created by zuhxs on 2019/6/1.
//

#include "utils/ast.hpp"

namespace yapc {
    llvm::Value *IdentifierAST::GetPtr(CodeGenUtils &context) {
        // first find in local context
        std::cout << "finding " << name << std::endl;
        auto *value = context.GetValue(name);
        if (!value) {
            value = context.GetModule().get()->getGlobalVariable(name);
        }
        if (!value) {
            throw CodegenException("Identifier not found");
        }
        return value;
    }

    genValue IdentifierAST::codegen(CodeGenUtils &context) {
        return context.GetBuilder().CreateLoad(GetPtr(context));
    }
}