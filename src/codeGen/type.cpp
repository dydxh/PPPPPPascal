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
        auto *type = llvm::ConstantDataArray::getString(llvm_context, val, true);
        return type;
    }

    llvm::Type *TypeAST::GetType(CodeGenUtils &context) {
        if (auto *PrimaryType = dynamic_cast<const PrimaryTypeAST*>(this)){
            switch (type) {
                case Type::BOOLEAN: return context.GetBuilder().getInt1Ty();
                case Type::INTEGER: return context.GetBuilder().getInt32Ty();
                case Type::LONGINT: return context.GetBuilder().getInt32Ty();
                case Type::REAL: return context.GetBuilder().getDoubleTy();
                default: throw CodegenException("Unsupported type3");
            }
        }
        else if (auto *DecType = dynamic_cast<const DeclTypeAST*>(this)) {
            return context.GetAlias(DecType->name->GetName());
        }
        else if (auto *DecType = dynamic_cast<const VoidTypeAST*>(this)) {
            return context.GetBuilder().getVoidTy();  // used for procedure
        }
        else if (auto *DecType = dynamic_cast<const StringTypeAST*>(this)) {
            return nullptr;
        }
        else {
            throw CodegenException("Unsupported type4");
        }
    }

}