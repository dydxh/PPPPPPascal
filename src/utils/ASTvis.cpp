#include "ASTvis.hpp"

#include <fstream>
#include <iostream>
#include <string>

void yapc::ASTvis::travAST(yapc::Context& context)
{

    of << texHeader;
    travProgram( context.program);
    of << texTail;

    std::cout << "debug info:\n" << context.program->proghead->name->GetName() << std::endl;
    std::cout << "\n\n>>>>>>>>>>>>>>>==========  AST over!==========<<<<<<<<<<<<<<<" << std::endl;

    return;
}

int yapc::ASTvis::travProgram(const std::shared_ptr<yapc::ProgramAST>& program)
{
    of << "\\node {Program: " << program->proghead->name->GetName() << "}\n";
    return travProgramBody(program);
}

int yapc::ASTvis::travProgramBody(const std::shared_ptr<yapc::ProgramAST>& program)
{
    int tmp = 0, lines = 6;
    of << "child { node {CONST}";
    tmp = travCONST(program->progblock->constpart);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;

    of << "child { node {TYPE}";
    tmp = travTYPE(program->progblock->typepart);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;

    of << "child { node {VAR}";
    tmp = travVAR(program->progblock->varpart);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;

    of << "child { node {PROC or FUNC}";
    tmp = travSubprocList(program->progblock->progpart);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;

    of << "child { node {STMT}";
    tmp = travCompound(program->progblock->progbody);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;

    return lines;
}

int yapc::ASTvis::travCONST(const std::shared_ptr<yapc::ConstDeclListAST>& const_declListAST)
{
    std::list<std::shared_ptr<ConstDeclAST>>& constList(const_declListAST->get_children());
    int lines = constList.size();

    for (auto &p : constList) {
        of << "child { node {";
        of << p->name->GetName() << " : ";
        switch (p->value->type) {
            case yapc::Type::VOID    : of << "VOID"   ; break;
            case yapc::Type::ARRAY   : of << "ARRAY"  ; break;
            case yapc::Type::RECORD  : of << "RECORD" ; break;
            case yapc::Type::BOOLEAN : of << "BOOLEAN"; break;
            case yapc::Type::INTEGER : of << "INTEGER"; break;
            case yapc::Type::LONGINT : of << "LONGINT"; break;
            case yapc::Type::REAL    : of << "REAL"   ; break;
            case yapc::Type::STRING  : of << "STRING" ; break;
            default : of << "ERROR"; break;
        }
        of << "}}\n";
    }
    return lines;
}

int yapc::ASTvis::travTYPE(const std::shared_ptr<yapc::TypeDeclListAST>& type_declListAST)
{
    std::list<std::shared_ptr<TypeDeclAST>>& typeList(type_declListAST->get_children());
    int lines = typeList.size();

    for (auto &p : typeList) {
        of << "child { node {";
        of << p->name->GetName() << " : ";
        switch (p->type->type) {
            case yapc::Type::VOID    : of << "VOID"   ; break;
            case yapc::Type::ARRAY   : of << "ARRAY"  ; break;
            case yapc::Type::RECORD  : of << "RECORD" ; break;
            case yapc::Type::BOOLEAN : of << "BOOLEAN"; break;
            case yapc::Type::INTEGER : of << "INTEGER"; break;
            case yapc::Type::LONGINT : of << "LONGINT"; break;
            case yapc::Type::REAL    : of << "REAL"   ; break;
            case yapc::Type::STRING  : of << "STRING" ; break;
            default : of << "ERROR"; break;
        }
        of << "}}\n";
    }
    return lines;
}
int yapc::ASTvis::travVAR(const std::shared_ptr<yapc::VarDeclListAST>& var_declListAST)
{
    std::list<std::shared_ptr<VarDeclAST>>& varList(var_declListAST->get_children());
    int lines = varList.size();

    for (auto &p : varList) {
        of << "child { node {";
        of << p->name->GetName() << " : ";
        switch (p->type->type) {
            case yapc::Type::VOID    : of << "VOID"   ; break;
            case yapc::Type::ARRAY   : of << "ARRAY"  ; break;
            case yapc::Type::RECORD  : of << "RECORD" ; break;
            case yapc::Type::BOOLEAN : of << "BOOLEAN"; break;
            case yapc::Type::INTEGER : of << "INTEGER"; break;
            case yapc::Type::LONGINT : of << "LONGINT"; break;
            case yapc::Type::REAL    : of << "REAL"   ; break;
            case yapc::Type::STRING  : of << "STRING" ; break;
            default : of << "ERROR"; break;
        }
        of << "}}\n";
    }
    return lines;
}

int yapc::ASTvis::travSubprocList(const std::shared_ptr<yapc::ProgListAST>& subProc_declListAST)
{
    std::list<std::shared_ptr<yapc::ProgramAST>>& progList(subProc_declListAST->get_children());
    int tmp = 0, lines = progList.size();

    for (auto &p : progList) {
        tmp = travSubproc(p);
        for (int i=0; i<tmp; ++i) of << texNone;
        lines += tmp;
    }
    return lines;
}

int yapc::ASTvis::travSubproc(const std::shared_ptr<yapc::ProgramAST>& subProc_AST)
{
    int lines = 0;
    of << "child { node {";
    if (subProc_AST->proghead->type->type == yapc::Type::VOID )
        of << "PROCEDURE: " << subProc_AST->proghead->name->GetName();
    else
    {
        of << "FUNCTION: " << subProc_AST->proghead->name->GetName();
        of << "$ ---- $RET$-$TYPE: ";
        switch (subProc_AST->proghead->type->type) {
            case yapc::Type::VOID    : of << "VOID"   ; break;
            case yapc::Type::ARRAY   : of << "ARRAY"  ; break;
            case yapc::Type::RECORD  : of << "RECORD" ; break;
            case yapc::Type::BOOLEAN : of << "BOOLEAN"; break;
            case yapc::Type::INTEGER : of << "INTEGER"; break;
            case yapc::Type::LONGINT : of << "LONGINT"; break;
            case yapc::Type::REAL    : of << "REAL"   ; break;
            case yapc::Type::STRING  : of << "STRING" ; break;
            default : of << "ERROR"; break;
        }
    }

    std::list<std::shared_ptr<ParamAST>>& paramAsts
            = subProc_AST->proghead->params->get_children();
    {
        of << "$ ---- $PARAMS: ";
        for (auto &p : paramAsts) {
            of << p->name->GetName() << " $-$ ";
            switch (p->type->type) {
                case yapc::Type::VOID    : of << "VOID"   ; break;
                case yapc::Type::ARRAY   : of << "ARRAY"  ; break;
                case yapc::Type::RECORD  : of << "RECORD" ; break;
                case yapc::Type::BOOLEAN : of << "BOOLEAN"; break;
                case yapc::Type::INTEGER : of << "INTEGER"; break;
                case yapc::Type::LONGINT : of << "LONGINT"; break;
                case yapc::Type::REAL    : of << "REAL"   ; break;
                case yapc::Type::STRING  : of << "STRING" ; break;
                default : of << "ERROR"; break;
            }
        }
    }
    of << "}";
    lines = travProgramBody(subProc_AST);
    of << "}\n";
    return lines;
}

int yapc::ASTvis::travCompound(const std::shared_ptr<yapc::CompoundStmtAST>& compound_declListAST)
{
    std::list<std::shared_ptr<yapc::StmtAST>>& stmtList(compound_declListAST->get_children());
    int tmp = 0, lines = stmtList.size();
    for (auto &p : stmtList) {
        tmp = 0;
        tmp += travStmt(std::dynamic_pointer_cast<yapc::IfStmtAST>(p));
        tmp += travStmt(std::dynamic_pointer_cast<yapc::WhileStmtAST>(p));
        tmp += travStmt(std::dynamic_pointer_cast<yapc::ForStmtAST>(p));
        tmp += travStmt(std::dynamic_pointer_cast<yapc::RepeatStmtAST>(p));
        tmp += travStmt(std::dynamic_pointer_cast<yapc::CallStmtAST>(p));
        tmp += travStmt(std::dynamic_pointer_cast<yapc::AssignStmtAST>(p));
        lines += tmp;
        std::cout << typeid(*p).name() << std::endl;
    }
    return lines;
}

int yapc::ASTvis::travStmt(const std::shared_ptr<yapc::StmtAST>&p_stmp)
{
    if (p_stmp == nullptr) return 0;
    of << "child { node {Base Statment}}\n";
    return 0;
}
// * done
int yapc::ASTvis::travStmt(const std::shared_ptr<yapc::IfStmtAST>&p_stmp)
{
    if (p_stmp == nullptr) return 0;
    int tmp = 0, lines = 0;
    of << "child { node {IF Statment if expr}\n";
    tmp = travExpr(p_stmp->expr);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp; tmp = 0;
    of << "child { node {IF Statment if stmt}\n";
    tmp = travCompound(p_stmp->stmt);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp; tmp = 0;
    of << "child { node {IF Statment else stmt}\n";
    tmp = travCompound(p_stmp->else_stmt);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp; tmp = 0;

    return lines;
}
int yapc::ASTvis::travStmt(const std::shared_ptr<yapc::WhileStmtAST>&p_stmp)
{
    if (p_stmp == nullptr) return 0;
    int tmp = 0, lines = 0;
    of << "child { node {WHILE Expr}";
    tmp = travExpr(p_stmp->expr);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp; tmp = 0;
    of << "child { node {WHILE Statment}\n";
    tmp = travCompound(p_stmp->stmt);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp; tmp = 0;
    return lines;
}
int yapc::ASTvis::travStmt(const std::shared_ptr<yapc::ForStmtAST>&p_stmp)
{
    if (p_stmp == nullptr) return 0;
    int tmp = 0, lines = 0;
    of << "child { node {FOR Expr: ";
    of << p_stmp->identifier->GetName() << " }\n ";

    tmp += travExpr(p_stmp->start_val);
    // of << texNone;
    tmp += travExpr(p_stmp->end_val);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp; tmp = 0;

    of << "child { node {FOR Statment}\n";
    tmp = travCompound(p_stmp->stmt);
    of << "}\n";
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;

    return lines;
}
int yapc::ASTvis::travStmt(const std::shared_ptr<yapc::RepeatStmtAST>&p_stmp)
{
    if (p_stmp == nullptr) return 0;
    int tmp = 0, lines = 0;
    of << "child { node {REPEAT Statment";
    // tmp = travExpr(p_stmp->expr);
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;    of << "}\n}\n";
    return lines;
}
int yapc::ASTvis::travStmt(const std::shared_ptr<yapc::CallStmtAST>&p_stmp)
{
    if (p_stmp == nullptr) return 0;
    int tmp = 0, lines = 0;
    of << "child { node {CALL Statment";
    // tmp = travExpr(p_stmp->expr);
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;
    of << "}\n}\n";
    return lines;
}
int yapc::ASTvis::travStmt(const std::shared_ptr<yapc::AssignStmtAST>&p_stmp)
{
    if (p_stmp == nullptr) return 0;
    int tmp = 0, lines = 0;
    of << "child { node {ASSIGN Statment";
    // tmp = travExpr(p_stmp->expr);
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;
    of << "}\n}\n";
    return lines;
}

int yapc::ASTvis::travExpr(const std::shared_ptr<ExprAST>& expr)
{
    int tmp = 0, lines = 0;
    tmp += travExpr(std::dynamic_pointer_cast<yapc::BinaryExprAST>(expr));
    tmp += travExpr(std::dynamic_pointer_cast<yapc::UnaryExprAST>(expr));
    tmp += travExpr(std::dynamic_pointer_cast<yapc::ArrayAccessAST>(expr));
    tmp += travExpr(std::dynamic_pointer_cast<yapc::RecordAccessAST>(expr));
    tmp += travExpr(std::dynamic_pointer_cast<yapc::FuncCallAST>(expr));
    tmp += travExpr(std::dynamic_pointer_cast<yapc::CustomFuncAST>(expr));
    tmp += travExpr(std::dynamic_pointer_cast<yapc::SysFuncAST>(expr));
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;
    return lines;
}

int yapc::ASTvis::travExpr(const std::shared_ptr<BinaryExprAST>& expr)
{
    if (expr == nullptr) return 0;
    int tmp = 0, lines = 1;

    of << "child { node {BINARY: ";
    switch (expr->op)
    {
        case yapc::BinaryOp::EQ: of << "==";break;
        case yapc::BinaryOp::NE: of << "!=";break;
        case yapc::BinaryOp::LE: of << "<=";break;
        case yapc::BinaryOp::GE: of << ">=";break;
        case yapc::BinaryOp::LT: of << "<";break;
        case yapc::BinaryOp::GT: of << ">";break;
        case yapc::BinaryOp::ADD: of << "+";break;
        case yapc::BinaryOp::SUB: of << "-";break;
        case yapc::BinaryOp::DIV: of << "/";break;
        case yapc::BinaryOp::SLASH: of << "//";break;
        case yapc::BinaryOp::MOD: of << "\%";break;
        case yapc::BinaryOp::MUL: of << "*";break;
        case yapc::BinaryOp::OR:  of << "|";break;
        case yapc::BinaryOp::AND: of << "&";break;
        case yapc::BinaryOp::XOR: of << "\^";break;
        case yapc::BinaryOp::POW: of << "**";break;
        default: of << "????";break;
    }
    of << "}\n";
    tmp += travExpr(std::dynamic_pointer_cast<yapc::BinaryExprAST>(expr->lhs));
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp; tmp = 0;
    tmp += travExpr(std::dynamic_pointer_cast<yapc::BinaryExprAST>(expr->rhs));
    for (int i=0; i<tmp; ++i) of << texNone;
    lines += tmp;
    of << "}\n";

    return lines;
}
int yapc::ASTvis::travExpr(const std::shared_ptr<yapc::UnaryExprAST>& expr)
{
    if (expr == nullptr) return 0;
    int tmp = 0, lines = 1;
    return lines;
}
int yapc::ASTvis::travExpr(const std::shared_ptr<yapc::ArrayAccessAST>& expr)
{
    if (expr == nullptr) return 0;
    int tmp = 0, lines = 1;
    return lines;
}
int yapc::ASTvis::travExpr(const std::shared_ptr<yapc::RecordAccessAST>& expr)
{
    if (expr == nullptr) return 0;
    int tmp = 0, lines = 1;
    return lines;
}
int yapc::ASTvis::travExpr(const std::shared_ptr<yapc::FuncCallAST>& expr)
{
    if (expr == nullptr) return 0;
    int tmp = 0, lines = 1;
    return lines;
}
int yapc::ASTvis::travExpr(const std::shared_ptr<yapc::CustomFuncAST>& expr)
{
    if (expr == nullptr) return 0;
    int tmp = 0, lines = 1;
    return lines;
}
int yapc::ASTvis::travExpr(const std::shared_ptr<yapc::SysFuncAST>& expr)
{
    if (expr == nullptr) return 0;
    int tmp = 0, lines = 1;
    return lines;
}
