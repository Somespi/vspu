#include "vspu.h"
#include "cpu/cpu.h"

int main() {
    CPU cpu;
    
    uint8_t memory[256] = {
        0x01, 0x01, 0x02, // MOV reg1, 2
        0x01, 0x02, 0x06, // MOV reg2, 6
        0x02, 0x01, 0x02, // ADD reg1, 2
        0x12, 0x01, 0x02, // CMP reg1, reg2
        0xB, 0x02, // JNE 2
       
    };



    cpu.set_memory(memory);

    while (cpu.registers.ip <= 256) {
        std::cout << (int)cpu.registers.ip << "\n";
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
