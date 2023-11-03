#include "llvm/Transforms/Utils/ExpressionOptimizer.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/PatternMatch.h"

using namespace llvm;
using namespace PatternMatch;

void increaseIterator(BasicBlock::iterator &it, unsigned amount) {
    unsigned count = 0;
    while (count++ < amount) {
        it++;
    }
} 

bool checkSquared(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **firstSquared) {
    Value *operand;
    Value *firstLoadInst;
    Value *secondLoadInst;
    Value *mulInst;

    firstLoadInst = &*it;
    if (!match(firstLoadInst, m_Load(m_Value(operand)))) return false;
    if (++it == BB.end()) return false;

    secondLoadInst = &*it;
    if (!match(secondLoadInst, m_Load(m_Specific(operand)))) return false;
    if (++it == BB.end()) return false;

    mulInst = &*it;
    if (!match(mulInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
    
    *firstOperand = operand;
    *firstSquared = mulInst;
    
    return true;
}

bool checkMiddle(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle) {
    Value *firstValue;
    Value *secondValue;
    Value *firstLoadInst;
    Value *secondLoadInst;
    Value *firstMulInst;
    Value *secondMulInst;
    Value *currentInst;

    currentInst = &*it;
    if (!match(currentInst, m_Load(m_Value(firstValue)))) return false;
    firstLoadInst = currentInst;
    if (++it == BB.end()) return false;

    currentInst = &*it;
    if (match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstLoadInst)))) {   //  2 * a * b , a * 2 * b
        firstMulInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (!match(currentInst, m_Load(m_Value(secondValue)))) return false;
        secondLoadInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (!match(currentInst, m_c_Mul(m_Specific(firstMulInst), m_Specific(secondLoadInst)))) return false;
        secondMulInst = currentInst;
    }
    else if (match(currentInst, m_Load(m_Value(secondValue)))) {    //  a * b * 2
        secondLoadInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (!match(currentInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
        firstMulInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (!match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstMulInst)))) return false;
        secondMulInst = currentInst;
    }
    else {
        return false;
    }

    *firstOperand = firstValue;
    *secondOperand = secondValue;
    *middle = secondMulInst;

    return true;
}

bool checkSquaredSquared(BasicBlock& BB, BasicBlock::iterator it, Value **secondOperand, Value **secondSquared) {
    Value *operand;
    Value *firstLoadInst;
    Value *secondLoadInst;
    Value *mulInst;

    firstLoadInst = &*it;
    if (!match(firstLoadInst, m_Load(m_Value(operand)))) return false;
    if (++it == BB.end()) return false;

    secondLoadInst = &*it;
    if (!match(secondLoadInst, m_Load(m_Specific(operand)))) return false;
    if (++it == BB.end()) return false;

    mulInst = &*it;
    if (!match(mulInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
    
    *secondOperand = operand;
    *secondSquared = mulInst;
    
    return true;
}

bool checkSquaredMiddle(BasicBlock& BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle) {
    Value *secondValue;
    Value *firstLoadInst;
    Value *secondLoadInst;
    Value *firstMulInst;
    Value *secondMulInst;
    Value *currentInst;

    currentInst = &*it;
    if (match(currentInst, m_Load(m_Specific(*firstOperand)))) {
        firstLoadInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstLoadInst)))) {
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_Load(m_Value(secondValue)))) return false;
            secondLoadInst =  currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstMulInst), m_Specific(secondLoadInst)))) return false;
            secondMulInst = currentInst;
        }
        else if (match(currentInst, m_Load(m_Value(secondValue)))) {
            secondLoadInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstMulInst)))) return false;
            secondMulInst = currentInst;
        }
        else {
            return false;
        }
    }
    else if (match(currentInst, m_Load(m_Value(secondValue)))) {
        firstLoadInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstLoadInst)))) {
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_Load(m_Specific(*firstOperand)))) return false;
            secondLoadInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstMulInst), m_Specific(secondLoadInst)))) return false;
            secondMulInst = currentInst;
        }
        else if (match(currentInst, m_Load(m_Specific(*firstOperand)))) {
            secondLoadInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstMulInst)))) return false;
            secondMulInst = currentInst;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }

    *secondOperand = secondValue;
    *middle = secondMulInst;

    return true;
}

bool checkMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **firstSquared) {
    Value *firstLoadInst;
    Value *secondLoadInst;
    Value *mulInst;

    firstLoadInst = &*it;
    if (match(firstLoadInst, m_Load(m_Specific(*firstOperand)))) {
        if (++it == BB.end()) return false;

        secondLoadInst = &*it;
        if (!match(secondLoadInst, m_Load(m_Specific(*firstOperand)))) return false;
        if (++it == BB.end()) return false;

        mulInst = &*it;
        if (!match(mulInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
    }
    else if (match(firstLoadInst, m_Load(m_Specific(*secondOperand)))) {
        if (++it == BB.end()) return false;

        secondLoadInst = &*it;
        if (!match(secondLoadInst, m_Load(m_Specific(*secondOperand)))) return false;
        if (++it == BB.end()) return false;

        mulInst = &*it;
        if (!match(mulInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;

        Value *temp = *firstOperand;
        *firstOperand = *secondOperand;
        *secondOperand = temp;
    }
    else {
        return false;
    }

    *firstSquared = mulInst;
    
    return true;
}

bool checkAddSquaredSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstSquared, Value **secondSquared, Value **firstAdd) {
    Value *addition;

    addition = &*it;
    if (!match(addition, m_c_Add(m_Specific(*firstSquared), m_Specific(*secondSquared)))) return false;

    *firstAdd = addition;

    return true;
}

bool checkAddSquaredMiddle(BasicBlock &BB, BasicBlock::iterator it, Value **firstSquared, Value **middle, Value **firstAdd) {
    Value *addition;

    addition = &*it;
    if (!match(addition, m_c_Add(m_Specific(*firstSquared), m_Specific(*middle)))) return false;

    *firstAdd = addition;

    return true;
}

bool checkAddSquaredSquaredMiddle(BasicBlock &BB, BasicBlock::iterator it, Value **firstOperand, Value **secondOperand, Value **middle) {
    Value *firstLoadInst;
    Value *secondLoadInst;
    Value *firstMulInst;
    Value *secondMulInst;
    Value *currentInst;

    currentInst = &*it;
    if (match(currentInst, m_Load(m_Specific(*firstOperand)))) {
        firstLoadInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstLoadInst)))) {
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_Load(m_Specific(*secondOperand)))) return false;
            secondLoadInst =  currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstMulInst), m_Specific(secondLoadInst)))) return false;
            secondMulInst = currentInst;
        }
        else if (match(currentInst, m_Load(m_Specific(*secondOperand)))) {
            secondLoadInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstMulInst)))) return false;
            secondMulInst = currentInst;
        }
        else {
            return false;
        }
    }
    else if (match(currentInst, m_Load(m_Specific(*secondOperand)))) {
        firstLoadInst = currentInst;
        if (++it == BB.end()) return false;

        currentInst = &*it;
        if (match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstLoadInst)))) {
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_Load(m_Specific(*firstOperand)))) return false;
            secondLoadInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstMulInst), m_Specific(secondLoadInst)))) return false;
            secondMulInst = currentInst;
        }
        else if (match(currentInst, m_Load(m_Specific(*firstOperand)))) {
            secondLoadInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;
            firstMulInst = currentInst;
            if (++it == BB.end()) return false;

            currentInst = &*it;
            if (!match(currentInst, m_c_Mul(m_SpecificInt(2), m_Specific(firstMulInst)))) return false;
            secondMulInst = currentInst;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }

    *middle = secondMulInst;

    return true;
}

bool checkAddSquaredMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **secondOperand, Value **secondSquared) {
    Value *firstLoadInst;
    Value *secondLoadInst;
    Value *mulInst;

    firstLoadInst = &*it;
    if (!match(firstLoadInst, m_Load(m_Specific(*secondOperand)))) return false;
    if (++it == BB.end()) return false;

    secondLoadInst = &*it;
    if (!match(secondLoadInst, m_Load(m_Specific(*secondOperand)))) return false;
    if (++it == BB.end()) return false;

    mulInst = &*it;
    if (!match(mulInst, m_c_Mul(m_Specific(firstLoadInst), m_Specific(secondLoadInst)))) return false;

    *secondSquared = mulInst;

    return true;
}

bool checkAddAddSquaredMiddleSquared(BasicBlock &BB, BasicBlock::iterator it, Value **firstAdd, Value **secondAdd, Value **finalInst) {
    Value *addition;

    addition = &*it;
    if (!match(addition, m_c_Add(m_Specific(*firstAdd), m_Specific(*secondAdd)))) return false;

    *finalInst = addition;

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
                    increaseIterator(itBB, 4);
                    state = ExpressionOptimizerState::Middle;
                }
                else if (checkSquared(BB, itBB, &firstOperand, &firstSquared)) {
                    increaseIterator(itBB, 3);
                    state = ExpressionOptimizerState::Squared;
                }
                else {
                    increaseIterator(itBB, 1);
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::Squared:
                if (checkSquaredMiddle(BB, itBB, &firstOperand, &secondOperand, &middle)) {
                    increaseIterator(itBB, 4);
                    state = ExpressionOptimizerState::SquaredMiddle;
                }
                else if (checkSquaredSquared(BB, itBB, &secondOperand, &secondSquared)) {
                    increaseIterator(itBB, 3);
                    state = ExpressionOptimizerState::SquaredSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::Middle:
                if (checkMiddleSquared(BB, itBB, &firstOperand, &secondOperand, &firstSquared)) {
                    increaseIterator(itBB, 3);
                    state = ExpressionOptimizerState::SquaredMiddle;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::SquaredSquared:
                if (checkAddSquaredSquared(BB, itBB, &firstSquared, &secondSquared, &firstAdd)) {
                    increaseIterator(itBB, 1);
                    state = ExpressionOptimizerState::AddSquaredSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::SquaredMiddle:
                if (checkAddSquaredMiddle(BB, itBB, &firstSquared, &middle, &firstAdd)) {
                    increaseIterator(itBB, 1);
                    state = ExpressionOptimizerState::AddSquaredMiddle;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::AddSquaredSquared:
                if (checkAddSquaredSquaredMiddle(BB, itBB, &firstOperand, &secondOperand, &secondAdd)) {
                    increaseIterator(itBB, 4);
                    state = ExpressionOptimizerState::AddSquaredMiddleSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::AddSquaredMiddle:
                if (checkAddSquaredMiddleSquared(BB, itBB, &secondOperand, &secondAdd)) {
                    increaseIterator(itBB, 3);
                    state = ExpressionOptimizerState::AddSquaredMiddleSquared;
                }
                else {
                    state = ExpressionOptimizerState::Initial;
                }
                break;
            case ExpressionOptimizerState::AddSquaredMiddleSquared:
                if (checkAddAddSquaredMiddleSquared(BB, itBB, &firstAdd, &secondAdd, &finalInst)) {
                    increaseIterator(itBB, 1);
                    endIt = itBB;

                    LoadInst *newLoadInstFirstOperand = new LoadInst(IntegerType::get((*beginningIt).getContext(), 32), firstOperand, "", &*beginningIt);
                    LoadInst *newLoadInstSecondOperand = new LoadInst(IntegerType::get((*beginningIt).getContext(), 32), secondOperand, "", &*beginningIt);
                    IRBuilder<> Builder(&*beginningIt);
                    Value *newAddInst = Builder.CreateAdd(newLoadInstFirstOperand, newLoadInstSecondOperand, "", false, true);
                    Value *newMulInst = Builder.CreateMul(newAddInst, newAddInst, "", false, true);

                    finalInst->replaceAllUsesWith(newMulInst);

                    while (beginningIt != endIt) {
                        BasicBlock::iterator currentIt = beginningIt++;
                        (*currentIt).replaceAllUsesWith(UndefValue::get((*currentIt).getType()));
                        (*currentIt).eraseFromParent();
                    }
                }
                state = ExpressionOptimizerState::Initial;
                break;
            }
        }
    }
    return PreservedAnalyses::all();
}