#ifndef LLVM_TRANSFORMS_UTILS_DEBUGDELETE_H
#define LLVM_TRANSFORMS_UTILS_DEBUGDELETE_H

#include "llvm/IR/PassManager.h"

namespace llvm {
    
class DebugDelete : public PassInfoMixin<DebugDelete> {
public:
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_DEBUGDELETE_H