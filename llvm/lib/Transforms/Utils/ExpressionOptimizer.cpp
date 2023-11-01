#include "llvm/Transforms/Utils/ExpressionOptimizer.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constant.h"

using namespace llvm;

void increaseIterator(BasicBlock::iterator &it, unsigned amount) {
    unsigned count = 0;
    while (count++ < amount) {
        it++;
    }
} 

bool checkSquared(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **firstSquared) {
    LoadInst *firstLoadInst = nullptr;
    LoadInst *secondLoadInst = nullptr;

    if (!(firstLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (++it == BB.end()) return false;
    
    if (!(secondLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (firstLoadInst->getPointerOperand() != secondLoadInst->getPointerOperand()) return false;
    if (++it == BB.end()) return false;
    
    Instruction &I = *it;
    if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
    if (!(I.getOperand(0) == firstLoadInst && I.getOperand(1) == secondLoadInst)) return false;

    (*firstOperand) = firstLoadInst->getPointerOperand();
    (*firstSquared) = &I;

    return true;
}

bool checkMiddle(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle) {
    LoadInst *firstLoadInst = nullptr;
    LoadInst *secondLoadInst = nullptr;
    Instruction *firstMulInst = nullptr;
    ConstantInt *constantValue = nullptr;

    if (!(firstLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    (*firstOperand) = firstLoadInst->getPointerOperand();
    if (++it == BB.end()) return false;

    secondLoadInst = dyn_cast<LoadInst>(&*it);
    if (secondLoadInst) {   // a * b * 2
        (*secondOperand) = secondLoadInst->getPointerOperand();
        if (++it == BB.end()) return false;
        
        firstMulInst = &*it;
        if (strncmp(firstMulInst->getOpcodeName(), "mul", 4)) return false;
        if (!(firstMulInst->getOperand(0) == firstLoadInst && firstMulInst->getOperand(1) == secondLoadInst)) return false;
        if (++it == BB.end()) return false;

        Instruction &I = *it;
        if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
        if (I.getOperand(0) != firstMulInst) return false;
        if (!(constantValue = dyn_cast<ConstantInt>(I.getOperand(1)))) return false;
        if (constantValue->getValue() != 2) return false;

        (*middle) = &I;

        return true;
    }
    else if (!strncmp((*it).getOpcodeName(), "mul", 4)) {   // 2 * a * b, a * 2 * b
        firstMulInst = &*it;
        if (firstMulInst->getOperand(0) == firstLoadInst) {
            if (!(constantValue = dyn_cast<ConstantInt>(firstMulInst->getOperand(1)))) return false;
            if (constantValue->getValue() != 2) return false;
        }
        else if (firstMulInst->getOperand(1) == firstLoadInst) {
            if (!(constantValue = dyn_cast<ConstantInt>(firstMulInst->getOperand(0)))) return false;
            if (constantValue->getValue() != 2) return false;
        }
        else {
            return false;
        }
        if (++it == BB.end()) return false;

        if (!(secondLoadInst = dyn_cast<LoadInst>(&*it))) return false;
        (*secondOperand) = secondLoadInst->getPointerOperand();
        if (++it == BB.end()) return false;

        Instruction &I = *it;
        if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
        if (!(I.getOperand(0) == firstMulInst && I.getOperand(1) == secondLoadInst)) return false;

        (*middle) = &I;

        return true;
    }

    return false;
}

bool checkSquaredSquared(BasicBlock& BB, BasicBlock::iterator it, Value **secondOperand, Value **secondSquared) {
    LoadInst *firstLoadInst = nullptr;
    LoadInst *secondLoadInst = nullptr;

    if (!(firstLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (++it == BB.end()) return false;

    if (!(secondLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (firstLoadInst->getPointerOperand() != secondLoadInst->getPointerOperand()) return false;
    if (++it == BB.end()) return false;

    Instruction &I = *it;
    if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
    if (!(I.getOperand(0) == firstLoadInst && I.getOperand(1) == secondLoadInst)) return false;

    (*secondOperand) = firstLoadInst->getPointerOperand();
    (*secondSquared) = &I;

    return true;
}

bool checkSquaredMiddle(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle) {
    LoadInst *firstLoadInst = nullptr;
    LoadInst *secondLoadInst = nullptr;
    Instruction *firstMulInst = nullptr;
    ConstantInt *constantValue = nullptr;

    if (!(firstLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (++it == BB.end()) return false;

    secondLoadInst = dyn_cast<LoadInst>(&*it);
    if (secondLoadInst) {   // a * b * 2
        if (!(firstLoadInst->getPointerOperand() == *firstOperand || secondLoadInst->getPointerOperand() == *firstOperand)) return false;
        if (++it == BB.end()) return false;

        firstMulInst = &*it;
        if (strncmp(firstMulInst->getOpcodeName(), "mul", 4)) return false;
        if (!(firstMulInst->getOperand(0) == firstLoadInst && firstMulInst->getOperand(1) == secondLoadInst)) return false;
        if (++it == BB.end()) return false;

        Instruction &I = *it;
        if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
        if (I.getOperand(0) != firstMulInst) return false;
        if (!(constantValue = dyn_cast<ConstantInt>(I.getOperand(1)))) return false;
        if (constantValue->getValue() != 2) return false;

        if (firstLoadInst->getPointerOperand() == *firstOperand) {
            (*secondOperand) = secondLoadInst->getPointerOperand();
        }
        else if (secondLoadInst->getPointerOperand() == *firstOperand) {
            (*secondOperand) = firstLoadInst->getPointerOperand();
        }
        (*middle) = &I;

        return true;
    }
    else if (!strncmp((*it).getOpcodeName(), "mul", 4)) {   // 2 * a * b, a * 2 * b
        firstMulInst = &*it;
        if (firstMulInst->getOperand(0) == firstLoadInst) {
            if (!(constantValue = dyn_cast<ConstantInt>(firstMulInst->getOperand(1)))) return false;
            if (constantValue->getValue() != 2) return false;
        }
        else if (firstMulInst->getOperand(1) == firstLoadInst) {
            if (!(constantValue = dyn_cast<ConstantInt>(firstMulInst->getOperand(0)))) return false;
            if (constantValue->getValue() != 2) return false;
        }
        else {
            return false;
        }
        if (++it == BB.end()) return false;

        if (!(secondLoadInst = dyn_cast<LoadInst>(&*it))) return false;
        if (!(firstLoadInst->getPointerOperand() == *firstOperand || secondLoadInst->getPointerOperand() == *firstOperand)) return false;
        if (++it == BB.end()) return false;

        Instruction &I = *it;
        if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
        if (!(I.getOperand(0) == firstMulInst && I.getOperand(1) == secondLoadInst)) return false;

        if (firstLoadInst->getPointerOperand() == *firstOperand) {
            (*secondOperand) = secondLoadInst->getPointerOperand();
        }
        else if (secondLoadInst->getPointerOperand() == *firstOperand) {
            (*secondOperand) = firstLoadInst->getPointerOperand();
        }
        (*middle) = &I;

        return true;
    }

    return false;
}

bool checkMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **firstSquared) {
    LoadInst *firstLoadInst = nullptr;
    LoadInst *secondLoadInst = nullptr;

    if (!(firstLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (!(firstLoadInst->getPointerOperand() == *firstOperand || firstLoadInst->getPointerOperand() == *secondOperand)) return false;
    if (++it == BB.end()) return false;

    if (!(secondLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (firstLoadInst->getPointerOperand() != secondLoadInst->getPointerOperand()) return false;
    if (++it == BB.end()) return false;

    Instruction &I = *it;
    if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
    if (!(I.getOperand(0) == firstLoadInst && I.getOperand(1) == secondLoadInst)) return false;
    
    if (firstLoadInst->getPointerOperand() == *secondOperand) {
        (*secondOperand) = *firstOperand;
        (*firstOperand) = firstLoadInst->getPointerOperand();
    }
    (*firstSquared) = &I;

    return true;
}

bool checkAddSquaredSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstSquared, Value **secondSquared, Value **firstAdd) {
    Instruction &I = *it;
    if (strncmp(I.getOpcodeName(), "add", 4)) return false;
    if (!((I.getOperand(0) == *firstSquared && I.getOperand(1) == *secondSquared) ||
        (I.getOperand(0) == *secondSquared && I.getOperand(1) == *firstSquared))) return false;
    
    (*firstAdd) = &I;
    
    return true;
}

bool checkAddSquaredMiddle(BasicBlock &BB, BasicBlock::iterator it, Value **firstSquared, Value **middle, Value **firstAdd) {
    Instruction &I = *it;
    if (strncmp(I.getOpcodeName(), "add", 4)) return false;
    if (!((I.getOperand(0) == *firstSquared && I.getOperand(1) == *middle) ||
        (I.getOperand(0) == *middle && I.getOperand(1) == *firstSquared))) return false;
    
    (*firstAdd) = &I;

    return true;
}

bool checkAddSquaredSquaredMiddle(BasicBlock &BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle) {
    LoadInst *firstLoadInst = nullptr;
    LoadInst *secondLoadInst = nullptr;
    Instruction *firstMulInst = nullptr;
    ConstantInt *constantValue = nullptr;

    if (!(firstLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (!(firstLoadInst->getPointerOperand() == *firstOperand || firstLoadInst->getPointerOperand() == *secondOperand)) return false;
    if (++it == BB.end()) return false;

    secondLoadInst = dyn_cast<LoadInst>(&*it);
    if (secondLoadInst) {   // a * b * 2
        if (!((firstLoadInst->getPointerOperand() == *firstOperand && secondLoadInst->getPointerOperand() == *secondOperand) ||
            (firstLoadInst->getPointerOperand() == *secondOperand && secondLoadInst->getPointerOperand() == *firstOperand))) return false;
        if (++it == BB.end()) return false;

        firstMulInst = &*it;
        if (strncmp(firstMulInst->getOpcodeName(), "mul", 4)) return false;
        if (!(firstMulInst->getOperand(0) == firstLoadInst && firstMulInst->getOperand(1) == secondLoadInst)) return false;
        if (++it == BB.end()) return false;

        Instruction &I = *it;
        if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
        if (I.getOperand(0) != firstMulInst) return false;
        if (!(constantValue = dyn_cast<ConstantInt>(I.getOperand(1)))) return false;
        if (constantValue->getValue() != 2) return false;

        (*middle) = &I;

        return true;
    }
    else if (!strncmp((*it).getOpcodeName(), "mul", 4)) {   // 2 * a * b, a * 2 * b
        firstMulInst = &*it;
        if (firstMulInst->getOperand(0) == firstLoadInst) {
            if (!(constantValue = dyn_cast<ConstantInt>(firstMulInst->getOperand(1)))) return false;
            if (constantValue->getValue() != 2) return false;
        }
        else if (firstMulInst->getOperand(1) == firstLoadInst) {
            if (!(constantValue = dyn_cast<ConstantInt>(firstMulInst->getOperand(0)))) return false;
            if (constantValue->getValue() != 2) return false;
        }
        else {
            return false;
        }
        if (++it == BB.end()) return false;

        if (!(secondLoadInst = dyn_cast<LoadInst>(&*it))) return false;
        if (!((firstLoadInst->getPointerOperand() == *firstOperand && secondLoadInst->getPointerOperand() == *secondOperand) ||
            (firstLoadInst->getPointerOperand() == *secondOperand && secondLoadInst->getPointerOperand() == *firstOperand))) return false;
        if (++it == BB.end()) return false;

        Instruction &I = *it;
        if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
        if (!(I.getOperand(0) == firstMulInst && I.getOperand(1) == secondLoadInst)) return false;

        (*middle) = &I;

        return true;
    }

    return false;
}

bool checkAddSquaredMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **secondOperand, Value **secondSquared) {
    LoadInst *firstLoadInst = nullptr;
    LoadInst *secondLoadInst = nullptr;

    if (!(firstLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (!(firstLoadInst->getPointerOperand() == *secondOperand)) return false;
    if (++it == BB.end()) return false;

    if (!(secondLoadInst = dyn_cast<LoadInst>(&*it))) return false;
    if (firstLoadInst->getPointerOperand() != secondLoadInst->getPointerOperand()) return false;
    if (++it == BB.end()) return false;

    Instruction &I = *it;
    if (strncmp(I.getOpcodeName(), "mul", 4)) return false;
    if (!(I.getOperand(0) == firstLoadInst && I.getOperand(1) == secondLoadInst)) return false;

    (*secondSquared) = &I;

    return true;
}

bool checkAddAddSquaredMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstAdd, Value **secondAdd, Value **finalInst) {
    Instruction &I = *it;
    if (strncmp(I.getOpcodeName(), "add", 4)) return false;
    if (!((I.getOperand(0) == *firstAdd && I.getOperand(1) == *secondAdd) ||
        (I.getOperand(0) == *secondAdd && I.getOperand(1) == *firstAdd))) return false;
    
    (*finalInst) = &I;

    return true;
}

PreservedAnalyses ExpressionOptimizer::run(Function &F, FunctionAnalysisManager &AM) {
    for (Function::iterator itF = F.begin(); itF != F.end(); itF++) {
        
        BasicBlock &BB = *itF;
        ExpressionOptimizerState state = ExpressionOptimizerState::Initial;

        BasicBlock::iterator beginningIt;
        BasicBlock::iterator endIt;
        Value *firstSquared = nullptr;
        Value *firstOperand = nullptr;
        Value *secondSquared = nullptr;
        Value *secondOperand = nullptr;
        Value *middle = nullptr;
        Value *firstAdd = nullptr;
        Value *secondAdd = nullptr;
        Value *finalInst = nullptr;
        
        for (BasicBlock::iterator itBB = BB.begin(); itBB != BB.end();) {

            switch (state) {
            case ExpressionOptimizerState::Initial:
                beginningIt = itBB;
                if (checkMiddle(BB, itBB, &firstOperand, &secondOperand, &middle)) {
                    increaseIterator(itBB, 3);
                    state = ExpressionOptimizerState::Middle;
                }
                else if (checkSquared(BB, itBB, &firstOperand, &firstSquared)) {
                    increaseIterator(itBB, 2);
                    state = ExpressionOptimizerState::Squared;
                }
                break;
            case ExpressionOptimizerState::Squared:
                if (checkSquaredMiddle(BB, itBB, &firstOperand, &secondOperand, &middle)) {
                    increaseIterator(itBB, 3);
                    state = ExpressionOptimizerState::SquaredMiddle;
                }
                else if (checkSquaredSquared(BB, itBB, &secondOperand, &secondSquared)) {
                    increaseIterator(itBB, 2);
                    state = ExpressionOptimizerState::SquaredSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::Middle:
                if (checkMiddleSquared(BB, itBB, &firstOperand, &secondOperand, &firstSquared)) {
                    increaseIterator(itBB, 2);
                    state = ExpressionOptimizerState::SquaredMiddle;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::SquaredSquared:
                if (checkAddSquaredSquared(BB, itBB, &firstSquared, &secondSquared, &firstAdd)) {
                    state = ExpressionOptimizerState::AddSquaredSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::SquaredMiddle:
                if (checkAddSquaredMiddle(BB, itBB, &firstSquared, &middle, &firstAdd)) {
                    state = ExpressionOptimizerState::AddSquaredMiddle;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::AddSquaredSquared:
                if (checkAddSquaredSquaredMiddle(BB, itBB, &firstOperand, &secondOperand, &secondAdd)) {
                    increaseIterator(itBB, 3);
                    state = ExpressionOptimizerState::AddSquaredMiddleSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::AddSquaredMiddle:
                if (checkAddSquaredMiddleSquared(BB, itBB, &secondOperand, &secondAdd)) {
                    increaseIterator(itBB, 2);
                    state = ExpressionOptimizerState::AddSquaredMiddleSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::AddSquaredMiddleSquared:
                if (checkAddAddSquaredMiddleSquared(BB, itBB, &firstAdd, &secondAdd, &finalInst)) {
                    endIt = itBB++;

                    LoadInst *newLoadInstFirstOperand = new LoadInst(IntegerType::get((*beginningIt).getContext(), 32), firstOperand, "", &*beginningIt);
                    LoadInst *newLoadInstSecondOperand = new LoadInst(IntegerType::get((*beginningIt).getContext(), 32), secondOperand, "", &*beginningIt);
                    IRBuilder<> Builder(&*beginningIt);
                    Value *newAddInst = Builder.CreateAdd(newLoadInstFirstOperand, newLoadInstSecondOperand, "", false, true);
                    Value *newMulInst = Builder.CreateMul(newAddInst, newAddInst, "", false, true);

                    finalInst->replaceAllUsesWith(newMulInst);

                    while (endIt != beginningIt) {
                        BasicBlock::iterator currentIt = endIt--;
                        (*currentIt).eraseFromParent();
                    }
                    (*beginningIt).eraseFromParent();

                    state = ExpressionOptimizerState::Initial;
                    continue;   // needed because itBB is already increased
                }
                state = ExpressionOptimizerState::Initial;
                break;
            }

            itBB++;
        }
    }
    return PreservedAnalyses::all();
}