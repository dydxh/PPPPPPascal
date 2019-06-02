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
        of << "child { node {Statment}}\n";
    }
    return lines;
}