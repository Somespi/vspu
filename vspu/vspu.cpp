#include "vspu.h"
#include "cpu/cpu.h"
#include "assembler/assembler.h"

int main() {
    CPU cpu;

    Assembler assembler(
        "mov 0x1, #2\n"
        "mov 0x2, #6\n"
        "add 0x1, #2\n"
        "cmp 0x1, 0x2\n"
        "jne 0x2",
        "<stdout>");

    cpu.set_memory(assembler.assemble());

    while (cpu.registers.ip < 256) {
        uint8_t opcode = cpu.get_memory()[cpu.registers.ip];
        cpu.execute(opcode);
    }

    std::cout << "Final Register State:\n";
    int i = 1;
    for (int8_t reg : cpu.registers.arithmetic) {
        std::cout << "Register " << i << ": " << static_cast<int>(reg) << std::endl;
        i++;
    }

    return 0;
}