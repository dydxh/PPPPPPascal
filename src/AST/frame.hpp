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
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<ParamListAST> params;
        Type type;

        ProgHeadAST(Type type, std::shared_ptr<IdentifierAST>& name, std::shared_ptr<ParamListAST>& param) : type(type), name(name), params(param) {}

        //genValue codegen(genContext context) override;
    };

    using ProgListAST = ListAST<ProgramAST>;

    class ProgBlockAST : public BasicAST {
    public:
        std::shared_ptr<ConstDeclListAST> const_part;
        std::shared_ptr<TypeDeclListAST> type_part;
        std::shared_ptr<VarDeclListAST> var_part;
        std::shared_ptr<ProgListAST> prog_part;

        ProgBlockAST(std::shared_ptr<ConstDeclListAST>& constpart, std::shared_ptr<TypeDeclListAST>& typepart,
                     std::shared_ptr<VarDeclListAST>& varpart, std::shared_ptr<ProgListAST>& progpart)
                     : const_part(constpart), type_part(typepart), var_part(varpart), prog_part(progpart) {}

        //genValue codegen(genContext context) override;
    };

    class ProgramAST : public  BasicAST {
    public:
        std::shared_ptr<ProgHeadAST> proghead;
        std::shared_ptr<ProgBlockAST> progblock;

        ProgramAST(std::shared_ptr<ProgHeadAST>& prog_head, std::shared_ptr<ProgBlockAST>& prog_block)
            : proghead(prog_head), progblock(prog_block) {}

        //genValue codegen(genContext context) override;
    };
}

#endif
