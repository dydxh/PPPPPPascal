#ifndef yapc_ast_frame
#define yapc_ast_frame

#include <list>
#include <memory>
#include <iostream>

#include "basicast.hpp"
#include "decleration.hpp"
#include "expr.hpp"
#include "type.hpp"
#include "statement.hpp"

namespace yapc {
    class ProgramAST;

    class ProgHeadAST : public BasicAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<ParamListAST> params;
        Type type;

        ProgHeadAST(std::unique_ptr<IdentifierAST>& name, std::unique_ptr<ParamListAST>& param) : name(std::move(name)), params(std::move(param)) {}

        genValue codegen(genContext context) override;
    };

    using ProgListAST = ListAST<ProgramAST>;

    class ProgBlockAST : public BasicAST {
    public:
        std::unique_ptr<ConstDeclListAST> const_part;
        std::unique_ptr<TypeDeclListAST> type_part;
        std::unique_ptr<VarDeclListAST> var_part;
        std::unique_ptr<ProgListAST> prog_part;

        ProgBlockAST(std::unique_ptr<ConstDeclAST>& constpart, std::unique_ptr<TypeDeclAST>& typepart,
                     std::unique_ptr<VarDeclListAST>& varpart, std::unique_ptr<ProgListAST>& progpart)
                     : const_part(constpart), type_part(typepart), var_part(varpart), prog_part(progpart) {}

        genValue codegen(genContext context) override;
    };

    class ProgramAST : public  BasicAST {
    public:
        std::unique_ptr<ProgHeadAST> proghead;
        std::unique_ptr<ProgBlockAST> progblock;

        ProgramAST(std::unique_ptr<ProgHeadAST>& prog_head, std::unique_ptr<ProgBlockAST>& prog_block)
            : proghead(prog_head), progblock(prog_block) {}

        genValue codegen(genContext context) override;
    };
}

#endif
