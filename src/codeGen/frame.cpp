//
// Created by zuhxs on 2019/5/30.
//

#include "utils/ast.hpp"

namespace yapc {
    genValue ProgramAST::codegen(CodeGenUtils &context) {
        progblock->constpart->codegen(context);
        progblock->typepart->codegen(context);
        progblock->varpart->codegen(context);

        auto *func_type = llvm::FunctionType::get(context.GetBuilder().getInt32Ty(), false);
        auto *main_func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                                 "main", *context.GetModule().get());
        auto *block = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "entry", main_func);
        context.GetBuilder().SetInsertPoint(block);

        progblock->progbody->codegen(context);
        context.GetBuilder().CreateRet(context.GetBuilder().getInt32(0));
    }
}