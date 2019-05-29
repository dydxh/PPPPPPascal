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
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<TypeAST> type;

        VarDeclAST(const std::shared_ptr<IdentifierAST>& name, const std::shared_ptr<TypeAST>& type) : name(name), type(type) {}

        //genValue codegen(genContext context) override;
    };

    class ConstDeclAST : public DeclAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<ConstAST> value;

        ConstDeclAST(const std::shared_ptr<IdentifierAST>& name, const std::shared_ptr<ConstAST>& value) : name(name), value(value) {}

        //genValue codegen(genContext context) override;
    };

    class TypeDeclAST : public DeclAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<TypeAST> type;

        TypeDeclAST(const std::shared_ptr<IdentifierAST>& name, const std::shared_ptr<TypeAST>& type) : name(name), type(type) {}

        //genValue codegen(genContext context) override;
    };

    using TypeDeclListAST = ListAST<TypeDeclAST>;
    using VarDeclListAST = ListAST<VarDeclAST>;
    using ConstDeclListAST = ListAST<ConstDeclAST>;
    using IdentifierListAST = ListAST<IdentifierAST>;

    class ParamAST : public BasicAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        std::shared_ptr<TypeAST> type;

        ParamAST(const std::shared_ptr<IdentifierAST>& name, const std::shared_ptr<TypeAST>& type) : name(name), type(type) {}

        //genValue codegen(genContext context) override;
    };

    using ParamListAST = ListAST<ParamAST>;
    using ArgListAST = ListAST<ExprAST>;

//    class ParamListAST : public BasicAST {
//    public:
//        ParamListAST() {}
//        ParamListAST(std::shared_ptr<ParamAST>& value) {
//            children.push_back(value);
//        }
//        ParamListAST(const std::shared_ptr<ParamListAST>&& value) {
//            children.push_back(value);
//        }
//        ~ParamListAST() = default;
//
//        void AppendChild(std::list<std::shared_ptr<ParamAST>>& orphan) {
//            children.push_back(orphan);
//        }
//
//        void AppendChild(const std::list<std::shared_ptr<ParamAST>>&& orphan) {
//            children.push_back(orphan);
//        }
//
//        void MergeAST(std::list<std::shared_ptr<ParamAST>>& orphans) {
//            for(auto& e : orphans) {
//                AppendChild(e);
//            }
//        }
//
//    protected:
//        std::list<std::shared_ptr<ParamAST>> children;
//    };
//
//    class ArgsListAST : public BasicAST {
//    public:
//        ArgsListAST() {}
//        ArgsListAST(std::shared_ptr<ExprAST>&)
//    protected:
//        std::list<std::shared_ptr<ExprAST>> children;
//    };
}

#endif
