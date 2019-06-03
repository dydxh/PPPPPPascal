#include <iostream>
#include <cstdio>
#include <cstring>
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



void emit_target(llvm::raw_fd_ostream &dest, llvm::TargetMachine::CodeGenFileType type, llvm::Module &module)
{
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
    { llvm::errs() << error; exit(1); }

    auto cpu = "generic";
    auto features = "";
    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto target_machine = target->createTargetMachine(target_triple, cpu, features, opt, rm);
    module.setDataLayout(target_machine->createDataLayout());

    llvm::legacy::PassManager pass;
    if (target_machine->addPassesToEmitFile(pass, dest, nullptr, type))
    { llvm::errs() << "The target machine cannot emit an object file"; exit(1); }

    std::cout << "flag1" << std::endl;
    pass.run(module);
    std::cout << "flag2" << std::endl;

    dest.flush();
}

int main(const int argc, const char** argv)
{
    enum class Target
    { UNDEFINED, LLVM, ASM, OBJ };

    if(argc < 3) {
        std::cout << "[Usage]: ./[elf] [ir/asm/obj] [source]" << std::endl;
        return 0;
    }

    Target target = Target::UNDEFINED;
    if (strcmp(argv[1], "ir") == 0) target = Target::LLVM;
    else if (strcmp(argv[1], "asm") == 0) target = Target::ASM;
    else if (strcmp(argv[1], "obj") == 0) target = Target::OBJ;

    yapc::Driver* driver;
    yapc::Context context;
    try {
        driver = new yapc::Driver(context);
    }
    catch(std::bad_alloc& msg) {
        std::cerr << "Failed to allocate driver: (" << msg.what() << "), exit." << std::endl;
        return false;
    }

    driver->Parse(argv[2]);
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
    std::cout << "\n\n>>>>>>>>>>>>>>>==========  IR over!==========<<<<<<<<<<<<<<<" << std::endl;

    std::string output = argv[2];
    output.erase(output.rfind('.'));
    switch (target)
    {
        case Target::LLVM: output.append(".ll"); break;
        case Target::ASM:  output.append(".s");  break;
        case Target::OBJ:  output.append(".o");  break;
        default: break;
    }
    std::error_code ec;
    llvm::raw_fd_ostream fd(output, ec, llvm::sys::fs::F_None);
    if (ec)
    { llvm::errs() << "Could not open file: " << ec.message(); exit(1); }

    switch (target)
    {
        case Target::LLVM: genContext.GetModule()->print(fd, nullptr); break;
        case Target::ASM: emit_target(fd, llvm::TargetMachine::CGFT_AssemblyFile, *(genContext.GetModule())); break;
        case Target::OBJ: emit_target(fd, llvm::TargetMachine::CGFT_ObjectFile, *(genContext.GetModule())); break;
        default: break;
    }


    return 0;
}