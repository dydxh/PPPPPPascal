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
        std::shared_ptr<TypeAST> type;
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<ParamListAST> params;

        ProgHeadAST(const std::shared_ptr<TypeAST> typeval, const std::shared_ptr<IdentifierAST> nameval, const std::shared_ptr<ParamListAST> paramval) : type(typeval), name(nameval), params(paramval) {}

        //genValue codegen(genContext context) override;
    };

    using ProgListAST = ListAST<ProgramAST>;

    class ProgBlockAST : public BasicAST {
    public:
        std::shared_ptr<ConstDeclListAST> constpart;
        std::shared_ptr<TypeDeclListAST> typepart;
        std::shared_ptr<VarDeclListAST> varpart;
        std::shared_ptr<ProgListAST> progpart;
        std::shared_ptr<CompoundStmtAST> progbody;

        ProgBlockAST(const std::shared_ptr<ConstDeclListAST>& constpart, const std::shared_ptr<TypeDeclListAST>& typepart,
            const std::shared_ptr<VarDeclListAST>& varpart, const std::shared_ptr<ProgListAST>& progpart,
            const std::shared_ptr<CompoundStmtAST>& progbody)
            : constpart(constpart), typepart(typepart), varpart(varpart), progpart(progpart), progbody(progbody) {}

        //genValue codegen(genContext context) override;
    };

    class ProgramAST : public  BasicAST {
    public:
        std::shared_ptr<ProgHeadAST> proghead;
        std::shared_ptr<ProgBlockAST> progblock;

        ProgramAST(const std::shared_ptr<ProgHeadAST>& prog_head, const std::shared_ptr<ProgBlockAST>& prog_block)
            : proghead(prog_head), progblock(prog_block) {}

        //genValue codegen(genContext context) override;
    };
}

#endif
