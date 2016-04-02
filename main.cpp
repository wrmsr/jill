#include <iostream>

#include <llvm/IR/LLVMContext.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/IR/Module.h>

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

  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> bufOrErr = llvm::MemoryBuffer::getFileOrSTDIN(input);
  if (bufOrErr.getError()) {
    std::cerr << "Unable to open bitcode file: " << bufOrErr.getError().message() << std::endl;
    return 1;
  }
  std::unique_ptr<llvm::MemoryBuffer> buf = std::move(bufOrErr.get());

  llvm::ErrorOr<llvm::Module *> modOrErr = llvm::parseBitcodeFile(buf->getMemBufferRef(), llvm::getGlobalContext());
  if (!modOrErr) {
    std::cerr << "Unable to parse bitcode file: " << modOrErr.getError().message() << std::endl;
    return 1;
  }

  return 0;
}

