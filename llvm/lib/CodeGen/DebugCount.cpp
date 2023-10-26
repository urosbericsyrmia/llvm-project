#include "llvm/CodeGen/Passes.h"
#include "llvm/InitializePasses.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

#define DEBUG_COUNT_DESC "Debug Count"
#define DEBUG_COUNT_NAME "debug-count"

using namespace llvm;

namespace {

class DebugCount : public MachineFunctionPass {
public:
    DebugCount() : MachineFunctionPass(ID) {}

    bool runOnMachineFunction(MachineFunction &MF) override;

    static char ID;
private:
    StringRef getPassName() const override {
        return DEBUG_COUNT_DESC;
    }
};

bool DebugCount::runOnMachineFunction(MachineFunction &MF) {
    outs() << MF.getName() << "\n";
    return true;
}

} // end anonymous namespace

char DebugCount::ID = 0;
char &llvm::DebugCountID = DebugCount::ID;

INITIALIZE_PASS(DebugCount, DEBUG_COUNT_NAME, DEBUG_COUNT_DESC, false, false)

MachineFunctionPass *llvm::createDebugCount() {
    return new DebugCount();
}