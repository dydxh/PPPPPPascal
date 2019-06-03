//
// Created by zuhxs on 2019/5/30.
//

#include "utils/ast.hpp"
#include "codeGen_utils.hpp"

namespace yapc {
    genValue ProgramAST::codegen(CodeGenUtils &context) {

        if (!context.is_subroutine) {    // main function
            auto *func_type = llvm::FunctionType::get(context.GetBuilder().getInt32Ty(), false);
            auto *main_func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                                     "main", *context.GetModule().get());
            auto *block = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "entry", main_func);
            context.GetBuilder().SetInsertPoint(block);

            progblock->constpart->codegen(context);
            progblock->typepart->codegen(context);
            progblock->varpart->codegen(context);

            //printf("aaa\n");
            progblock->progbody->codegen(context);
            context.GetBuilder().CreateRet(context.GetBuilder().getInt32(0));

            context.is_subroutine = true;
            progblock->progpart->codegen(context);
            context.is_subroutine = false;

            // llvm::verifyFunction(*main_func, &llvm::errs());
            // if (context.fpm)
            // { context.fpm->run(*main_func); }
            // if (context.mpm)
            // { context.mpm->run(*(context.GetModule())); }
        }
        else {
            context.GetTrace().push_back(proghead->name->GetName());  // push back the trace

            std::vector<llvm::Type *> types;
            std::vector<std::string> names;
            for (auto p : proghead->params->get_children()) {
                types.push_back(p->type->GetType(context));
                names.push_back(p->name->GetName());
            }
            auto *func_type = llvm::FunctionType::get(this->proghead->type->GetType(context), types, false);
            auto *func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, proghead->name->GetName(), *context.GetModule().get());
            auto *block = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "entry", func);
            context.GetBuilder().SetInsertPoint(block);

            auto index = 0;
            for (auto &arg : func->args()) {
                auto *type = arg.getType();
                llvm::Constant *constant;
                if (type->isIntegerTy(32)) {
                    constant = llvm::ConstantInt::get(type, 0);
                }
                else if (type->isDoubleTy()) {
                    constant = llvm::ConstantFP::get(type, 0.0);
                }
                else {
                    throw CodegenException("Unknown function return type");
                }
                std::string prefix(std::move(context.GetTrace().back()));
                std::cout << prefix << std::endl;
                context.GetTrace().push_back(prefix);
                auto *variable = new llvm::GlobalVariable(*context.GetModule(), type, false, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + names[index++]);
                context.GetBuilder().CreateStore(&arg, variable);
                //auto *local = context.GetBuilder().CreateAlloca(arg.getType());
                //context.SetValue(names[index++], local);
                //context.GetBuilder().CreateStore(&arg, local);
            }
            progblock->constpart->codegen(context);
            progblock->typepart->codegen(context);
            progblock->varpart->codegen(context);

            if (proghead->type->type != Type::VOID) {   // set the return variable
                auto *type = proghead->type->GetType(context);
                llvm::Constant *constant;
                if (type->isIntegerTy(32)) {
                    constant = llvm::ConstantInt::get(type, 0);
                }
                else if (type->isDoubleTy()) {
                    constant = llvm::ConstantFP::get(type, 0.0);
                }
                else {
                    throw CodegenException("Unknown function return type");
                }
                std::string prefix(std::move(context.GetTrace().back()));
                std::cout << prefix << std::endl;
                context.GetTrace().push_back(prefix);
                auto *variable = new llvm::GlobalVariable(*context.GetModule(), type, false, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + proghead->name->GetName());

                //auto *ret = context.GetBuilder().CreateAlloca(proghead->type->GetType(context));
                //context.SetValue(proghead->name->GetName(), ret);
            }

            progblock->progpart->codegen(context);


            block = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "back", func);
            context.GetBuilder().SetInsertPoint(block);
            progblock->progbody->codegen(context);

            if (proghead->type->type != Type::VOID) {
                auto *local = context.GetValue(proghead->name->GetName());
                auto *ret = context.GetBuilder().CreateLoad(local);
                context.GetBuilder().CreateRet(ret);
            } else {
                context.GetBuilder().CreateRetVoid();
            }

            // llvm::verifyFunction(*func, &llvm::errs());
            // if (context.fpm)
            // { context.fpm->run(*func); }

            return nullptr;

        }

    }
}