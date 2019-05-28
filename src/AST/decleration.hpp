#ifndef YAPC_DECLERATION_HPP
#define YAPC_DECLERATION_HPP

#include "basicast.hpp"
#include "identifier.hpp"
#include "type.hpp"

namespace yapc {
    class DeclAST : public BasicAST {

    };

    class VarDeclAST : public DeclAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<TypeAST> type;

        VarDeclAST(std::unique_ptr<IdentifierAST>& name, std::unique_ptr<TypeAST>& type) : name(std::move(name)), type(std::move(type)) {}

        genValue codegen(genContext context) override;
    };

    class ConstDeclAST : public DeclAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<ConstAST> value;

        ConstDeclAST(std::unique_ptr<IdentifierAST>& name, std::unique_ptr<ConstAST>& value) : name(std::move(name)), value(std::move(value)) {}

        genValue codegen(genContext context) override;
    };

    class TypeDeclAST : public DeclAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<TypeAST> type;

        TypeDeclAST(std::unique_ptr<IdentifierAST>& name, std::unique_ptr<TypeAST>& type) : name(std::move(name)), type(std::move(type)) {}

        genValue codegen(genContext context) override;
    };

    using TypeDeclListAST = ListAST<TypeDeclAST>;
    using VarDeclListAST = ListAST<VarDeclAST>;
    using ConstDeclListAST = ListAST<ConstDeclAST>;

    class ParamAST : public BasicAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        std::unique_ptr<TypeAST> type;

        ParamAST(std::unique_ptr<IdentifierAST>& name, std::unique_ptr<TypeAST>& type) : name(std::move(name)), type(std::move(type)) {}

        genValue codegen(genContext context) override;
    };

    using ParamListAST = ListAST<ParamAST>;
    using ArgListAST = ListAST<ExprAST>;

//    class ParamListAST : public BasicAST {
//    public:
//        ParamListAST() {}
//        ParamListAST(std::unique_ptr<ParamAST>& value) {
//            children.push_back(std::move(value));
//        }
//        ParamListAST(const std::unique_ptr<ParamListAST>&& value) {
//            children.push_back(std::move(value));
//        }
//        ~ParamListAST() = default;
//
//        void AppendChild(std::list<std::unique_ptr<ParamAST>>& orphan) {
//            children.push_back(std::move(orphan));
//        }
//
//        void AppendChild(const std::list<std::unique_ptr<ParamAST>>&& orphan) {
//            children.push_back(std::move(orphan));
//        }
//
//        void MergeAST(std::list<std::unique_ptr<ParamAST>>& orphans) {
//            for(auto& e : orphans) {
//                AppendChild(std::move(e));
//            }
//        }
//
//    protected:
//        std::list<std::unique_ptr<ParamAST>> children;
//    };
//
//    class ArgsListAST : public BasicAST {
//    public:
//        ArgsListAST() {}
//        ArgsListAST(std::unique_ptr<ExprAST>&)
//    protected:
//        std::list<std::unique_ptr<ExprAST>> children;
//    };
}

#endif
