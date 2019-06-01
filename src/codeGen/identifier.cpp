//
// Created by zuhxs on 2019/6/1.
//

#include "utils/ast.hpp"

namespace yapc {
    llvm::Value *IdentifierAST::GetPtr(CodeGenUtils &context) {
        // first find in local context
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
        std::cout << "name: " + name << std::endl;
        auto *value = context.GetValue(name);
        if (!value) {
            value = context.GetModule().get()->getGlobalVariable(name);
        }
        if (!value) {
            throw CodegenException("Identifier not found");
        }
        return value;
    }
}