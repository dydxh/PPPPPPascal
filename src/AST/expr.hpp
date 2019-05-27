//
// Created by dydxh on 5/28/19.
//

#ifndef yapc_ast_expr
#define yapc_ast_expr

#include "basicast.hpp"
#include "identifier.hpp"

namespace yapc {
    enum class UnaryOp {
        NEG, POS, NOT
    };

    enum class BinaryOp {
        EQ, NE, LE, GE,
        LT, GT, ADD, SUB,
        DIV, SLASH, MOD, MUL,
        OR, AND, XOR
    };

    class BinaryExprAST : public ExprAST {
    public:
        BinaryOp op;
        std::unique_ptr<ExprAST> lhs, rhs;

        BinaryExprAST(BinaryOp op, std::unique_ptr<ExprAST>& lval, std::unique_ptr<ExprAST>& rval) : op(op) {
            lhs = std::move(lval);
            rhs = std::move(rval);
        }

        genValue codegen(genContext context) override;
    };

    class UnaryExprAST : public ExprAST {
    public:
        UnaryOp op;
        std::unique_ptr<ExprAST> rhs;

        UnaryExprAST(UnaryOp op, std::unique_ptr<ExprAST>& val) : op(op), rhs(std::move(rhs)) {}
    };

    class FuncCallAST : public ExprAST {
        
    };

    class ArrayAccessAST : public ExprAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<ExprAST> index;

        ArrayAccessAST(std::unique_ptr<IdentifierAST>& ID, std::unique_ptr<ExprAST>& idx) {
            name = std::move(ID);
            index = std::move(idx);
        }
    };

    class RecordAccessAST : public ExprAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<IdentifierAST> field;

        RecordAccessAST(std::unique_ptr<IdentifierAST>& ID, std::unique_ptr<IdentifierAST>& field) {
            name = std::move(ID);
            field = std::move(field);
        }
    };
}

#endif //YAPC_EXPR_HPP
