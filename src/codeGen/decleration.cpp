//
// Created by zuhxs on 2019/5/30.
//

#include "utils/ast.hpp"

namespace yapc {

    genValue ConstDeclAST::codegen(CodeGenUtils &context) {
        printf("fff");
        if (context.is_subroutine) {
            // TODO: String check
            auto *local = context.GetBuilder().CreateAlloca(value->GetType(context));
            auto success = context.SetValue(name->GetName(), local);
            if (!success) {
                throw CodegenException("duplicate identifier with const " + name->GetName());
            }
            context.GetBuilder().CreateStore(value->codegen(context), local);
            return local;
        }
        else {
            // TODO String check
            auto *constant = llvm::cast<llvm::Constant>(value->codegen(context));
            return new llvm::GlobalVariable(*context.GetModule(), value->GetType(context), true, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
        }
    }

    genValue VarDeclAST::codegen(CodeGenUtils &context) {
        if (context.is_subroutine) {
            auto *local = context.GetBuilder().CreateAlloca(type->GetType(context));
            auto success = context.SetValue(name->GetName(), local);
            if (!success)
                throw CodegenException("duplicate identifier with var " + name->GetName());
            return local;
        }
        else {
            auto *type = this->type->GetType(context);
            llvm::Constant *constant;
            switch (this->type->type) {
                case Type::LONGINT: constant = llvm::ConstantInt::get(type, 0);
                case Type::INTEGER: constant = llvm::ConstantInt::get(type, 0);
                case Type::REAL: constant = llvm::ConstantFP::get(type, 0.0);
                case Type::BOOLEAN: constant = llvm::ConstantInt::get(type, 0);
                default: throw CodegenException("unsupported type");  // TODO String support
            }
        };
    }

    genValue TypeDeclAST::codegen(CodeGenUtils &context) {
        auto success = context.SetAlias(name->GetName(), type->GetType(context));
        if (!success) {
            throw CodegenException("duplicate type alias");
        }
        return nullptr;
    }
}
