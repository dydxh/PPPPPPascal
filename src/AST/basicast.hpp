#ifndef yapc_ast_ast
#define yapc_ast_ast

#include <list>
#include <memory>
#include <cassert>

namespace yapc {
    using genValue = int;
    using genContext = int;

    class BasicAST {
    public:
        virtual ~BasicAST() = default;

        virtual genValue codegen(genContext context) = 0;
    };

    using BasicPtr = std::unique_ptr<BasicAST>;

    class ExprAST : public BasicAST {
    public:
        ExprAST() = default;
        ~ExprAST() = default;
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
        ListAST(std::unique_ptr<T>& value) {
            children.push_back(std::move(value));
        }
        ListAST(const std::unique_ptr<T>&& value) {
            children.push_back(std::move(value));
        }
        ~ListAST() = default;

        void AppendChild(std::list<std::unique_ptr<T>>& orphan) {
            children.push_back(std::move(orphan));
        }

        void AppendChild(const std::list<std::unique_ptr<T>>&& orphan) {
            children.push_back(std::move(orphan));
        }

        void MergeAST(std::list<std::unique_ptr<T>>& orphans) {
            for(auto& e : orphans) {
                AppendChild(std::move(e));
            }
        }

    protected:
        std::list<std::unique_ptr<T>> children;
    };
}

#endif