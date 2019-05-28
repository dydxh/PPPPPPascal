#ifndef yapc_ast_ast
#define yapc_ast_ast

#include <list>
#include <memory>
#include <iostream>
#include <cassert>

namespace yapc {
    using genValue = int;
    using genContext = int;

    class BasicAST {
    public:
        ~BasicAST() {};

        //virtual genValue codegen(genContext context) = 0;
    };

    using BasicPtr = std::shared_ptr<BasicAST>;

    class ExprAST : public BasicAST {
    public:
        ExprAST() {};
        ~ExprAST() {};
    };

    class StmtAST : public BasicAST {
    public:
        StmtAST() = default;
        ~StmtAST() = default;
    };

    template<typename T>
    class ListAST : public BasicAST {
    public:
        ListAST() {}
        ListAST(std::shared_ptr<T>& value) {
            children.push_back(value);
        }
        ListAST(const std::shared_ptr<T>&& value) {
            children.push_back(value);
        }
        ~ListAST() = default;

        void AppendChild(std::list<std::shared_ptr<T>>& orphan) {
            children.push_back(orphan);
        }

        void AppendChild(const std::list<std::shared_ptr<T>>&& orphan) {
            children.push_back(orphan);
        }

        void MergeAST(std::list<std::shared_ptr<T>>& orphans) {
            for(auto& e : orphans) {
                AppendChild(e);
            }
        }

        std::list<std::shared_ptr<T>>& get_children() {return children;}

    protected:
        std::list<std::shared_ptr<T>> children;
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> MakeAST(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif