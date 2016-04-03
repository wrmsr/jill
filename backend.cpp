#include "backend.hpp"

char JVMWriter::ID = 0;

JVMWriter::JVMWriter(const llvm::DataLayout *dl, llvm::formatted_raw_ostream &o,
                     const std::string &cls, unsigned int dbg)
  : FunctionPass(ID), dataLayout(dl), out(o), classname(cls), debug(dbg) {

}

/**
 * Register required analysis information.
 *
 * @param au  AnalysisUsage object representing the analysis usage information
 *            of this pass.
 */
void JVMWriter::getAnalysisUsage(llvm::AnalysisUsage &au) const {
  au.addRequired<llvm::LoopInfo>();
  au.setPreservesAll();
}

/**
 * Process the given function.
 *
 * @param f  the function to process
 * @return   whether the function was modified (always false)
 */
bool JVMWriter::runOnFunction(llvm::Function &f) {
  if (!f.isDeclaration() && !f.hasAvailableExternallyLinkage())
    printFunction(f);
  return false;
}

/**
 * Perform per-module initialization.
 *
 * @param m  the module
 * @return   whether the module was modified (always false)
 */
bool JVMWriter::doInitialization(llvm::Module &m) {
  module = &m;
  instNum = 0;

  std::string modID = module->getModuleIdentifier();
  size_t slashPos = modID.rfind('/');
  if (slashPos == std::string::npos)
    sourcename = modID;
  else
    sourcename = modID.substr(slashPos + 1);

  if (!classname.empty()) {
    for (std::string::iterator i = classname.begin(),
           e = classname.end(); i != e; i++)
      if (*i == '.') *i = '/';
  } else {
    classname = sourcename.substr(0, sourcename.rfind('.'));
    for (std::string::iterator i = classname.begin(),
           e = classname.end(); i != e; i++)
      if (*i == '.') *i = '_';
  }

  printHeader();
  printFields();
  printExternalMethods();
  printConstructor();
  printClInit();
  printMainMethod();
  return false;
}

/**
 * Perform per-module finalization.
 *
 * @param m  the module
 * @return   whether the module was modified (always false)
 */
bool JVMWriter::doFinalization(llvm::Module &m) {
  return false;
}

