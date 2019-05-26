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
%token AND ARRAY ASSIGNMENT LITERAL COLON COMMA CONST TRUE FALSE
%token DIV DO DOT DOTDOT DOWNTO ELSE EQUAL FOR FUNCTION
%token GE GT IDENTIFIER END IF IN LBRAC LE LPAREN LT MINUS MOD NOT
%token NOTEQUAL OF OR PBEGIN PLUS PROCEDURE PROGRAM RBRAC
%token REALNUMBER RECORD REPEAT RPAREN SEMICOLON SLASH THEN
%token TO TYPE UNTIL VAR WHILE STAR STARSTAR DIGSEQ FORWARD EXTERNAL

%start program

%%
program: program_heading SEMICOLON block DOT TERMINATE
    ;

program_heading: PROGRAM IDENTIFIER
    ;

identifier_list: identifier_list COMMA IDENTIFIER
    | IDENTIFIER
    ;

block: constant_definition_part
       variable_declaration_part
       procedure_and_function_declaration_part
       compound_statement
    ;
constant_definition_part: CONST constant_list
    |
    ;

constant_list: constant_list constant_definition
    | constant_definition
    ;

constant_definition: IDENTIFIER EQUAL signed_const SEMICOLON
    ;

signed_const: sign_integer
    | sign_real
    ;

sign: PLUS
    | MINUS
    ;

type_denoter: IDENTIFIER
    | array_type
    ;

array_type: ARRAY LBRAC sign_integer DOTDOT sign_integer RBRAC OF type_denoter
    ;

sign_integer: DIGSEQ
    | sign DIGSEQ
    ;

sign_real: REALNUMBER
    | sign REALNUMBER

variable_declaration_part: VAR variable_declaration_list SEMICOLON
    |
    ;

variable_declaration_list:
   variable_declaration_list SEMICOLON variable_declaration
    | variable_declaration
    ;

variable_declaration: identifier_list COLON type_denoter
    ;

procedure_and_function_declaration_part: proc_or_func_declaration_list SEMICOLON
    |
    ;

proc_or_func_declaration_list: proc_or_func_declaration_list SEMICOLON proc_or_func_declaration
    | proc_or_func_declaration
    ;

proc_or_func_declaration: procedure_declaration
    | function_declaration
    ;

procedure_declaration: procedure_header SEMICOLON block
    ;

procedure_header: PROCEDURE IDENTIFIER
    | PROCEDURE IDENTIFIER formal_parameter_list
    ;

formal_parameter_list: LPAREN parameter_decl_list RPAREN
    ;

parameter_decl_list: parameter_decl_list SEMICOLON param_decl
    | param_decl
    ;

param_decl: identifier_list COLON type_denoter
    ;

function_declaration: function_header SEMICOLON block
    ;

function_header: FUNCTION IDENTIFIER COLON type_denoter
    | FUNCTION IDENTIFIER formal_parameter_list COLON type_denoter
    ;

compound_statement: PBEGIN statement_sequence END
    ;

statement_sequence: statement_sequence SEMICOLON statement
    | statement
    ;

statement: open_statement
    | closed_statement
    ;

closed_statement: assignment_statement
    | call_statement
    | compound_statement
    | repeat_statement
    | closed_if_statement
    | closed_while_statement
    | closed_for_statement
    |
    ;

open_statement: open_if_statement
    | open_while_statement
    | open_for_statement
    ;

repeat_statement: REPEAT statement_sequence UNTIL expression
    ;

open_while_statement: WHILE expression DO open_statement
    ;

closed_while_statement: WHILE expression DO closed_statement
    ;

open_for_statement: FOR IDENTIFIER ASSIGNMENT initial_value direction final_value DO open_statement
    ;

closed_for_statement: FOR IDENTIFIER ASSIGNMENT initial_value direction final_value DO closed_statement
    ;

initial_value: expression;

direction: TO
    | DOWNTO
    ;

final_value: expression;

open_if_statement: IF expression THEN statement
    | IF expression THEN closed_statement ELSE open_statement
    ;

closed_if_statement: IF expression THEN closed_statement ELSE closed_statement
    ;

assignment_statement: variable_access ASSIGNMENT expression
    ;

variable_access: IDENTIFIER
    | indexed_variable
    ;

indexed_variable: variable_access LBRAC index_expression_list RBRAC
    ;

index_expression_list: index_expression_list COMMA index_expression
    | index_expression
    ;

index_expression: expression;

call_statement: IDENTIFIER params;

params: LPAREN actual_parameter_list RPAREN
    | LPAREN RPAREN
    ;

actual_parameter_list: actual_parameter_list COMMA expression
    | expression
    ;

expression: simple_expression
    | expression relop simple_expression
    ;

simple_expression: sign term
    | term
    | sign term addop simple_expression
    | term addop simple_expression
    ;

term: exponentiation
    | term mulop exponentiation
    ;

exponentiation: primary
    | primary STARSTAR exponentiation
    ;

primary: variable_access
    | unsigned_constant
    | call_statement
    | LPAREN expression RPAREN
    | NOT primary
    ;

unsigned_constant: unsigned_number
    | FALSE
    | TRUE
    | LITERAL;

unsigned_number: unsigned_integer | unsigned_real;

unsigned_integer: DIGSEQ;

unsigned_real: REALNUMBER;

addop: PLUS
    | MINUS
    | OR
    ;

mulop: STAR
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
    | IN
    ;

%%

void yapc::Parser::error(const location_type &loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
    throw std::logic_error("Syntax error: invalid syntax");
}