#pragma once

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/Analysis/ConstantsScanner.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Pass.h>


class JVMWriter : public llvm::FunctionPass {
private:
  static char ID;

  llvm::formatted_raw_ostream &out;
  std::string sourcename;
  std::string classname;
  unsigned int debug;
  llvm::Module *module;
  const llvm::DataLayout *dataLayout;
  static char id;

  llvm::DenseSet<const llvm::Value*> externRefs;
  llvm::DenseMap<const llvm::BasicBlock*, unsigned int> blockIDs;
  llvm::DenseMap<const llvm::Value*, unsigned int> localVars;
  unsigned int usedRegisters;
  unsigned int vaArgNum;
  unsigned int instNum;

public:
  JVMWriter(const llvm::DataLayout *dl, llvm::formatted_raw_ostream &o,
            const std::string &cls, unsigned int dbg);

  virtual bool doInitialization(llvm::Module &module) override;
  virtual bool runOnFunction(llvm::Function &F) override;
  virtual void getAnalysisUsage(llvm::AnalysisUsage &usage) const override;
  virtual bool doFinalization(llvm::Module &module) override;

   // block.cpp
    void printBasicBlock(const llvm::BasicBlock *block);
    void printInstruction(const llvm::Instruction *inst);

    // branch.cpp
    void printPHICopy(const llvm::BasicBlock *src, const llvm::BasicBlock *dest);
    void printBranchInstruction(const llvm::BasicBlock *curBlock,
                                const llvm::BasicBlock *destBlock);
    void printBranchInstruction(const llvm::BasicBlock *curBlock,
                                const llvm::BasicBlock *trueBlock,
                                const llvm::BasicBlock *falseBlock);
    void printBranchInstruction(const BranchInst *inst);
    void printSelectInstruction(const llvm::Value *cond,
                                const llvm::Value *trueVal,
                                const llvm::Value *falseVal);
    void printSwitchInstruction(const SwitchInst *inst);
    void printLoop(const Loop *l);

    // const.cpp
    void printPtrLoad(uint64_t n);
    void printConstLoad(const APInt &i);
    void printConstLoad(float f);
    void printConstLoad(double d);
    void printConstLoad(const llvm::Constant *c);
    void printConstLoad(const std::string &str, bool cstring);
    void printStaticConstant(const llvm::Constant *c);
    void printConstantExpr(const ConstantExpr *ce);

    // function.cpp
    std::string getCallSignature(const llvm::FunctionType *ty);
    void printOperandPack(const Instruction *inst,
                          unsigned int minOperand,
                          unsigned int maxOperand);
    void printFunctionCall(const llvm::Value *functionVal, const Instruction *inst);
    void printIntrinsicCall(const IntrinsicInst *inst);
    void printCallInstruction(const Instruction *inst);
    void printInvokeInstruction(const InvokeInst *inst);
    void printLocalVariable(const llvm::Function &f, const Instruction *inst);
    void printFunctionBody(const llvm::Function &f);
    unsigned int getLocalVarNumber(const llvm::Value *v);
    void printCatchJump(unsigned int numJumps);
    void printFunction(const llvm::Function &f);

    // instruction.cpp
    void printCmpInstruction(unsigned int predicate,
                             const llvm::Value *left,
                             const llvm::Value *right);
    void printArithmeticInstruction(unsigned int op,
                                    const llvm::Value *left,
                                    const llvm::Value *right);
    void printBitCastInstruction(const llvm::Type *ty, const llvm::Type *srcTy);
    void printCastInstruction(const std::string &typePrefix,
                              const std::string &srcTypePrefix);
    void printCastInstruction(unsigned int op, const llvm::Value *v,
                              const llvm::Type *ty, const llvm::Type *srcTy);
    void printGepInstruction(const llvm::Value *v,
                             gep_type_iterator i,
                             gep_type_iterator e);
    void printAllocaInstruction(const AllocaInst *inst);
    void printVAArgInstruction(const VAArgInst *inst);
    void printVAIntrinsic(const IntrinsicInst *inst);
    void printMemIntrinsic(const MemIntrinsic *inst);
    void printMathIntrinsic(const IntrinsicInst *inst);
    void printBitIntrinsic(const IntrinsicInst *inst);

    // loadstore.cpp
    void printValueLoad(const llvm::Value *v);
    void printValueStore(const llvm::Value *v);
    void printIndirectLoad(const llvm::Value *v);
    void printIndirectLoad(const llvm::Type *ty);
    void printIndirectStore(const llvm::Value *ptr, const llvm::Value *val);
    void printIndirectStore(const llvm::Type *ty);

    // name.cpp
    std::string sanitizeName(std::string name);
    std::string getValueName(const llvm::Value *v);
    std::string getLabelName(const BasicBlock *block);

    // printinst.cpp
    void printBinaryInstruction(const char *name,
                                const llvm::Value *left,
                                const llvm::Value *right);
    void printBinaryInstruction(const std::string &name,
                                const llvm::Value *left,
                                const llvm::Value *right);
    void printSimpleInstruction(const char *inst);
    void printSimpleInstruction(const char *inst, const char *operand);
    void printSimpleInstruction(const std::string &inst);
    void printSimpleInstruction(const std::string &inst,
                                const std::string &operand);
    void printVirtualInstruction(const char *sig);
    void printVirtualInstruction(const char *sig, const llvm::Value *operand);
    void printVirtualInstruction(const char *sig,
                                 const llvm::Value *left,
                                 const llvm::Value *right);
    void printVirtualInstruction(const std::string &sig);
    void printVirtualInstruction(const std::string &sig, const llvm::Value *operand);
    void printVirtualInstruction(const std::string &sig,
                                 const llvm::Value *left,
                                 const llvm::Value *right);
    void printLabel(const char *label);
    void printLabel(const std::string &label);

    // sections.cpp
    void printHeader();
    void printFields();
    void printExternalMethods();
    void printConstructor();
    void printClInit();
    void printMainMethod();

    // types.cpp
    unsigned int getBitWidth(     const llvm::Type *ty, bool expand = false);
    char getTypeID(               const llvm::Type *ty, bool expand = false);
    std::string getTypeName(      const llvm::Type *ty, bool expand = false);
    std::string getTypeDescriptor(const llvm::Type *ty, bool expand = false);
    std::string getTypePostfix(   const llvm::Type *ty, bool expand = false);
    std::string getTypePrefix(    const llvm::Type *ty, bool expand = false)
};
