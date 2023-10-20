#include "llvm/Transforms/Utils/DebugDelete.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/ADT/simple_ilist.h"
#include "llvm/ADT/ilist.h"

using namespace llvm;

PreservedAnalyses DebugDelete::run(Function& F, FunctionAnalysisManager& AM) {
    for (Function::iterator it_f = F.begin(); it_f != F.end(); it_f++) {
        BasicBlock& BB = *it_f;
        for (BasicBlock::iterator it_bb = BB.begin(); it_bb != BB.end();) {
            Instruction& I = *it_bb;
            it_bb++;
            if (isa<DbgInfoIntrinsic>(&I) && I.isSafeToRemove()) {
                I.eraseFromParent();
            }
        }
    }
    return PreservedAnalyses::all();
}