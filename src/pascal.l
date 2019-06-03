%{
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <memory>
#include "Scanner.hpp"
#include "AST/basicast.hpp"

#undef YY_DECL
#define YY_DECL int yapc::Scanner::yylex(yapc::Parser::semantic_type* lval, yapc::Parser::location_type* loc)

using token = yapc::Parser::token;

#define yyterminate() return token::TERMINATE;

#define YY_NO_UNISTD_H

#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option nodefault
%option c++
%option yyclass="yapc::Scanner"
%option noyywrap
%option caseless
%option stack

NQUOTE [^']

%%
%{
    yylval = lval;
%}

"AND"       {std::cout << yytext;  return token::AND;}
"ARRAY"     {std::cout << yytext;  return token::ARRAY;}
"CONST"     {std::cout << yytext;  return token::CONST;}
"DIV"       {std::cout << yytext;  return token::DIV;}
"MUL"       {std::cout << yytext;  return token::MUL;}
"DO"        {std::cout << yytext;  return token::DO;}
"DOWNTO"    {std::cout << yytext;  return token::DOWNTO;}
"ELSE"      {std::cout << yytext;  return token::ELSE;}
"END"       {std::cout << yytext;  return token::END;}
"FOR"       {std::cout << yytext;  return token::FOR;}
"FUNCTION"  {std::cout << yytext;  return token::FUNCTION;}
"IF"        {std::cout << yytext;  return token::IF;}
"MOD"       {std::cout << yytext;  return token::MOD;}
"NOT"       {std::cout << yytext;  return token::NOT;}
"OF"        {std::cout << yytext;  return token::OF;}
"OR"        {std::cout << yytext;  return token::OR;}
"XOR"       {std::cout << yytext;  return token::XOR;}
"BEGIN"     {std::cout << yytext;  return token::PBEGIN;}
"PROCEDURE" {std::cout << yytext;  return token::PROCEDURE;}
"PROGRAM"   {std::cout << yytext;  return token::PROGRAM;}
"REPEAT"    {std::cout << yytext;  return token::REPEAT;}
"THEN"      {std::cout << yytext;  return token::THEN;}
"TO"        {std::cout << yytext;  return token::TO;}
"TYPE"      {std::cout << yytext;  return token::TYPE;}
"UNTIL"     {std::cout << yytext;  return token::UNTIL;}
"VAR"       {std::cout << yytext;  return token::VAR;}
"WHILE"     {std::cout << yytext;  return token::WHILE;}
"TRUE"      {std::cout << yytext; yylval->build<bool>(true);  return token::TRUE;}
"FALSE"     {std::cout << yytext; yylval->build<bool>(false); return token::FALSE;}
"RECORD"    {std::cout << yytext;  return token::RECORD;}
"WRITE"     {std::cout << yytext; yylval->build<SysFunc>(SysFunc::WRITE); return token::SYS_FUNC;}
"WRITELN"   {std::cout << yytext; yylval->build<SysFunc>(SysFunc::WRITELN); return token::SYS_FUNC;}
"READ"      {std::cout << yytext; yylval->build<SysFunc>(SysFunc::READ); return token::SYS_FUNC;}
"READLN"    {std::cout << yytext; yylval->build<SysFunc>(SysFunc::READLN); return token::SYS_FUNC;}
"INTEGER"   {std::cout << yytext; yylval->build<std::shared_ptr<PrimaryTypeAST>>(MakeAST<PrimaryTypeAST>(Type::INTEGER)); return token::SIMPLE_TYPE;}
"LONGINT"   {std::cout << yytext; yylval->build<std::shared_ptr<PrimaryTypeAST>>(MakeAST<PrimaryTypeAST>(Type::INTEGER)); return token::SIMPLE_TYPE;}
"BOOLEAN"   {std::cout << yytext; yylval->build<std::shared_ptr<PrimaryTypeAST>>(MakeAST<PrimaryTypeAST>(Type::BOOLEAN)); return token::SIMPLE_TYPE;}
"REAL"      {std::cout << yytext; yylval->build<std::shared_ptr<PrimaryTypeAST>>(MakeAST<PrimaryTypeAST>(Type::REAL)); return token::SIMPLE_TYPE;}
"STRING"    {std::cout << yytext; yylval->build<std::shared_ptr<StringTypeAST>>(MakeAST<StringTypeAST>()); return token::STRING;}
[+|-][0-9]+      {std::cout << "Sign Integer: " << yytext; yylval->build<int>(std::stoi(yytext)); return token::SIGNEDDIGSEQ;}
[0-9]+      {yylval->build<int>(std::stoi(yytext)); std::cout << "Integer: " << yytext; return token::DIGSEQ;}
[+|-][0-9]+"."[0-9]+   {std::cout << "Sign Real Number: " << yytext; yylval->build<double>(std::stod(yytext)); return token::SIGNEDREALNUMBER;}
[0-9]+"."[0-9]+   {std::cout << "Real Number: " << yytext; yylval->build<double>(std::stod(yytext)); return token::REALNUMBER;}
'({NQUOTE}|'')+'  {std::cout << "STRING: " << yytext; yytext[yyleng] = 0; yylval->build<std::string>(std::string(yytext)); return token::LITERAL;}
[a-zA-Z]([a-zA-Z0-9])*  {std::cout << "IDD: " << yytext << " "; yytext[yyleng] = 0; yylval->build<std::string>(std::string(yytext)); return token::IDENTIFIER;}

"(*" {
    char c;
    while(c = yyinput()) {
        if(c == '*') {
            if((c = yyinput()) == ')')
                break;
            else unput(c);
        }
        else if(c == '\n') {
            loc->lines();
        }
        else if(c == EOF) {
            CommentError(loc);
        }
    }
}
"{" {
    char c;
    while(c = yyinput()) {
        if(c == '}') break;
        else if(c == '\n') {
            CommentError(loc);
            loc->lines();
        }
        else if(c == EOF) {
            CommentError(loc);
        }
    }
}
"//" {
    char c;
    while(c = yyinput()) {
        if(c == '\n') {
            loc->lines();
            break;
        }
        else if(c == EOF) {
            break;
        }
    }
}

":="        {std::cout << yytext;  return token::ASSIGNMENT;}
":"         {std::cout << yytext;  return token::COLON;}
","         {std::cout << yytext; return token::COMMA;}
"."         {std::cout << yytext;  return token::DOT;}
".."        {std::cout << yytext;  return token::DOTDOT;}
"="         {std::cout << yytext;  return token::EQUAL;}
">="        {std::cout << yytext;  return token::GE;}
">"         {std::cout << yytext;  return token::GT;}
"["         {std::cout << yytext;  return token::LBRAC;}
"<="        {std::cout << yytext;  return token::LE;}
"("         {std::cout << yytext;  return token::LPAREN;}
"<"         {std::cout << yytext;  return token::LT;}
"-"         {std::cout << yytext;  return token::MINUS;}
"<>"        {std::cout << yytext;  return token::NOTEQUAL;}
"+"         {std::cout << yytext;  return token::PLUS;}
"]"         {std::cout << yytext;  return token::RBRAC;}
")"         {std::cout << yytext;  return token::RPAREN;}
";"         {std::cout << yytext;  return token::SEMICOLON;}
"/"         {std::cout << yytext;  return token::SLASH;}
"*"         {std::cout << yytext;  return token::STAR;}
"**"        {std::cout << yytext;  return token::STARSTAR;}

[ \t\f]    {std::cout << ' '; continue;}

[\n\r]     {std::cout << std::endl; loc->lines();}

.          {
                std::cerr << "Scanner: Error at " << *loc << ": " << std::endl;
                throw std::logic_error("Scanner: Invalid character\n");
           }

%%

void Scanner::CommentError(yapc::Parser::location_type* loc) {
    std::cerr << "Unexcepted EOF in comment at line: " << loc->end.line << std::endl;
    throw std::runtime_error("EOF error: Unexcepted EOF in comment");
}