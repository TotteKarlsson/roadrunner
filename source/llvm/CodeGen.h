/*
 * CodeGen.h
 *
 *  Created on: Jul 2, 2013
 *      Author: andy
 */

#ifndef CodeGenH
#define CodeGenH

#include "LLVMIncludes.h"

namespace rr
{

class CodeGen
{
public:
    virtual llvm::Value *codeGen() = 0;
protected:
    ~CodeGen() {};
};

class LoadSymbolResolver
{
public:
    virtual llvm::Value *loadSymbolValue(const std::string& symbol) = 0;
protected:
    virtual ~LoadSymbolResolver() {};
};

class LLVMStoreSymbolResolver
{
public:
    virtual llvm::Value *storeSymbolValue(const std::string& symbol,
            llvm::Value *value) = 0;
protected:
    ~LLVMStoreSymbolResolver() {};
};

} /* namespace rr */
#endif /* LLVMCodeGenH */