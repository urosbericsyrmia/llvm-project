#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/InitializePasses.h"

#define DEBUG_DELETE_NAME "Debug Delete"
#define DEBUG_DELETE_ARG "debug-delete"

using namespace llvm;

namespace {

class DebugDelete : public MachineFunctionPass {
public:
    DebugDelete() : MachineFunctionPass(ID) { }

    bool runOnMachineFunction(MachineFunction &MF) override;

    static char ID;
private:
    StringRef getPassName() const override {
        return DEBUG_DELETE_NAME;
    }
};

bool DebugDelete::runOnMachineFunction(MachineFunction &MF) {
    for (MachineFunction::iterator itMF = MF.begin(); itMF != MF.end(); itMF++) {
        MachineBasicBlock &MBB = *itMF;
        for (MachineBasicBlock::iterator itMBB = MBB.begin(); itMBB != MBB.end();) {
            MachineInstr &MI = *itMBB;
            itMBB++;
            if (MI.isDebugInstr()) {
                MI.eraseFromParent();
            }
        }
    }
    
    return true;
}

} // end anonymous namespace

char DebugDelete::ID = 0;
char &llvm::DebugDeleteID = DebugDelete::ID;

INITIALIZE_PASS(DebugDelete, DEBUG_DELETE_ARG, DEBUG_DELETE_NAME, false, false)

MachineFunctionPass *llvm::createDebugDelete() {
    return new DebugDelete();
}


