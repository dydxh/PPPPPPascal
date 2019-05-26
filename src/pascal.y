%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {yapc}
%define api.parser.class {Parser}
%define api.value.type variant
%locations

%code requires {
    #include <iostream>
    #include <string>
    #include <stdexcept>
    namespace yapc {
        class Driver;
        class Scanner;
    }
}

%parse-param {Scanner& scanner} {Driver& driver}

%code {
    #include "Driver.hpp"
    #undef yylex
    #define yylex scanner.yylex
    using namespace yapc;
}

%define parse.error verbose
%define parse.assert

%token TERMINATE 0 "end of file"
%token PROGRAM IDENTIFIER CONST TYPE RECORD ARRAY VAR FUNCTION PROCEDURE PBEGIN END
%token TRUE FALSE DIGSEQ SIGNEDDIGSEQ LITERAL REALNUMBER SIGNEDREALNUMBER SYS_TYPE SYS_FUNC
%token IF THEN ELSE WHILE FOR REPEAT UNTIL DO TO DOWNTO OF
%token AND OR XOR NOT MINUS PLUS MUL DIV MOD SLASH ASSIGNMENT LE GE LT GT EQUAL NOTEQUAL
%token DOT DOTDOT STAR STARSTAR SEMICOLON COLON COMMA
%token LBRAC RBRAC LPAREN RPAREN

%nonassoc ELSE
%nonassoc NOELSE

%start program

%%
program: program_header SEMICOLON proc_block DOT TERMINATE
    ;

program_header: PROGRAM IDENTIFIER
    ;

proc_block: const_decl_part type_decl_part var_decl_part subproc_decl_part compound_part
    ;

const_decl_part: CONST const_decl_list
    |
    ;

const_decl_list: const_decl_list const_decl
    | const_decl
    ;

const_decl: IDENTIFIER EQUAL const_value SEMICOLON
    ;

type_decl_part: TYPE type_decl_list
    |
    ;

type_decl_list: type_decl_list type_decl
    | type_decl
    ;

type_decl: IDENTIFIER EQUAL type_denoter SEMICOLON
    ;

type_denoter: common_type
    | array_type
    | record_type
    ;

common_type: IDENTIFIER
    | SYS_TYPE
    ;

array_type: ARRAY LBRAC signed_integer DOTDOT signed_integer RBRAC OF type_denoter
    | ARRAY LBRAC IDENTIFIER DOTDOT IDENTIFIER RBRAC OF type_denoter
    ;

record_type: RECORD field_decl_list END
    ;

field_decl_list: field_decl_list field_decl
    | field_decl
    ;

field_decl: name_list COLON type_denoter SEMICOLON
    ;

name_list: name_list COMMA IDENTIFIER
    | IDENTIFIER
    ;

var_decl_part: VAR var_decl_list
    |
    ;

var_decl_list: var_decl_list var_decl
    | var_decl
    ;

var_decl: name_list COLON type_denoter SEMICOLON
    ;

subproc_decl_part: subproc_decl_list
    |
    ;

subproc_decl_list: subproc_decl_list subproc_decl
    | subproc_decl
    ;

subproc_decl: procdure_decl
    | func_decl
    ;

procdure_decl: procedure_header SEMICOLON proc_block SEMICOLON
    ;

procedure_header: PROCEDURE IDENTIFIER
    | PROCEDURE IDENTIFIER formal_param_part
    ;

formal_param_part: LPAREN param_decl_list RPAREN
    ;

param_decl_list: param_decl_list SEMICOLON param_decl
    | param_decl
    ;

param_decl: name_list COLON type_denoter
    ;

func_decl: function_header SEMICOLON proc_block SEMICOLON
    ;

function_header: FUNCTION IDENTIFIER COLON type_denoter
    | FUNCTION IDENTIFIER formal_param_part COLON type_denoter
    ;

compound_part: PBEGIN stmt_list END
    ;

stmt_list: stmt_list stmt SEMICOLON
    |
    ;

stmt: assignment_stmt
    | repeat_stmt
    | call_stmt
    | if_stmt
    | while_stmt
    | for_stmt
    | compound_part
    ;


assignment_stmt: IDENTIFIER ASSIGNMENT expression
    | IDENTIFIER LBRAC expression RBRAC ASSIGNMENT expression
    | IDENTIFIER DOT IDENTIFIER ASSIGNMENT expression
    ;

repeat_stmt: REPEAT stmt_list UNTIL expression
    ;

while_stmt: WHILE expression DO stmt
    ;

for_stmt: FOR IDENTIFIER ASSIGNMENT expression direction expression DO stmt
    ;

direction: TO
    | DOWNTO
    ;

if_stmt: IF expression THEN stmt NOELSE
    | IF expression THEN stmt ELSE stmt
    ;

call_stmt: IDENTIFIER params
    | SYS_FUNC params
    ;

params: LPAREN parameter_list RPAREN
    | LPAREN RPAREN
    | 
    ;

parameter_list: parameter_list COMMA expression
    | expression
    ;

expression: expr
    | expression relop expr
    ;

expr: term addop expr
    | term
    ;

term: factor
    | term mulop factor
    ;

factor: primary
    | primary STARSTAR factor
    ;

primary: IDENTIFIER
    | IDENTIFIER LBRAC expression RBRAC
    | IDENTIFIER DOT IDENTIFIER
    | IDENTIFIER LPAREN RPAREN
    | IDENTIFIER LPAREN arg_list RPAREN
    | SYS_FUNC LPAREN arg_list RPAREN
    | const_value
    | LPAREN expression RPAREN
    | NOT primary
    | sign primary
    ;

arg_list: arg_list COMMA expression
    | expression

const_value: signed_number
    | LITERAL
    | TRUE
    | FALSE
    ;

signed_number: signed_integer
    | signed_real
    ;

signed_integer: DIGSEQ
    | SIGNEDDIGSEQ
    ;

signed_real: SIGNEDREALNUMBER
    | REALNUMBER
    ;

sign: PLUS
    | MINUS
    ;

addop: PLUS
    | MINUS
    | OR
    | XOR
    ;

mulop: STAR
    | MUL
    | SLASH
    | DIV
    | MOD
    | AND
    ;

relop: EQUAL
    | NOTEQUAL
    | LT
    | GT
    | LE
    | GE
    ;

%%

void yapc::Parser::error(const location_type &loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
    throw std::logic_error("Syntax error: invalid syntax");
}