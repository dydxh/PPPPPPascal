%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {yapc}
%define api.parser.class {Parser}
%define api.value.type variant
%locations

%code requires {
    #include "AST/expr.hpp"
    #include <iostream>
    #include <memory>
    #include <string>
    #include <stdexcept>
    using namespace yapc;
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
%token TRUE FALSE DIGSEQ SIGNEDDIGSEQ LITERAL REALNUMBER SIGNEDREALNUMBER SIMPLE_TYPE STRING SYS_FUNC
%token IF THEN ELSE WHILE FOR REPEAT UNTIL DO TO DOWNTO OF
%token AND OR XOR NOT MINUS PLUS MUL DIV MOD SLASH ASSIGNMENT LE GE LT GT EQUAL NOTEQUAL
%token DOT DOTDOT STAR STARSTAR SEMICOLON COLON COMMA
%token LBRAC RBRAC LPAREN RPAREN

%type <int> SIGNEDDIGSEQ DIGSEQ
%type <double> SIGNEDREALNUMBER REALNUMBER
%type <bool> TRUE FALSE
%type <std::string> LITERAL
%type <std::unique_ptr<ConstAST>> const_value signed_number
%type <std::unique_ptr<IntegerAST>> signed_integer
%type <std::unique_ptr<RealAST>> signed_real
%type <UnaryOp> sign
%type <BinaryOp> relop addop mulop

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

type_denoter: IDENTIFIER
    | SIMPLE_TYPE
    | STRING
    | array_type
    | record_type
    ;

array_type: ARRAY LBRAC signed_integer DOTDOT signed_integer RBRAC OF SIMPLE_TYPE {
        $$ = MakeAST<ArrayTypeAST>($3, $5, $8);
    }
    | ARRAY LBRAC IDENTIFIER DOTDOT IDENTIFIER RBRAC OF SIMPLE_TYPE {
        $$ = MakeAST<ArrayTypeAST>(MakeAST<IdentifierAST>($3), MakeAST<IdentifierAST>($5), $8);
    }
    ;

record_type: RECORD field_decl_list END
    ;

field_decl_list: field_decl_list field_decl {
        $$ = $1;
        $$->MergeAST($2);
    }
    | field_decl {$$ = $1;}
    ;

field_decl: name_list COLON SIMPLE_TYPE SEMICOLON {
        $$ = MakeAST<TypeDeclListAST>();
        for(auto name : name_list)
            $$->AppendChild(MakeAST<TypeDeclAST>(name, $3));
    }
    ;

name_list: name_list COMMA IDENTIFIER {
        $$ = $1;
        $$->AppendChild(MakeAST<IdentifierAST>($3));
    }
    | IDENTIFIER {$$ = MakeAST<IdentifierListAST>(MakeAST<IdentifierAST>($1));}
    ;

var_decl_part: VAR var_decl_list {$$ = $1;}
    | {$$ = MakeAST<VarDeclListAST>();}
    ;

var_decl_list: var_decl_list var_decl {
        $$ = $1;
        $$->MergeAST($2->get_children());
    }
    | var_decl {$$ = $1;}
    ;

var_decl: name_list COLON type_denoter SEMICOLON {
        $$ = MakeAST<VarDeclListAST>();
        for(auto name : $1->get_children())
            $$->AppendChild(MakeAST<VarDeclAST>(name, $3));
    }
    ;

subproc_decl_part: subproc_decl_list {$$ = $1;}
    | {$$ = MakeAST<ProgramListAST>();}
    ;

subproc_decl_list: subproc_decl_list subproc_decl {
        $$ = $1;
        $$->AppendChild($2);
    }
    | subproc_decl {
        $$ = MakeAST<ProgramListAST>($1);
    }
    ;

subproc_decl: procdure_decl {$$ = $1;}
    | func_decl {$$ = $1;}
    ;

procdure_decl: procedure_header SEMICOLON proc_block SEMICOLON {
        $$ = MakeAST<ProgramAST>($1, $3);
    }
    ;

procedure_header: PROCEDURE IDENTIFIER {
        $$ = MakeAST<ProgHeadAST>(Type::VOID, MakeAST<IdentifierAST>($2), MakeAST<ParamListAST>());
    }
    | PROCEDURE IDENTIFIER formal_param_part {
        $$ = MakeAST<ProgHeadAST>(Type::VOID, MakeAST<IdentifierAST>($2), $3);
    }
    ;

formal_param_part: LPAREN param_decl_list RPAREN {$$ = $1;}
    ;

param_decl_list: param_decl_list SEMICOLON param_decl {
        $$ = $1;
        $$->MergeAST($3->get_children());
    }
    | param_decl {$$ = $1;}
    ;

param_decl: name_list COLON type_denoter {
        $$ = MakeAST<ParamListAST>();
        for(auto name : $1->get_children())
            $$->AppendChild(MakeAST<ParamAST>(name, $3));
    }
    ;

func_decl: function_header SEMICOLON proc_block SEMICOLON {
        $$ = MakeAST<ProgramAST>($1, $3);
    }
    ;

function_header: FUNCTION IDENTIFIER COLON SIMPLE_TYPE {
        $$ = MakeAST<ProgHeadAST>($4, MakeAST<IdentifierAST>($2), MakeAST<ParamListAST>());
    }
    | FUNCTION IDENTIFIER formal_param_part COLON SIMPLE_TYPE {
        $$ = MakeAST<ProgHeadAST>($5, MakeAST<IdentifierAST>($2), $3);
    }
    ;

compound_part: PBEGIN stmt_list END {$$ = $2;}
    ;

stmt_list: stmt_list stmt SEMICOLON {
        $$ = $1;
        $$->MergeAST($2->get_children());
    }
    | {$$ = MakeAST<CompoundStmtAST>();}
    ;

stmt: assignment_stmt {$$ = MakeAST<CompoundStmtAST>($1);}
    | repeat_stmt {$$ = MakeAST<CompoundStmtAST>($1);}
    | call_stmt {$$ = MakeAST<CompoundStmtAST>($1);}
    | if_stmt {$$ = MakeAST<CompoundStmtAST>($1);}
    | while_stmt {$$ = MakeAST<CompoundStmtAST>($1);}
    | for_stmt {$$ = MakeAST<CompoundStmtAST>($1);}
    | compound_part {$$ = $1;}
    ;


assignment_stmt: IDENTIFIER ASSIGNMENT expression {
        $$ = MakeAST<AssignStmtAST>(MakeAST<IdentifierAST>($1), $2);
    }
    | IDENTIFIER LBRAC expression RBRAC ASSIGNMENT expression {
        $$ = MakeAST<AssignStmtAST>(MakeAST<ArrayAccessAST>($1, $3), $6);
    }
    | IDENTIFIER DOT IDENTIFIER ASSIGNMENT expression {
        $$ = MakeAST<AssignStmtAST>(MakeAST<RecordAccessAST>($1, $3), $5);
    }
    ;

repeat_stmt: REPEAT stmt_list UNTIL expression {
        $$ = MakeAST<RepeatStmtAST>($4, $2);
    }
    ;

while_stmt: WHILE expression DO stmt {
        $$ = MakeAST<WhileStmtAST>($2, $4);
    }
    ;

for_stmt: FOR IDENTIFIER ASSIGNMENT expression direction expression DO stmt {
        $$ = MakeAST<ForStmtAST>($5, MakeAST<IdentifierAST>($2), $4, $5, $8);
    }
    ;

direction: TO {$$ = Direct::TO;}
    | DOWNTO {$$ = Direct::DOWNTO;}
    ;

if_stmt: IF expression THEN stmt NOELSE {
        $$ = MakeAST<IfStmtAST>($2, $4, MakeAST<CompoundStmtAST>());
    }
    | IF expression THEN stmt ELSE stmt {
        $$ = MakeAST<IfStmtAST>($2, $4, $6);
    }
    ;

call_stmt: IDENTIFIER params {
        $$ = MakeAST<CallStmtAST>(MakeAST<CustomFuncAST>($1, $2));
    }
    | SYS_FUNC params {
        $$ = MakeAST<CallStmtAST>(MakeAST<SysFuncAST>($1, $2));
    }
    ;

params: LPAREN arg_list RPAREN {$$ = $2;}
    | LPAREN RPAREN {$$ = MakeAST<ArgListAST>();}
    | {$$ = MakeAST<ArgListAST>();}
    ;

parameter_list: parameter_list COMMA expression
    | expression
    ;

expression: expr {$$ = $1;}
    | expression relop expr {
        $$ = MakeAST<BinaryExprAST>($2, $1, $3);
    }
    ;

expr: term addop expr {
        $$ = MakeAST<BinaryExprAST>($2, $1, $3);
    }
    | term {$$ = $1;}
    ;

term: factor {$$ = $1;}
    | term mulop factor {
        $$ = MakeAST<BinaryExprAST>($2, $1, $3);
    }
    ;

factor: primary {$$ = $1;}
    | primary STARSTAR factor {
        $$ = MakeAST<BinaryExprAST>(BinaryOp::POW, $1, $3);
    }
    ;

primary: IDENTIFIER {$$ = MakeAST<IdentifierAST>($1);}
    | IDENTIFIER DOT IDENTIFIER {
        $$ = MakeAST<RecordAccessAST>($1, $3);
    }
    | IDENTIFIER LBRAC expression RBRAC {
        $$ = MakeAST<ArrayAccessAST>($1, $3);
    }
    | IDENTIFIER LPAREN RPAREN {
        $$ = MakeAST<CustomFuncAST>($1, MakeAST<ArgListAST>());
    }
    | IDENTIFIER LPAREN arg_list RPAREN {
        $$ = MakeAST<CustomFuncAST>($1, $3);
    }
    | SYS_FUNC LPAREN arg_list RPAREN {
        $$ = MakeAST<SysFuncAST>($1, $3);
    }
    | const_value {$$ = $1;}
    | LPAREN expression RPAREN {$$ = $2;}
    | NOT primary {
        $$ = MakeAST<UnaryExprAST>(UnaryOp::NOT, $2);
    }
    | sign primary {
        $$ = MakeAST<UnaryExprAST>($1, $2);
    }
    ;

arg_list: arg_list COMMA expression {
        $$ = $1;
        $$->AppendChild($3);
    }
    | expression {
        $$ = MakeAST<ArgListAST>();
        $$->AppendChild($1);
    }

const_value: signed_number {$$ = $1;}
    | LITERAL {$$ = MakeAST<StringAST>($1);}
    | TRUE {$$ = MakeAST<BooleanAST>($1);}
    | FALSE {$$ = MakeAST<BooleanAST>($1);}
    ;

signed_number: signed_integer {$$ = $1;}
    | signed_real {$$ = $1;}
    ;

signed_integer: DIGSEQ {$$ = MakeAST<IntegerAST>($1);}
    | SIGNEDDIGSEQ {$$ = MakeAST<IntegerAST>($1);}
    ;

signed_real: SIGNEDREALNUMBER {$$ = MakeAST<RealAST>($1);}
    | REALNUMBER {$$ = MakeAST<RealAST>($1);}
    ;

sign: PLUS {$$ = UnaryOp::POS;}
    | MINUS {$$ = UnaryOp::NEG;}
    ;

addop: PLUS {$$ = BinaryOp::ADD;}
    | MINUS {$$ = BinaryOp::SUB;}
    | OR  {$$ = BinaryOp::OR;}
    | XOR {$$ = BinaryOp::XOR;}
    ;

mulop: STAR {$$ = BinaryOp::MUL;}
    | MUL {$$ = BinaryOp::MUL;}
    | SLASH {$$ = BinaryOp::SLASH;}
    | DIV {$$ = BinaryOp::DIV;}
    | MOD {$$ = BinaryOp::MOD;}
    | AND {$$ = BinaryOp::AND;}
    ;

relop: EQUAL {$$ = BinaryOp::EQ;}
    | NOTEQUAL {$$ = BinaryOp::NE;}
    | LT {$$ = BinaryOp::LT;}
    | GT {$$ = BinaryOp::GT;}
    | LE {$$ = BinaryOp::LE;}
    | GE {$$ = BinaryOp::GE;}
    ;

%%

void Parser::error(const location_type &loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
    throw std::logic_error("Syntax error: invalid syntax");
}