//
// Created by zuhxs on 2019/5/30.
//

#include "utils/ast.hpp"

namespace yapc {

    genValue ConstDeclAST::codegen(CodeGenUtils &context) {
        if (context.is_subroutine) {   // not main funciton
            auto *constant = llvm::cast<llvm::Constant>(value->codegen(context));
            std::string prefix(std::move(context.GetTrace().back()));
            std::cout << prefix << std::endl;
            context.GetTrace().push_back(prefix);
            return new llvm::GlobalVariable(*context.GetModule(), value->GetType(context), true, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + name->GetName());
        }
        else {  // main function
            // TODO String check
            auto *constant = llvm::cast<llvm::Constant>(value->codegen(context));
            return new llvm::GlobalVariable(*context.GetModule(), value->GetType(context), true, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
        }
    }

    genValue VarDeclAST::codegen(CodeGenUtils &context) {
        if (context.is_subroutine) {
            auto *type = this->type->GetType(context);
            llvm::Constant *constant;

            switch (this->type->type) {  // TODO: new type support
                case Type::LONGINT: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::INTEGER: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::REAL: constant = llvm::ConstantFP::get(type, 0.0); break;
                case Type::BOOLEAN: constant = llvm::ConstantInt::get(type, 0); break;
                default: throw CodegenException("unsupported type");  // TODO String support
            }
            std::string prefix(std::move(context.GetTrace().back()));
            std::cout << prefix << std::endl;
            context.GetTrace().push_back(prefix);
            return new llvm::GlobalVariable(*context.GetModule(), type, false, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + name->GetName());
        }
        else {    // main function
            auto *type = this->type->GetType(context);
            llvm::Constant *constant;

            switch (this->type->type) {  // TODO: new type support
                case Type::LONGINT: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::INTEGER: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::REAL: constant = llvm::ConstantFP::get(type, 0.0); break;
                case Type::BOOLEAN: constant = llvm::ConstantInt::get(type, 0); break;
                default: throw CodegenException("unsupported type");  // TODO String support
            }
            return new llvm::GlobalVariable(*context.GetModule(), type, false, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
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
