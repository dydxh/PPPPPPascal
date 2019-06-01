//
// Created by zuhxs on 2019/5/29.
//

#include "utils/ast.hpp"


namespace yapc {

    // TODO: powi supporting (llvm::Intrinsic::powi)

    genValue BinaryExprAST::codegen(genContext context) {
        auto *lhs = this->lhs->codegen(context);
        auto *rhs = this->rhs->codegen(context);
        std::map<BinaryOp, llvm::CmpInst::Predicate> BTypeAlt = {
                {BinaryOp::GT, llvm::CmpInst::ICMP_SGT},
                {BinaryOp::GE, llvm::CmpInst::ICMP_SGE},
                {BinaryOp::LT, llvm::CmpInst::ICMP_SLT},
                {BinaryOp::LE, llvm::CmpInst::ICMP_SLE},
                {BinaryOp::EQ, llvm::CmpInst::ICMP_EQ},
                {BinaryOp::NE, llvm::CmpInst::ICMP_NE}
        };
        std::map<BinaryOp, llvm::CmpInst::Predicate> FTypeAlt = {
                {BinaryOp::GT, llvm::CmpInst::FCMP_OGT},
                {BinaryOp::GE, llvm::CmpInst::FCMP_OGE},
                {BinaryOp::LT, llvm::CmpInst::FCMP_OLT},
                {BinaryOp::LE, llvm::CmpInst::FCMP_OLE},
                {BinaryOp::EQ, llvm::CmpInst::FCMP_OEQ},
                {BinaryOp::NE, llvm::CmpInst::FCMP_ONE}
        };

        if (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy()) {
            if (!lhs->getType()->isDoubleTy()) {
                lhs = context.GetBuilder().CreateSIToFP(lhs, context.GetBuilder().getDoubleTy());
            }
            else if (!rhs->getType()->isDoubleTy()) {
                rhs = context.GetBuilder().CreateSIToFP(rhs, context.GetBuilder().getDoubleTy());
            }
            auto it = FTypeAlt.find(op);
            if (it != FTypeAlt.end()) {
                return context.GetBuilder().CreateFCmp(it->second, lhs, rhs);
            }
            llvm::Instruction::BinaryOps binop;
            switch(op) {
                case BinaryOp::ADD: binop = llvm::Instruction::FAdd; break;
                case BinaryOp::SUB: binop = llvm::Instruction::FSub; break;
                case BinaryOp::SLASH: binop = llvm::Instruction::FDiv; break;
                case BinaryOp::MUL: binop = llvm::Instruction::FMul; break;
                default: throw CodegenException("operation not valid");
            }
            return context.GetBuilder().CreateBinOp(binop, lhs, rhs);
        }

        else if (lhs->getType()->isIntegerTy(1) && rhs->getType()->isIntegerTy(1)) {
            auto it = BTypeAlt.find(op);
            if (it != BTypeAlt.end()) {
                return context.GetBuilder().CreateICmp(it->second, lhs, rhs);
            }
            llvm::Instruction::BinaryOps binop;
            switch (op) {
                case BinaryOp::AND: binop = llvm::Instruction::And; break;
                case BinaryOp::OR: binop = llvm::Instruction::Or; break;
                case BinaryOp::XOR: binop = llvm::Instruction::Xor; break;
                default: throw CodegenException("operation not valid");
            }
            return context.GetBuilder().CreateBinOp(binop, lhs, rhs);
        }

        else if (lhs->getType()->isIntegerTy(32) && rhs->getType()->isIntegerTy(32)) {
            auto it = BTypeAlt.find(op);
            if (it != BTypeAlt.end()) {
                return context.GetBuilder().CreateICmp(it->second, lhs, rhs);
            }
            llvm::Instruction::BinaryOps binop;
            switch (op) {
                case BinaryOp::ADD: binop = llvm::Instruction::Add; break;
                case BinaryOp::SUB: binop = llvm::Instruction::Sub; break;
                case BinaryOp::MUL: binop = llvm::Instruction::Mul; break;
                case BinaryOp::DIV: binop = llvm::Instruction::SDiv; break;
                case BinaryOp::MOD: binop = llvm::Instruction::SRem; break;
                case BinaryOp::AND: binop = llvm::Instruction::And; break;
                case BinaryOp::OR: binop = llvm::Instruction::Or; break;
                case BinaryOp::XOR: binop = llvm::Instruction::Xor; break;
                case BinaryOp::SLASH:
                    lhs = context.GetBuilder().CreateSIToFP(lhs, context.GetBuilder().getDoubleTy());
                    rhs = context.GetBuilder().CreateSIToFP(rhs, context.GetBuilder().getDoubleTy());
                    binop = llvm::Instruction::FDiv; break;
                default: throw CodegenException("operator not valid");
            }
        }


    }




}