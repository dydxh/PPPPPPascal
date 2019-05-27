#ifndef yapc_ast_type
#define yapc_ast_type

#include "basicast.hpp"
#include "identifier.hpp"

namespace yapc {
    enum class Type {
        UNKNOWN, VOID, INTEGER, REAL, STRING, ARRAY, RECORD, BOOLEAN
    };

    class TypeAST : public BasicAST {
    public:
        Type type = Type::UNKNOWN;

        TypeAST() = default;
        ~TypeAST() = default;

        genValue codegen(genContext context) override;
    };

    class PrimaryTypeAST : public TypeAST {
    public:
        PrimaryTypeAST(Type type) {
            this->type = type;
        }
    };

    class VoidType : public TypeAST {
    public:
        VoidType() {
            this->type = Type::VOID;
        }
    };

    class StringTypeAST : public TypeAST {
    public:
        StringTypeAST() {
            this->type = Type::STRING;
        }
    };

    class RecordTypeAST : public TypeAST {
    public:
        RecordTypeAST() {
            this->type = Type::RECORD;
        }
    };

    class ArrayTypeAST : public TypeAST {
    public:
        std::unique_ptr<ExprAST> StartValue, EndValue;
        std::unique_ptr<TypeAST> ItemType;

        ArrayTypeAST(std::unique_ptr<ExprAST>&& startvalue, std::unique_ptr<ExprAST>&& endvalue, std::unique_ptr<TypeAST>&& itemtype) {
            StartValue = std::move(startvalue);
            EndValue = std::move(endvalue);
            ItemType = std::move(itemtype);
            this->type = Type::ARRAY;
        }
    };
    
    class DeclTypeAST : public TypeAST {
    public:
        std::unique_ptr<IdentifierAST> name;
        
        DeclTypeAST(std::unique_ptr<IdentifierAST>&& ID) {
            name = std::move(ID);
        }
        ~DeclTypeAST() = default;
    };

    class ConstAST : public ExprAST {
    public:
        Type type = Type::UNKNOWN;

        ConstAST() = default;
    };

    class BooleanAST : public TypeAST {
    public:
        bool val;

        BooleanAST(bool val) : val(val) {
            this->type = Type::BOOLEAN;
        }

        genValue codegen(genContext context) override;
    };

    class IntegerAST : public TypeAST {
    public:
        int val;

        IntegerAST(int val) : val(val) {
            this->type = Type::INTEGER;
        }

        genValue codegen(genContext context) override;
    };

    class RealAST : public ConstAST {
    public:
        double val;

        RealAST(double val) : val(val) {
            this->type = Type::REAL;
        }

        genValue codegen(genContext context) override;
    };
    
    class StringAST : public ConstAST {
    public:
        std::string val;

        StringAST(const char* val) : val(val) {
            this->type = Type::STRING;
        }

        genValue codegen(genContext context) override;
    };
}

#endif