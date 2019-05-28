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
        OR, AND, XOR, POW
    };

    class BinaryExprAST : public ExprAST {
    public:
        BinaryOp op;
        std::shared_ptr<ExprAST> lhs, rhs;

        BinaryExprAST(BinaryOp op, std::shared_ptr<ExprAST>& lval, std::shared_ptr<ExprAST>& rval) : op(op) {
            lhs = lval;
            rhs = rval;
        }

        //genValue codegen(genContext context) override;
    };

    class UnaryExprAST : public ExprAST {
    public:
        UnaryOp op;
        std::shared_ptr<ExprAST> rhs;

        UnaryExprAST(UnaryOp op, std::shared_ptr<ExprAST>& val) : op(op), rhs(val) {}
    };

    class ArrayAccessAST : public ExprAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<ExprAST> index;

        ArrayAccessAST(std::string& ID, std::shared_ptr<ExprAST>& idx) {
            name = MakeAST<IdentifierAST>(ID);
            index = idx;
        }

        ArrayAccessAST(std::shared_ptr<IdentifierAST>& ID, std::shared_ptr<ExprAST>& idx) {
            name = ID;
            index = idx;
        }
    };

    class RecordAccessAST : public ExprAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<IdentifierAST> field;

        RecordAccessAST(std::string& ID, std::string& field) {
            this->name = MakeAST<IdentifierAST>(ID);
            this->field = MakeAST<IdentifierAST>(field);
        }
        RecordAccessAST(std::shared_ptr<IdentifierAST>& ID, std::shared_ptr<IdentifierAST>& field) {
            name = ID;
            field = field;
        }
    };

    class FuncCallAST : public ExprAST {
    public:
        std::shared_ptr<FuncCallAST> funccall;

        FuncCallAST() = default;
        FuncCallAST(std::shared_ptr<FuncCallAST>& call_stmt) : funccall(call_stmt) {}

        //genValue codegen(genContext context) override;
    };

    class CustomFuncAST : public FuncCallAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<ArgListAST> args;

        CustomFuncAST(std::string& name, std::shared_ptr<ArgListAST>& args) {
            this->name = MakeAST<IdentifierAST>(name);
            this->args = args;
        }
        CustomFuncAST(std::shared_ptr<IdentifierAST>& name, std::shared_ptr<ArgListAST>& args) : name(name), args(args) {}

        CustomFuncAST(std::string& name) : name(MakeAST<IdentifierAST>(name)), args(nullptr) {}
        CustomFuncAST(std::shared_ptr<IdentifierAST>& name) : name(name), args(nullptr) {}

        //genValue codegen(genContext context) override;
    };

    class SysFuncAST : public FuncCallAST {
    public:
        SysFunc name;
        std::shared_ptr<ArgListAST> args;

        SysFuncAST(const SysFunc& name, std::shared_ptr<ArgListAST>& args) : name(name), args(args) {}
        SysFuncAST(const SysFunc& name) : name(name), args(nullptr) {}

        //genValue codegen(genContext context) override;
    };
}

#endif
