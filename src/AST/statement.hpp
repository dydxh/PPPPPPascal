#ifndef yapc_ast_statement
#define yapc_ast_statement

#include <list>

#include "basicast.hpp"
#include "expr.hpp"

namespace yapc {
    using CompoundStmtAST = ListAST<StmtAST>;

    class IfStmtAST : public  StmtAST {
    public:
        std::unique_ptr<ExprAST> expr;
        std::unique_ptr<CompoundStmtAST> stmt;
        std::unique_ptr<CompoundStmtAST> else_stmt;

        IfStmtAST(std::unique_ptr<ExprAST>& expr, std::unique_ptr<CompoundStmtAST> stmt)
            : expr(std::move(expr)), stmt(std::move(stmt)), else_stmt(nullptr) {}
        IfStmtAST(std::unique_ptr<ExprAST>& expr, std::unique_ptr<CompoundStmtAST> stmt, std::unique_ptr<CompoundStmtAST> else_stmt)
            : expr(std::move(expr)), stmt(std::move(stmt)), else_stmt(std::move(else_stmt)) {}
        ~IfStmtAST() = default;

        genValue codegen(genContext context) override;
    };

    class WhileStmtAST : public StmtAST {
    public:
        std::unique_ptr<ExprAST> expr;
        std::unique_ptr<CompoundStmtAST> stmt;

        WhileStmtAST(std::unique_ptr<ExprAST>& expr, std::unique_ptr<CompoundStmtAST>& stmt) : expr(std::move(expr)), stmt(std::move(stmt)) {}

        genValue codegen(genContext context) override;
    };

    enum class Direct {
        TO, DOWNTO
    };

    class ForStmtAST : public  StmtAST {
    public:
        Direct direct;
        std::unique_ptr<IdentifierAST> identifier;
        std::unique_ptr<ExprAST> start_val;
        std::unique_ptr<ExprAST> end_val;
        std::unique_ptr<CompoundStmtAST> stmt;

        ForStmtAST(Direct dir, std::unique_ptr<IdentifierAST>& identifier, std::unique_ptr<ExprAST>& startval, std::unique_ptr<ExprAST>& endval, std::unique_ptr<CompoundStmtAST>& stmt)
            : direct(dir), identifier(std::move(identifier)), start_val(std::move(startval)), end_val(std::move(endval)), stmt(std::move(stmt)) {}

        genValue codegen(genContext context) override;
    };

    class RepeatStmtAST : public StmtAST {
    public:
        std::unique_ptr<ExprAST> expr;
        std::unique_ptr<CompoundStmtAST> stmt;

        RepeatStmtAST(std::unique_ptr<ExprAST>& expr, std::unique_ptr<CompoundStmtAST>& stmt) : expr(std::move(expr)), stmt(std::move(stmt)) {}

        genValue codegen(genContext context) override;
    };

    class CallStmtAST : public StmtAST {
    public:
        std::unique_ptr<FuncCallAST> func;

        CallStmtAST(std::unique_ptr<CallStmtAST> func) : func(std::move(func)) {}

        genValue codegen(genContext context) override;
    };

    class AssignStmtAST : public StmtAST {
    public:
        std::unique_ptr<ExprAST> lhs;
        std::unique_ptr<ExprAST> rhs;

        AssignStmtAST(std::unique_ptr<ExprAST>& lval, std::unique_ptr<ExprAST>& rval) : lhs(std::move(lval)), rhs(std::move(rval)) {}

        genValue codegen(genContext context) override;
    };
}

#endif
