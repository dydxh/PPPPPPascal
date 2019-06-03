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

        TypeAST() {};
        ~TypeAST() {};
        genValue codegen(genContext context) override {}
        llvm::Type *GetType(CodeGenUtils &context);
    };

    class PrimaryTypeAST : public TypeAST {
    public:
        PrimaryTypeAST(Type type) {
            this->type = type;
        }
    };

    class VoidTypeAST : public TypeAST {
    public:
        VoidTypeAST() {
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
        void AddField(const std::shared_ptr<VarDeclAST>& single_field) {
            fields.push_back(single_field);
        }
        void MergeField(const std::shared_ptr<RecordTypeAST>& another_record) {
            for(auto& e : another_record->get_fields()) {
                fields.push_back(e);
            }
        }
        std::list<std::shared_ptr<VarDeclAST>>& get_fields() {return this->fields;}

    protected:
        std::list<std::shared_ptr<VarDeclAST>> fields;
    };

    class ArrayTypeAST : public TypeAST {
    public:
        std::shared_ptr<ExprAST> StartValue, EndValue;
        std::shared_ptr<TypeAST> ItemType;

        ArrayTypeAST(const std::shared_ptr<ExprAST>& startvalue, const std::shared_ptr<ExprAST>& endvalue, const std::shared_ptr<TypeAST>& itemtype) {
            StartValue = startvalue;
            EndValue = endvalue;
            ItemType = itemtype;
            this->type = Type::ARRAY;
        }
    };
    
    class DeclTypeAST : public TypeAST {
    public:
        std::shared_ptr<IdentifierAST> name;
        
        DeclTypeAST(const std::shared_ptr<IdentifierAST>&& ID) {
            name = ID;
        }
        ~DeclTypeAST() {};
    };

    class ConstAST : public ExprAST {
    public:
        Type type = Type::UNKNOWN;

        ConstAST() {};
        llvm::Type *GetType(CodeGenUtils &context) {
            switch (type) {
                case Type::BOOLEAN: return context.GetBuilder().getInt1Ty();
                case Type::INTEGER: return context.GetBuilder().getInt32Ty();
                case Type::LONGINT: return context.GetBuilder().getInt32Ty();
                case Type::REAL: return context.GetBuilder().getDoubleTy();
                //case Type::STRING: throw CodegenException("string currently not supported.\n");
                default: return nullptr;
            }
        }

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

        genValue codegen(genContext context) override;
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

        genValue codegen(genContext context) override;
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

        genValue codegen(genContext context) override;
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

        genValue codegen(genContext context) override;
    };
}

#endif