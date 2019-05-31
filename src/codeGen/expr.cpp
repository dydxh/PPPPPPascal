//
// Created by zuhxs on 2019/5/29.
//

#include "utils/ast.hpp"


namespace yapc {


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
        }


    }




}