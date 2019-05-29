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
        CodeGenUtils(std::string module_id) : Builder(llvm::IRBuilder<>(llvm_context)), TheModule(llvm::make_unique<llvm::Module>(TheModule, llvm_context)) {}
        llvm::Value *GetValue(std::string key) {
            auto V = NamedValues[key];
            if (!V)
                return nullptr;
            return V;
        };
        bool SetValue(std::string key, llvm::Value *value) {
            if (GetValue(key))
                return false;
            NamedValues[key] = value;
            return true;
        }
        void ResetValue() {
            NamedValues.clear();
        }
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

    private:
        llvm::IRBuilder<> Builder;
        std::unique_ptr<llvm::Module> TheModule;
        std::map<std::string, llvm::Value *> NamedValues;
        std::map<std::string, llvm::Type *> Aliases;
    };

}



#endif //YAPC_CODEGEN_UTILS_HPP