#include "ASTvis.hpp"

#include <fstream>
#include <iostream>
#include <string>

void yapc::ASTvis::travAST(yapc::Context& context)
{
    std::fstream s(filename, s.trunc | s.out);
    if (!s.is_open()) { std::cout << "failed to open " << filename << '\n'; return;}

    s << texHeader;
    travProgram( context.program, s);
    s << texTail;

    std::cout << "debug info:\n" << context.program->proghead->name->GetName() << std::endl;

    return;
}

void yapc::ASTvis::travProgram(const std::shared_ptr<yapc::ProgramAST>& program, std::fstream& s )
{
    s << "\\node (program00) [program] {Program: " << program->proghead->name->GetName() << "} [->]\n";
    travCONST();
    travTYPE();
    travVAR();
    travSubproc();
    travCompound();
}

void yapc::ASTvis::travCONST(){}
void yapc::ASTvis::travTYPE(){}
void yapc::ASTvis::travVAR(){}
void yapc::ASTvis::travSubproc(){}
void yapc::ASTvis::travCompound(){}