#ifndef yapc_ast_statement
#define yapc_ast_statement

#include <list>

#include "basicast.hpp"
#include "expr.hpp"

namespace yapc {
    using CompoundStmtAST = ListAST<StmtAST>;

    class IfStmtAST : public StmtAST {
    public:
        std::shared_ptr<ExprAST> expr;
        std::shared_ptr<CompoundStmtAST> stmt;
        std::shared_ptr<CompoundStmtAST> else_stmt;

        IfStmtAST(const std::shared_ptr<ExprAST>& expr, const std::shared_ptr<CompoundStmtAST> stmt)
            : expr(expr), stmt(stmt), else_stmt(nullptr) {}
        IfStmtAST(const std::shared_ptr<ExprAST>& expr, const std::shared_ptr<CompoundStmtAST> stmt, const std::shared_ptr<CompoundStmtAST> else_stmt)
            : expr(expr), stmt(stmt), else_stmt(else_stmt) {}
        ~IfStmtAST() = default;

        genValue codegen(genContext context) override;
    };

    class WhileStmtAST : public StmtAST {
    public:
        std::shared_ptr<ExprAST> expr;
        std::shared_ptr<CompoundStmtAST> stmt;

        WhileStmtAST(const std::shared_ptr<ExprAST>& expr, const std::shared_ptr<CompoundStmtAST>& stmt) : expr(expr), stmt(stmt) {}

        genValue codegen(genContext context) override;
    };

    enum class Direct {
        TO, DOWNTO
    };

    class ForStmtAST : public StmtAST {
    public:
        Direct direct;
        std::shared_ptr<IdentifierAST> identifier;
        std::shared_ptr<ExprAST> start_val;
        std::shared_ptr<ExprAST> end_val;
        std::shared_ptr<CompoundStmtAST> stmt;

        ForStmtAST(Direct dir, const std::shared_ptr<IdentifierAST>& identifier, const std::shared_ptr<ExprAST>& startval, const std::shared_ptr<ExprAST>& endval, const std::shared_ptr<CompoundStmtAST>& stmt)
            : direct(dir), identifier(identifier), start_val(startval), end_val(endval), stmt(stmt) {}

        genValue codegen(genContext context) override;
    };

    class RepeatStmtAST : public StmtAST {
    public:
        std::shared_ptr<ExprAST> expr;
        std::shared_ptr<CompoundStmtAST> stmt;

        RepeatStmtAST(const std::shared_ptr<ExprAST>& expr, const std::shared_ptr<CompoundStmtAST>& stmt) : expr(expr), stmt(stmt) {}

        genValue codegen(genContext context) override {}
    };

    class CallStmtAST : public StmtAST {
    public:
        std::shared_ptr<FuncCallAST> func;

        CallStmtAST(const std::shared_ptr<FuncCallAST>& func) : func(func) {}

        genValue codegen(genContext context) override;
    };

    class AssignStmtAST : public StmtAST {
    public:
        std::shared_ptr<ExprAST> lhs;
        std::shared_ptr<ExprAST> rhs;

        AssignStmtAST(const std::shared_ptr<ExprAST>& lval, const std::shared_ptr<ExprAST>& rval) : lhs(lval), rhs(rval) {}

        genValue codegen(genContext context) override;
    };
}

#endif
