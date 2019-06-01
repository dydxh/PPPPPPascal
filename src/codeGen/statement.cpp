//
// Created by zuhxs on 2019/6/1.
//

#include <utils/ast.hpp>

namespace yapc {
    genValue IfStmtAST::codegen(CodeGenUtils &context) {
        auto *condition = expr->codegen(context);
        if (!condition->getType()->isIntegerTy(1)) {
            throw CodegenException("if statement not boolean");
        }

        auto *func = context.GetBuilder().GetInsertBlock()->getParent();
        auto *true_stat = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "then", func);
        auto *else_stat = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "else");
        auto *cond_id = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "cont");
        context.GetBuilder().CreateCondBr(condition, true_stat, else_stat);

        context.GetBuilder().SetInsertPoint(true_stat);
        stmt->codegen(context);
        context.GetBuilder().CreateBr(cond_id);

        func->getBasicBlockList().push_back(else_stat);
        context.GetBuilder().SetInsertPoint(else_stat);
        else_stmt->codegen(context);
        context.GetBuilder().CreateBr(cond_id);

        func->getBasicBlockList().push_back(cond_id);
        context.GetBuilder().SetInsertPoint(cond_id);

    }

    genValue AssignStmtAST::codegen(CodeGenUtils &context) {
        printf("inside assignment ast.\n");
        auto *lhs = this->lhs->codegen(context);
        auto *rhs = this->rhs->codegen(context);

        auto *lhs_type = lhs->getType()->getPointerElementType();
        auto *rhs_type = rhs->getType();

        if (lhs_type->isDoubleTy() && rhs_type->isIntegerTy(32)) {
            rhs = context.GetBuilder().CreateSIToFP(rhs, context.GetBuilder().getDoubleTy());
        }
        else if (!((lhs_type->isIntegerTy(1)  && rhs_type->isIntegerTy(1)) ||
                   (lhs_type->isIntegerTy(32) && rhs_type->isIntegerTy(32)) ||
                   (lhs_type->isDoubleTy()    && rhs_type->isDoubleTy())))
        {
            throw CodegenException("imcompatible assignment type");
        }
        context.GetBuilder().CreateStore(rhs, lhs);
        return nullptr;
    }
}