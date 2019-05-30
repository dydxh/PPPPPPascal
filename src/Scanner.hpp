#ifndef yapc_scanner_header
#define yapc_scanner_header

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include "Parser.hpp"
#include "location.hh"


namespace yapc {

class Scanner: public yyFlexLexer {
public:
    Scanner(std::istream* in) : yyFlexLexer(in) {}
    virtual ~Scanner() {}

    using FlexLexer::yylex;
    virtual int yylex(yapc::Parser::semantic_type* lval, yapc::Parser::location_type* loc);

private:
    yapc::Parser::semantic_type* yylval = nullptr;
};

}
#endif