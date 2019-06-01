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
    printf("bbb");
    CodeGenUtils genContext("main");
    printf("ccc");
    auto program = context.program;
    printf("aaaa");
    program->codegen(genContext);
    //context.program->codegen(genContext);
    //context.program->codegen(genContext);
    return 0;
}