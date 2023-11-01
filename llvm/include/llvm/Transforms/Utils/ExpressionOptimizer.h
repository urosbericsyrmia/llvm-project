#ifndef LLVM_TRANSFORMS_UTILS_EXPRESSIONOPTIMIZER_H
#define LLVM_TRANSFORMS_UTILS_EXPRESSIONOPTIMIZER_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class ExpressionOptimizer : public PassInfoMixin<ExpressionOptimizer> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

enum class ExpressionOptimizerState {
    Initial = 0,
    Squared,
    Middle,
    SquaredSquared,
    SquaredMiddle,
    AddSquaredSquared,
    AddSquaredMiddle,
    AddSquaredMiddleSquared
};

} // namespace llvm

using namespace llvm;

void increaseIterator(BasicBlock::iterator &it, unsigned amount);
bool checkSquared(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **firstSquared);
bool checkMiddle(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle);
bool checkSquaredSquared(BasicBlock& BB, BasicBlock::iterator it, Value **secondOperand, Value **secondSquared);
bool checkSquaredMiddle(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle);
bool checkMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **firstSquared);
bool checkAddSquaredSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstSquared, Value **secondSquared, Value **firstAdd);
bool checkAddSquaredMiddle(BasicBlock &BB, BasicBlock::iterator it, Value **firstSquared, Value **middle, Value **firstAdd);
bool checkAddSquaredSquaredMiddle(BasicBlock &BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle);
bool checkAddSquaredMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **secondOperand, Value **secondSquared);
bool checkAddAddSquaredMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstAdd, Value **secondAdd, Value **finalInst);

#endif // LLVM_TRANSFORMS_UTILS_EXPRESSIONOPTIMIZER_H
