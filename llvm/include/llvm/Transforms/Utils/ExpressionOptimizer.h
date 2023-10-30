#ifndef LLVM_TRANSFORMS_UTILS_EXPRESSIONOPTIMIZER_H
#define LLVM_TRANSFORMS_UTILS_EXPRESSIONOPTIMIZER_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class ExpressionOptimizer : public PassInfoMixin<ExpressionOptimizer> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_EXPRESSIONOPTIMIZER_H
