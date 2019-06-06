//
// Created by zuhxs on 2019/5/30.
//

#include "utils/ast.hpp"

namespace yapc {

    genValue ConstDeclAST::codegen(CodeGenUtils &context) {
        if (context.is_subroutine) {   // not main funciton
            auto IfString = std::dynamic_pointer_cast<yapc::StringAST>(value);
            if (IfString) {
                std::string prefix(std::move(context.GetTrace().back()));
                std::cout << prefix << std::endl;
                context.GetTrace().push_back(prefix);
                return context.GetBuilder().CreateGlobalStringPtr(IfString->val,  prefix + "_" + this->name->GetName());
            }
            auto *constant = llvm::cast<llvm::Constant>(value->codegen(context));
            std::string prefix(std::move(context.GetTrace().back()));
            std::cout << prefix << std::endl;
            context.GetTrace().push_back(prefix);
            return new llvm::GlobalVariable(*context.GetModule(), value->GetType(context), true, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + name->GetName());
        }
        else {  // main function
            auto IfString = std::dynamic_pointer_cast<yapc::StringAST>(value);
            if (IfString) {
                auto *constant = llvm::ConstantDataArray::getString(llvm_context, IfString->val, true);
                llvm::Value *result = new llvm::GlobalVariable(*context.GetModule(), constant->getType(), true, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
                return result;
            }
            auto *constant = llvm::cast<llvm::Constant>(value->codegen(context));
            return new llvm::GlobalVariable(*context.GetModule(), value->GetType(context), true, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
        }
    }

    genValue VarDeclAST::codegen(CodeGenUtils &context) {
        if (context.is_subroutine) {
            llvm::Type *type = this->type->GetType(context);
            llvm::Constant *constant;

            std::string prefix(std::move(context.GetTrace().back()));
            std::cout << prefix << std::endl;
            context.GetTrace().push_back(prefix);
            switch (this->type->type) {  // TODO: new type support
                case Type::LONGINT: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::INTEGER: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::REAL: constant = llvm::ConstantFP::get(type, 0.0); break;
                case Type::BOOLEAN: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::STRING: constant = llvm::ConstantDataArray::getString(llvm_context, "", true); break;
                //case Type::ARRAY: {llvm::ArrayRef<uint32_t> a(std::vector<uint32_t>(5, 2)); constant = llvm::ConstantDataArray::get(llvm_context, a); break;}
                case Type::ARRAY: {
                    llvm::IntegerType* i32 = llvm::IntegerType::get(context.GetModule()->getContext(), 32);
                    llvm::Constant *one = llvm::ConstantInt::get(i32, 1);
                    llvm::ArrayType* int_3 = llvm::ArrayType::get(i32, 3);
                    auto * variable = llvm::ConstantArray::get(int_3, std::vector<llvm::Constant *>{one, one, one});
                    return new llvm::GlobalVariable(*context.GetModule(), variable->getType(), false, llvm::GlobalVariable::ExternalLinkage, variable, prefix+"_"+name->GetName());
                    //return new llvm::GlobalVariable(*context.GetModule(), variable->getPointerElementType(), false, llvm::GlobalVariable::ExternalLinkage, variable, prefix + "_" + name->GetName());
                }
                default: throw CodegenException("unsupported type1");  // TODO String support
            }
            return new llvm::GlobalVariable(*context.GetModule(), constant->getType(), false, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + name->GetName());
        }
        else {    // main function
            llvm::Type *type = this->type->GetType(context);
            llvm::Constant *constant;

            switch (this->type->type) {  // TODO: new type support
                case Type::LONGINT: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::INTEGER: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::REAL: constant = llvm::ConstantFP::get(type, 0.0); break;
                case Type::BOOLEAN: constant = llvm::ConstantInt::get(type, 0); break;
                case Type::STRING: constant = llvm::ConstantDataArray::getString(llvm_context, "", true); break;
                case Type::ARRAY: {
                    auto array_it = std::dynamic_pointer_cast<ArrayTypeAST>(this->type);
                    auto *array_type = array_it->ItemType->GetType(context);
                    //llvm::IntegerType* i32 = llvm::IntegerType::get(context.GetModule()->getContext(), 32);
                    llvm::Constant *constant;
                    if (array_type->isIntegerTy(32)) {
                        constant = llvm::ConstantInt::get(array_type, 0);
                    }
                    else if (array_type->isIntegerTy(1)) {
                        constant = llvm::ConstantInt::get(array_type, 0);
                    }
                    else if (array_type->isDoubleTy()) {
                        constant = llvm::ConstantFP::get(type, 0.0);
                    }
                    else {
                        throw CodegenException("element type for array not supported");
                    }
                    llvm::Constant *one = llvm::ConstantInt::get(array_type, 0);

                    int number_value = 0;
                    if (llvm::ConstantInt* CI = llvm::dyn_cast<llvm::ConstantInt>(array_it->EndValue->codegen(context))) {
                        if (CI->getBitWidth() <= 32) {
                            number_value = CI->getSExtValue() + 1; // add 1 since the index starts from 0
                        }
                    }
                    else {
                        throw CodegenException("Initial array index not valid");
                    }
                    llvm::ArrayType* int_3 = llvm::ArrayType::get(array_type, number_value);
                    std::vector<llvm::Constant *> InitVector;
                    for (int i = 0; i < number_value; i++) {
                        InitVector.push_back(one);
                    }
                    auto * variable = llvm::ConstantArray::get(int_3, InitVector);
                    return new llvm::GlobalVariable(*context.GetModule(), variable->getType(), false, llvm::GlobalVariable::ExternalLinkage, variable, name->GetName());

                    //auto * variable = llvm::ArrayType::get(context.GetBuilder().getInt32Ty(), 5);

                    //return new llvm::GlobalVariable(*context.GetModule(), constant->getType(), false, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
                }
                default: throw CodegenException("unsupported type2");  // TODO String support
            }
            return new llvm::GlobalVariable(*context.GetModule(), constant->getType(), false, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
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
