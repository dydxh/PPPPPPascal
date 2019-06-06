//
// Created by zuhxs on 2019/5/29.
//

#ifndef YAPC_CODEGEN_UTILS_HPP
#define YAPC_CODEGEN_UTILS_HPP



#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"


#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>


static llvm::LLVMContext llvm_context;


namespace yapc {

    class CodeGenUtils {
    public:
        CodeGenUtils(std::string module_id) : Builder(llvm::IRBuilder<>(llvm_context)) {
            TheModule = std::make_unique<llvm::Module>(module_id, llvm_context);
            auto PrintType = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm_context), {llvm::Type::getInt8PtrTy(llvm_context)}, true);
            PrintfFunction = llvm::Function::Create(PrintType, llvm::Function::ExternalLinkage, "printf", *TheModule);
            PrintfFunction->setCallingConv(llvm::CallingConv::C);
        }

        llvm::GlobalVariable *GetValue(std::string key) {
            std::vector<std::string>::reverse_iterator r_iter;
            for (r_iter = Traces.rbegin(); r_iter != Traces.rend(); ++r_iter) {
                auto V = *r_iter;
                std::string symbol = V + "_" + key;
                auto *value = TheModule.get()->getGlobalVariable(symbol);
                if (!value) {
                    continue;
                }
                return value;
            }
            return nullptr;
        };

        llvm::Type *GetAlias(std::string key) {
            auto V = Aliases[key];
            if (!V)
                return nullptr;
            return V;
        }
        bool SetAlias(std::string key, llvm::Type *value) {
            if (GetAlias(key))
                return false;
            Aliases[key] = value;
            return true;
        }
        llvm::IRBuilder<> &GetBuilder() {
            return Builder;
        }
        bool is_subroutine = false;
        std::shared_ptr<llvm::Module> &GetModule() {
            return TheModule;
        }
        void dump() {
            TheModule.get()->print(llvm::errs(), nullptr);
        }
        std::vector<std::string> &GetTrace() {
            return Traces;
        }

    public:
        std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;
        std::unique_ptr<llvm::legacy::PassManager> mpm;
        llvm::Function *PrintfFunction;
    private:
        std::shared_ptr<llvm::Module> TheModule;
        llvm::IRBuilder<> Builder;
        //std::vector<std::map<std::string, llvm::Value *>> NamedValues1;
        //std::map<std::string, llvm::Value *> NamedValues;
        std::map<std::string, llvm::Type *> Aliases;
        std::vector<std::string> Traces;

    };

    class CodegenException : public std::exception {
    public:
        explicit CodegenException(const std::string &description) : description(description) {};
        const char *what() const noexcept {
            return ("Codegen error: " + description).c_str();
        }
    private:
        std::string description;
    };

}


#endif //YAPC_CODEGEN_UTILS_HPP