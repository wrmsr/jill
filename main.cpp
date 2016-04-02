#include <iostream>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/PassManager.h>

static llvm::cl::opt<std::string> input(llvm::cl::Positional, llvm::cl::desc("<input bitcode>"), llvm::cl::init("-"));
static llvm::cl::opt<std::string> classname("classname", llvm::cl::desc("Binary name of the generated class"));

enum DebugLevel {
  g0 = 0,
  g1 = 1,
  g2 = 2,
  g3 = 3
};

llvm::cl::opt<DebugLevel> debugLevel(
  llvm::cl::desc("Debugging level:"),
  llvm::cl::init(g1),
  llvm::cl::values(
    clEnumValN(g2, "g", "Same as -g2"),
    clEnumVal(g0, "No debugging information"),
    clEnumVal(g1, "Source file and line number information (default)"),
    clEnumVal(g2, "-g1 + Local variable information"),
    clEnumVal(g3, "-g2 + Commented LLVM assembly"),
    clEnumValEnd));

int main(int argc, const char *const *argv) {
  llvm::cl::ParseCommandLineOptions(argc, argv, "LLJVM Backend\n");

  auto bufOrErr = llvm::MemoryBuffer::getFileOrSTDIN(input);
  if (bufOrErr.getError()) {
    std::cerr << "Unable to open bitcode file: " << bufOrErr.getError().message() << std::endl;
    return 1;
  }
  auto buf = std::move(bufOrErr.get());

  auto modOrErr = llvm::parseBitcodeFile(buf->getMemBufferRef(), llvm::getGlobalContext());
  if (!modOrErr) {
    std::cerr << "Unable to parse bitcode file: " << modOrErr.getError().message() << std::endl;
    return 1;
  }
  auto *mod = std::move(modOrErr.get());

  llvm::DataLayout dl(
    "e-p:32:32:32"
      "-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64"
      "-f32:32:32-f64:64:64");
  mod->setDataLayout(&dl);

  llvm::PassManager pm;
  pm.add(new llvm::DataLayoutPass());

  pm.run(*mod);

  return 0;
}

