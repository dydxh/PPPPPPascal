#include <iostream>
#include <cstdio>
#include <string>
#include <utility>
#include "Driver.hpp"
#include "utils/ASTvis.hpp"


#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
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
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include "utils/ast.hpp"
#include "codeGen/codeGen_utils.hpp"



/*
void OutputResult(llvm::raw_fd_ostream &dest, llvm::TargetMachine::CodeGenFileType type, llvm::Module &module) {
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto target_triple = llvm::sys::getDefaultTargetTriple();
    module.setTargetTriple(target_triple);

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);
    if (!target)
    { std::cout << error; exit(1); }

    auto cpu = "generic";
    auto features = "";
    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto target_machine = target->createTargetMachine(target_triple, cpu, features, opt, rm);
    module.setDataLayout(target_machine->createDataLayout());

    llvm::legacy::PassManager pass;
    if (target_machine->addPassesToEmitFile(pass, dest, nullptr, type))
    { std::cout << "The target machine cannot emit an object file"; exit(1); }

    pass.run(module);
    dest.flush();
}
 */

int main(const int argc, const char** argv) {


    if(argc < 2) {
        std::cout << "[Usage]: ./[elf] [source]" << std::endl;
        return 0;
    }



    yapc::Driver* driver;
    yapc::Context context;
    try {
        driver = new yapc::Driver(context);
    }
    catch(std::bad_alloc& msg) {
        std::cerr << "Failed to allocate driver: (" << msg.what() << "), exit." << std::endl;
        return false;
    }

    driver->Parse(argv[1]);
    yapc::ASTvis astVis;
    astVis.travAST(context);
    CodeGenUtils genContext("main");
    auto program = context.program;
    try {
        program->codegen(genContext);
    } catch (CodegenException &e) {
        std::cout << e.what() << std::endl;
    }

    genContext.dump();


    return 0;
}