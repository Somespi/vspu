#include "vspu.h"
#include "cpu/cpu.h"

int main() {
    CPU cpu;
    
    uint8_t memory[256] = {
        0x01, 0x02, 0x00, // MOV reg1, 2
        0x03, 0x04, 0x00, // MOV reg3, 4
        0x25, 0x03, 0x00, // JMP 1
        0x05, 0x06, 0x00, // MOV reg5, 6 (not executed)
        
    };




    cpu.set_memory(memory);

    while (cpu.registers.ip < 256) {
        uint8_t opcode = cpu.get_memory()[cpu.registers.ip];
        cpu.execute(opcode);
        cpu.registers.ip++;
    }

    std::cout << "Final Register State:\n";
    int i = 1;
    for (int8_t reg : cpu.registers.arithmetic) {
        std::cout << "Register " << i << ": " << static_cast<int>(reg) << std::endl;
        i++;
    }
    return 0;
}
