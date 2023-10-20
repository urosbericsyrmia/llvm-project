#include "llvm/Transforms/Utils/DebugDelete.h"

using namespace llvm;

PreservedAnalyses DebugDelete::run(Function &F, FunctionAnalysisManager &AM) {
    errs() << F.getName() << "\n";
    return PreservedAnalyses::all();
}