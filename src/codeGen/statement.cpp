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
        auto *end = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "end");
        context.GetBuilder().CreateCondBr(condition, true_stat, else_stat);

        context.GetBuilder().SetInsertPoint(true_stat);
        stmt->codegen(context);
        context.GetBuilder().CreateBr(end);

        func->getBasicBlockList().push_back(else_stat);
        context.GetBuilder().SetInsertPoint(else_stat);
        else_stmt->codegen(context);
        context.GetBuilder().CreateBr(end);

        func->getBasicBlockList().push_back(end);
        context.GetBuilder().SetInsertPoint(end);
    }

    genValue AssignStmtAST::codegen(CodeGenUtils &context) {
        printf("inside assignment ast.\n");
        // get the lhs pointer, first cast
        llvm::Value *lhs;
        auto id_cast = std::dynamic_pointer_cast<IdentifierAST>(this->lhs);
        if (id_cast) {
            lhs = id_cast->GetPtr(context);
        }
        else {
            auto array_cast = std::dynamic_pointer_cast<ArrayAccessAST>(this->lhs);
            if (array_cast) {
                lhs = array_cast->GetPtr(context);
            }
            else
                throw CodegenException("assignment left argument not a identifier.\n");
        }
        printf("found\n");
        if (std::dynamic_pointer_cast<FuncCallAST>(this->lhs)) {
            printf("It is a custom func call!\n");
        }

        auto *rhs = this->rhs->codegen(context);
        //rhs = context.GetBuilder().CreateLoad(rhs);

        auto *lhs_type = lhs->getType()->getPointerElementType();
        auto *rhs_type = rhs->getType();

        if (lhs_type->isDoubleTy() && rhs_type->isIntegerTy(32)) {
            rhs = context.GetBuilder().CreateSIToFP(rhs, context.GetBuilder().getDoubleTy());
        }
        else if (!((lhs_type->isIntegerTy(1) && rhs_type->isIntegerTy(1))  // bool
                   || (lhs_type->isIntegerTy(32) && rhs_type->isIntegerTy(32))  // int
                   || (lhs_type->isDoubleTy() && rhs_type->isDoubleTy()) // float
                   || (lhs_type->isArrayTy() && rhs_type->isPointerTy())
                   || (lhs_type->isArrayTy() && rhs_type->isIntegerTy(32)))) // string
        {
            throw CodegenException("incompatible assignment type");
        }
        auto *lhs_type2 =  lhs->getType()->getPointerElementType();
        //assert(rhs->getType() == llvm::cast<llvm::PointerType>(lhs->getType())->getElementType());
        context.GetBuilder().CreateStore(rhs, lhs);
        return nullptr;
    }


    genValue WhileStmtAST::codegen(CodeGenUtils &context) {
        auto *func = context.GetBuilder().GetInsertBlock()->getParent();
        auto *while_stat = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "while", func);
        auto *loop_stat = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "loop", func);
        auto *end = llvm::BasicBlock::Create(context.GetModule().get()->getContext(), "end");

        context.GetBuilder().CreateBr(while_stat);
        context.GetBuilder().SetInsertPoint(while_stat);

        auto *cond = expr->codegen(context);
        if (!cond->getType()->isIntegerTy(1)) {  // not boolean
            throw CodegenException("while statement not boolean");
        }
        context.GetBuilder().CreateCondBr(cond, loop_stat, end);   // true -> loop_stat, false -> end

        context.GetBuilder().SetInsertPoint(loop_stat);
        stmt->codegen(context);
        context.GetBuilder().CreateBr(while_stat);   // context -> statement

        func->getBasicBlockList().push_back(end);    // end label
        context.GetBuilder().SetInsertPoint(end);

        return nullptr;
    }

    genValue ForStmtAST::codegen(CodeGenUtils &context) {
        if (!identifier->codegen(context)->getType()->isIntegerTy(32)) {
            throw CodegenException("for loop identifier not integer");
        }
        auto init = MakeAST<AssignStmtAST>(identifier, start_val);
        auto upto = direct == Direct::TO;
        auto cond = MakeAST<BinaryExprAST>(upto ? BinaryOp::LE : BinaryOp::GE, identifier, end_val);
        auto iter = MakeAST<AssignStmtAST>(identifier, MakeAST<BinaryExprAST>(upto ? BinaryOp::ADD : BinaryOp::SUB, identifier, MakeAST<IntegerAST>(1)));
        auto compound = MakeAST<CompoundStmtAST>();

        compound->Merge(stmt);
        compound->AppendChild(iter);
        auto while_stmt = MakeAST<WhileStmtAST>(cond, compound);
        init->codegen(context);
        while_stmt->codegen(context);
    }


    genValue CallStmtAST::codegen(CodeGenUtils &context) {
        func->codegen(context);
        return nullptr;
    }
}