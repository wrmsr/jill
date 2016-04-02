#include "backend.hpp"

char JVMWriter::ID;

JVMWriter::JVMWriter(const llvm::DataLayout *dl, llvm::formatted_raw_ostream &o,
                     const std::string &cls, unsigned int dbg)
  : FunctionPass(ID), dataLayout(dl), out(o), classname(cls), debug(dbg) {

}

bool JVMWriter::doInitialization(llvm::Module &module) {
  return llvm::Pass::doInitialization(module);
}

bool JVMWriter::runOnFunction(llvm::Function &F) {
  return false;
}

void JVMWriter::getAnalysisUsage(llvm::AnalysisUsage &usage) const {
  llvm::Pass::getAnalysisUsage(usage);
}

bool JVMWriter::doFinalization(llvm::Module &module) {
  return llvm::Pass::doFinalization(module);
}

