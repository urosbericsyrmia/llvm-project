#include "llvm/CodeGen/Passes.h"
#include "llvm/InitializePasses.h"
#include "llvm/CodeGen/MachineFunctionPass.h"

#define DEBUG_COUNT_NAME "Debug Count"
#define DEBUG_COUNT_ARG "debug-count"

using namespace llvm;

namespace {

class DebugCount : public MachineFunctionPass {
public:
    DebugCount() : MachineFunctionPass(ID) {}

    bool runOnMachineFunction(MachineFunction &MF) override;

    static char ID;
private:
    StringRef getPassName() const override {
        return DEBUG_COUNT_NAME;
    }
};

bool DebugCount::runOnMachineFunction(MachineFunction &MF) {
    unsigned countDbgValue = 0;
    unsigned countDbgValueList = 0;
    unsigned countDbgLabel = 0;
    unsigned countDbgPHI = 0;
    unsigned countDbgInstrRef = 0;
    
    for (MachineFunction::iterator itMF = MF.begin(); itMF != MF.end(); itMF++) {
        MachineBasicBlock& MBB = *itMF;
        for (MachineBasicBlock::iterator itMBB = MBB.begin(); itMBB != MBB.end(); itMBB++) {
            MachineInstr& MI = *itMBB;

            if (MI.isNonListDebugValue()) {
                countDbgValue++;
            }
            else if (MI.isDebugValueList()) {
                countDbgValueList++;
            }
            else if (MI.isDebugLabel()) {
                countDbgLabel++;
            }
            else if (MI.isDebugPHI()) {
                countDbgPHI++;
            }
            else if (MI.isDebugRef()) {
                countDbgInstrRef++;
            }
        }
    }

    errs() << "Function: " << MF.getName() << "\n";
    errs() << "\tDBG_VALUE: " << countDbgValue << "\n";
    errs() << "\tDBG_VALUE_LIST: " << countDbgValueList << "\n";
    errs() << "\tDBG_LABEL: " << countDbgLabel << "\n";
    errs() << "\tDBG_PHI: " << countDbgPHI << "\n";
    errs() << "\tDBG_INSTR_REF: " << countDbgInstrRef << "\n";

    return true;
}

} // end anonymous namespace

char DebugCount::ID = 0;
char &llvm::DebugCountID = DebugCount::ID;

INITIALIZE_PASS(DebugCount, DEBUG_COUNT_ARG, DEBUG_COUNT_NAME, false, false)

MachineFunctionPass *llvm::createDebugCount() {
    return new DebugCount();
}