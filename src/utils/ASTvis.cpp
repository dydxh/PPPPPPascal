#include "ASTvis.hpp"

#include <fstream>
#include <iostream>
#include <string>

void yapc::ASTvis::travAST(yapc::Context& context)
{
    std::fstream s(filename, s.trunc | s.out);
    if (!s.is_open()) { std::cout << "failed to open " << filename << '\n'; return;}


    s << texHeader << "\\node (State00) [state] {$S_{00}$} [->]\n" << texTail;


    s.seekp(0);
    std::string str;
    if (s >> str)
        std::cout << "read back from file: " << str << '\n';

    std::cout << "debug info:\n" << texTail << std::endl;

    return;
}