#ifndef yapc_ast_ast
#define yapc_ast_ast

#include <list>
#include <memory>
#include <cassert>

namespace yapc {
    using genValue = int;
    using genContext = int;

    class BasicAST : public std::enable_shared_from_this<BasicAST> {
    public:
        virtual ~BasicAST() = default;

        virtual genValue codegen(genContext context) = 0;

        virtual bool child_exist() {return true;}

        void append_child(const std::unique_ptr<BasicAST>& orphan) {
            orphan->father = this->shared_from_this();
            this->children.push_back(std::move(orphan));
        }

        void append_child(std::unique_ptr<BasicAST>&& orphan) {
            orphan->father = this->shared_from_this();
            this->children.push_back(std::move(orphan));
        }

        void merge_AST(const std::list<std::unique_ptr<BasicAST>>& orphans) {
            for(const auto &e : orphans)
                this->append_child(std::move(e));
        }

    protected:
        std::list<std::unique_ptr<BasicAST>> children;
        std::shared_ptr<BasicAST> father;
    };

    using BasicPtr = std::unique_ptr<BasicAST>;

    class ExprAST : public BasicAST {
    public:
        ExprAST() = default;
        ~ExprAST() = default;

        bool child_exist() {return false;}
    };
}

#endif