#ifndef yapc_ast_ast
#define yapc_ast_ast

#include <list>
#include <memory>
#include <iostream>
#include <cassert>
#include "codeGen/codeGen_utils.hpp"

namespace yapc {
    using genValue = llvm::Value *;
    using genContext = CodeGenUtils &;
    //using genValue = int;
    //using genContext = int;

    class BasicAST {
    public:
        ~BasicAST() {};

        virtual genValue codegen(genContext context) = 0;
    };

    using BasicPtr = std::shared_ptr<BasicAST>;

    class ExprAST : public BasicAST {
    public:
        ExprAST() {};
        ~ExprAST() {};
        genValue codegen(genContext context) = 0;
    };

    class StmtAST : public BasicAST {
    public:
        StmtAST() = default;
        ~StmtAST() = default;
        genValue codegen(genContext context) = 0;
    };

    template<typename T>
    class ListAST : public BasicAST {
    public:
        ListAST() {}
        ListAST(const std::shared_ptr<T>& value) {
            children.push_back(value);
        }
        ListAST(const std::shared_ptr<T>&& value) {
            children.push_back(value);
        }
        ~ListAST() = default;

        void AppendChild(const std::shared_ptr<T>& orphan) {
            children.push_back(orphan);
        }

        void AppendChild(const std::shared_ptr<T>&& orphan) {
            children.push_back(orphan);
        }

        void MergeAST(const std::list<std::shared_ptr<T>>& orphans) {
            for(auto& e : orphans) {
                AppendChild(e);
            }
        }

        std::list<std::shared_ptr<T>>& get_children() {return children;}
        genValue codegen(genContext context) override {
            for (auto &p : children) {
                p->codegen(context);
            }
        }

    protected:
        std::list<std::shared_ptr<T>> children;
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> MakeAST(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif