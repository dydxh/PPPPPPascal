#ifndef yapc_ast_type
#define yapc_ast_type

#include <memory>
#include <iostream>
#include <list>
#include <sstream>

#include "basicast.hpp"
#include "identifier.hpp"
#include "decleration.hpp"

namespace yapc {
    class VarDeclAST;

    enum class Type {
        UNKNOWN, VOID, INTEGER, REAL, STRING, ARRAY, RECORD, BOOLEAN, LONGINT
    };

    class TypeAST : public BasicAST {
    public:
        Type type = Type::UNKNOWN;

        TypeAST() = default;
        ~TypeAST() = default;
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

    protected:
        std::list<std::shared_ptr<VarDeclAST>> fields;
    };

    class ArrayTypeAST : public TypeAST {
    public:
        std::shared_ptr<ExprAST> StartValue, EndValue;
        Type ItemType;

        ArrayTypeAST(std::shared_ptr<ExprAST>&& startvalue, std::shared_ptr<ExprAST>&& endvalue, Type itemtype) {
            StartValue = startvalue;
            EndValue = endvalue;
            ItemType = itemtype;
            this->type = Type::ARRAY;
        }
    };
    
    class DeclTypeAST : public TypeAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        
        DeclTypeAST(std::shared_ptr<IdentifierAST>&& ID) {
            name = ID;
        }
        ~DeclTypeAST() = default;
    };

    class ConstAST : public ExprAST {
    public:
        Type type = Type::UNKNOWN;

        ConstAST() {};
    };

    class BooleanAST : public ConstAST {
    public:
        bool val;

        BooleanAST() : val(false) {
            this->type = Type::BOOLEAN;
        }
        BooleanAST(bool val) : val(val) {
            this->type = Type::BOOLEAN;
        }

        //genValue codegen(genContext context) override;
    };

    class IntegerAST : public ConstAST {
    public:
        int val;

        IntegerAST() : val(0) {
            this->type = Type::INTEGER;
        }
        IntegerAST(int val) : val(val) {
            this->type = Type::INTEGER;
        }

        //genValue codegen(genContext context) override;
    };

    class RealAST : public ConstAST {
    public:
        double val;

        RealAST() : val(0) {
            this->type = Type::REAL;
        }
        RealAST(const double& val) : val(val) {
            this->type = Type::REAL;
        }
        ~RealAST() {}

        //genValue codegen(genContext context) override;
    };
    
    class StringAST : public ConstAST {
    public:
        std::string val;

        StringAST() : val("") {
            this->type = Type::STRING;
        }
        StringAST(const char* val) : val(val) {
            this->type = Type::STRING;
        }
        StringAST(const std::string& val) : val(val) {
            this->type = Type::STRING;
        }

        //genValue codegen(genContext context) override;
    };
}

#endif