#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

#define DEBUG_COUNT_DESC "X86 Debug Count"
#define DEBUG_COUNT_NAME "x86-debug-count"

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

char DebugCount::ID = 0;

bool DebugCount::runOnMachineFunction(MachineFunction &MF) {
    outs() << MF.getName() << "\n";
    return true;
}

} // end anonymous namespace


namespace llvm {

INITIALIZE_PASS(DebugCount, DEBUG_COUNT_NAME, DEBUG_COUNT_DESC, false, false)

FunctionPass *llvm::createDebugCount() {
  return new DebugCount();
}

}