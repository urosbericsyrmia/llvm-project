#ifndef LLVM_TRANSFORMS_UTILS_DEBUGCOUNT_H
#define LLVM_TRANSFORMS_UTILS_DEBUGCOUNT_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class DebugCount : public PassInfoMixin<DebugCount> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm







#endif // LLVM_TRANSFORMS_UTILS_DEBUGCOUNT_H
