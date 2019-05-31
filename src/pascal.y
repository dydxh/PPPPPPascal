%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {yapc}
%define parser_class_name {Parser}
%define api.value.type variant
%locations

%code requires {
    #include "AST/expr.hpp"
    #include "AST/frame.hpp"
    #include "AST/basicast.hpp"
    #include "AST/decleration.hpp"
    #include "AST/identifier.hpp"
    #include "AST/statement.hpp"
    #include "AST/type.hpp"
    #include <iostream>
    #include <memory>
    #include <string>
    #include <stdexcept>
    namespace yapc {
        class Scanner;
        class Driver;
    }
    using namespace yapc;
}

%parse-param {Scanner& scanner}
%parse-param {Driver& driver}

%code {
    #include "Driver.hpp"
    #include "Scanner.hpp"
    #undef yylex
    #define yylex scanner.yylex
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
%type <std::string> LITERAL IDENTIFIER
%type <yapc::Direct> direction
%type <std::shared_ptr<ConstAST>> const_value signed_number
%type <std::shared_ptr<IntegerAST>> signed_integer
%type <std::shared_ptr<RealAST>> signed_real
%type <yapc::SysFunc> SYS_FUNC

%type <std::shared_ptr<ArgListAST>> arg_list params
%type <std::shared_ptr<ExprAST>> primary factor term expr expression
%type <std::shared_ptr<ProgListAST>> subproc_decl_list subproc_decl_part
%type <std::shared_ptr<ProgramAST>> func_decl procdure_decl subproc_decl program
%type <std::shared_ptr<ProgHeadAST>> function_header procedure_header program_header
%type <std::shared_ptr<ProgBlockAST>> proc_block
%type <std::shared_ptr<ConstDeclListAST>> const_decl_list const_decl_part
%type <std::shared_ptr<ConstDeclAST>> const_decl
%type <std::shared_ptr<TypeDeclListAST>> type_decl_list type_decl_part
%type <std::shared_ptr<TypeDeclAST>> type_decl
%type <std::shared_ptr<TypeAST>> type_denoter
%type <std::shared_ptr<PrimaryTypeAST>> SIMPLE_TYPE
%type <std::shared_ptr<StringTypeAST>> STRING
%type <std::shared_ptr<ArrayTypeAST>> array_type
%type <std::shared_ptr<RecordTypeAST>> field_decl field_decl_list record_type
%type <std::shared_ptr<IdentifierListAST>> name_list
%type <std::shared_ptr<VarDeclListAST>> var_decl var_decl_list var_decl_part
%type <std::shared_ptr<ParamListAST>> param_decl param_decl_list formal_param_part
%type <std::shared_ptr<CompoundStmtAST>> stmt stmt_list compound_part
%type <std::shared_ptr<CallStmtAST>> call_stmt
%type <std::shared_ptr<IfStmtAST>> if_stmt
%type <std::shared_ptr<ForStmtAST>> for_stmt
%type <std::shared_ptr<WhileStmtAST>> while_stmt
%type <std::shared_ptr<RepeatStmtAST>> repeat_stmt
%type <std::shared_ptr<AssignStmtAST>> assignment_stmt
%type <UnaryOp> sign
%type <BinaryOp> relop addop mulop

%nonassoc ELSE
%nonassoc NOELSE

%start program

%%
program: program_header SEMICOLON proc_block DOT TERMINATE {
        driver.context.program = MakeAST<ProgramAST>($1, $3);
    }
    ;

program_header: PROGRAM IDENTIFIER {$$ = MakeAST<ProgHeadAST>(MakeAST<VoidTypeAST>(), MakeAST<IdentifierAST>($2), MakeAST<ParamListAST>());}
    ;

proc_block: const_decl_part type_decl_part var_decl_part subproc_decl_part compound_part {
        $$ = MakeAST<ProgBlockAST>($1, $2, $3, $4, $5);
    }
    ;

const_decl_part: CONST const_decl_list {$$ = $2;}
    | {$$ = MakeAST<ConstDeclListAST>();}
    ;

const_decl_list: const_decl_list const_decl {
        $$ = $1;
        $$->AppendChild($2);
    }
    | const_decl {$$ = MakeAST<ConstDeclListAST>($1);}
    ;

const_decl: IDENTIFIER EQUAL const_value SEMICOLON {
        $$ = MakeAST<ConstDeclAST>(MakeAST<IdentifierAST>($1), $3);
    }
    ;

type_decl_part: TYPE type_decl_list {$$ = $2;}
    | {$$ = MakeAST<TypeDeclListAST>();}
    ;

type_decl_list: type_decl_list type_decl {
        $$ = $1;
        $$->AppendChild($2);
    }
    | type_decl {$$ = MakeAST<TypeDeclListAST>($1);}
    ;

type_decl: IDENTIFIER EQUAL type_denoter SEMICOLON {
        $$ = MakeAST<TypeDeclAST>(MakeAST<IdentifierAST>($1), $3);
    }
    ;

type_denoter: IDENTIFIER {
        $$ = MakeAST<DeclTypeAST>(MakeAST<IdentifierAST>($1));
    }
    | SIMPLE_TYPE {$$ = $1;}
    | STRING {$$ = $1;}
    | array_type {$$ = $1;}
    | record_type {$$ = $1;}
    ;

array_type: ARRAY LBRAC signed_integer DOTDOT signed_integer RBRAC OF SIMPLE_TYPE {
        $$ = MakeAST<ArrayTypeAST>($3, $5, $8);
    }
    | ARRAY LBRAC IDENTIFIER DOTDOT IDENTIFIER RBRAC OF SIMPLE_TYPE {
        $$ = MakeAST<ArrayTypeAST>(MakeAST<IdentifierAST>($3), MakeAST<IdentifierAST>($5), $8);
    }
    ;

record_type: RECORD field_decl_list END {$$ = $2;}
    ;

field_decl_list: field_decl_list field_decl {
        $$ = $1;
        $$->MergeField($2);
    }
    | field_decl {$$ = $1;}
    ;

field_decl: name_list COLON SIMPLE_TYPE SEMICOLON {
        $$ = MakeAST<RecordTypeAST>();
        for(auto name : $1->get_children())
            $$->AddField(MakeAST<VarDeclAST>(name, $3));
    }
    ;

name_list: name_list COMMA IDENTIFIER {
        $$ = $1;
        $$->AppendChild(MakeAST<IdentifierAST>($3));
    }
    | IDENTIFIER {$$ = MakeAST<IdentifierListAST>(MakeAST<IdentifierAST>($1));}
    ;

var_decl_part: VAR var_decl_list {$$ = $2;}
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
    | {$$ = MakeAST<ProgListAST>();}
    ;

subproc_decl_list: subproc_decl_list subproc_decl {
        $$ = $1;
        $$->AppendChild($2);
    }
    | subproc_decl {
        $$ = MakeAST<ProgListAST>($1);
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
        $$ = MakeAST<ProgHeadAST>(MakeAST<VoidTypeAST>(), MakeAST<IdentifierAST>($2), MakeAST<ParamListAST>());
    }
    | PROCEDURE IDENTIFIER formal_param_part {
        $$ = MakeAST<ProgHeadAST>(MakeAST<VoidTypeAST>(), MakeAST<IdentifierAST>($2), $3);
    }
    ;

formal_param_part: LPAREN param_decl_list RPAREN {$$ = $2;}
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
        $$ = MakeAST<AssignStmtAST>(MakeAST<IdentifierAST>($1), $3);
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
        $$ = MakeAST<ForStmtAST>($5, MakeAST<IdentifierAST>($2), $4, $6, $8);
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