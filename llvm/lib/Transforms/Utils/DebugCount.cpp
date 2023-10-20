#include "llvm/Transforms/Utils/DebugCount.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Support/Casting.h"

using namespace llvm;

PreservedAnalyses DebugCount::run(Function &F, FunctionAnalysisManager &AM) {
    unsigned count_dbg_declare = 0;
    unsigned count_dbg_value = 0;
    unsigned count_dbg_label = 0;
    unsigned count_dbg_assign = 0;

    for (Function::iterator it_f = F.begin(); it_f != F.end(); it_f ++) {
        BasicBlock& BB = *it_f;
        for (BasicBlock::iterator it_bb = BB.begin(); it_bb != BB.end(); it_bb++) {
            Instruction& I = *it_bb;

            if (isa<DbgInfoIntrinsic>(&I)) {
                DbgInfoIntrinsic& DII = cast<DbgInfoIntrinsic>(I);
                
                if (isa<DbgAssignIntrinsic>(&DII)) {
                    count_dbg_assign++;
                }
                else if (isa<DbgDeclareInst>(&DII)) {
                    count_dbg_declare++;
                }     
                else if (isa<DbgValueInst>(&DII)) {
                    count_dbg_value++;
                }
                else if (isa<DbgLabelInst>(&DII)) {
                    count_dbg_label++;
                }
            }
        }
    }

    errs() << "Function: " << F.getName() << "\n";
    errs() << "\tllvm.dbg.declare: " << count_dbg_declare << "\n";
    errs() << "\tllvm.dbg.value: " << count_dbg_value << "\n";
    errs() << "\tllvm.dbg.label: " << count_dbg_label << "\n";
    errs() << "\tllvm.dbg.assign: " << count_dbg_assign << "\n";
 
    return PreservedAnalyses::all();
}