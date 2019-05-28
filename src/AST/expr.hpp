#ifndef yapc_ast_expr
#define yapc_ast_expr

#include "basicast.hpp"
#include "identifier.hpp"
#include "decleration.hpp"

namespace yapc {
    enum class SysFunc {
        READ, WRITE, READLN, WRITELN
    };

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

        UnaryExprAST(UnaryOp op, std::unique_ptr<ExprAST>& val) : op(op), rhs(std::move(val)) {}
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

    class FuncCallAST : public ExprAST {
    public:
        std::unique_ptr<FuncCallAST> funccall;

        FuncCallAST() = default;
        FuncCallAST(std::unique_ptr<FuncCallAST>& call_stmt) : funccall(std::move(call_stmt)) {}

        genValue codegen(genContext context) override;
    };

    class CustomFuncAST : public FuncCallAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<ArgListAST> args;

        CustomFuncAST(std::unique_ptr<IdentifierAST>& name, std::unique_ptr<ArgListAST>& args) : name(std::move(name)), args(std::move(args)) {}
        CustomFuncAST(std::unique_ptr<IdentifierAST>& name) : name(std::move(name)), args(nullptr) {}

        genValue codegen(genContext context) override;
    };

    class SysFuncAST : public FuncCallAST {
    public:
        SysFunc name;
        std::unique_ptr<ArgListAST> args;

        SysFuncAST(const SysFunc& name, std::unique_ptr<ArgListAST>& args) : name(name), args(std::move(args)) {}
        SysFuncAST(const SysFunc& name) : name(name), args(nullptr) {}

        genValue codegen(genContext context) override;
    };
}

#endif
