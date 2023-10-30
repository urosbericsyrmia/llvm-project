#include "llvm/Transforms/Utils/ExpressionOptimizer.h"

using namespace llvm;

PreservedAnalyses ExpressionOptimizer::run(Function &F, FunctionAnalysisManager &AM) {
    outs() << F.getName() << "\n";
    return PreservedAnalyses::all();
}