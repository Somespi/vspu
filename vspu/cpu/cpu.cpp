#include "cpu.h"
#include <iostream>

CPU::CPU() {
    reset(); 
}

void CPU::reset() {

    for (int i = 0; i < 8; ++i) {
        registers.arithmetic[i] = 0;
        registers.comparison[i] = 0;
        registers.address[i] = 0;
    }
    registers.ip = 0;
    registers.flags.halted = false; 

    for (int i = 0; i < 256; ++i) {
        memory[i] = 0;
    }
}
int8_t* CPU::get_memory() {
    return memory;
}
void CPU::jump_by_instruction(uint8_t number) {
    for (int i = 0; i < number; i++) {
        while (memory[registers.ip] != 0x00) registers.ip++;
    }
    registers.ip++;

}

void CPU::execute(uint8_t opcode) {
    if (registers.flags.halted) return;
    if (opcode == 0x00) return;
    else if (opcode >= 0x01 && opcode <= 0x08) { // MOV
        registers.arithmetic[static_cast<int>(opcode) - 1] = memory[++registers.ip];
        registers.flags.zero = (registers.arithmetic[static_cast<int>(opcode) - 1] == 0);

    }
    else if (opcode >= 0x09 && opcode <= 0x16) { // ADD
        registers.arithmetic[static_cast<int>(opcode) - 9] += memory[++registers.ip];
        registers.flags.zero = (registers.arithmetic[static_cast<int>(opcode) - 9] == 0);

    }
    else if (opcode >= 0x17 && opcode <= 0x24) { // SUB
        registers.arithmetic[static_cast<int>(opcode) - 17] -= memory[++registers.ip];
        registers.flags.zero = (registers.arithmetic[static_cast<int>(opcode) - 17] == 0);

    }
    else if (opcode == 0x25) { // JMP
        jump_by_instruction(memory[++registers.ip]);
    }
    else if (opcode == 0x26) { // JZ, JE
        if (registers.flags.zero) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }
    }

    else if (opcode == 0x27) { // JNZ
        if (!registers.flags.zero) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }

    }

    else if (opcode == 0x28) { // JC
        if (registers.flags.carry) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }

    }

    else if (opcode == 0x29) { // JNC
        if (!registers.flags.carry) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }

    }

    else if (opcode == 0x30) { // JS
        if (registers.flags.sign == 1) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }

    }


    else if (opcode == 0x31) { // JNS
        if (registers.flags.sign == -1) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }

    }

    else if (opcode == 0x32) { // JP
        if (registers.flags.parity) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }

    }

    else if (opcode == 0x33) { // JNP
        if (!registers.flags.parity) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
        }

    }

    else if (opcode == 0x34) { // CMP
        int8_t result = registers.arithmetic[++registers.ip] - registers.arithmetic[++registers.ip];
        registers.flags.carry = (result < 0);
        registers.flags.sign = ((result < 0) ? -1 : 1);
        registers.flags.parity = ((result % 2 == 0) ? 1 : 0);
        registers.flags.zero = (result == 0);
        ++registers.ip;
    }

    else if (opcode >= 0x35 && opcode <= 0x42) { // MULT
        registers.arithmetic[static_cast<int>(opcode) - 35] *= memory[++registers.ip];
        registers.flags.zero = (registers.arithmetic[static_cast<int>(opcode) - 35] == 0);
       }

    else if (opcode >= 0x43 && opcode <= 0x50) { // DIV
        registers.arithmetic[static_cast<int>(opcode) - 43] /= memory[++registers.ip];
        registers.flags.zero = (registers.arithmetic[static_cast<int>(opcode) - 43] == 0);
      }
    else if (opcode >= 0x51 && opcode <= 0x58) { // AND
       registers.arithmetic[opcode - 0x51] &= memory[++registers.ip];
       registers.flags.zero = (registers.arithmetic[opcode - 0x51] == 0);
      }
    else if (opcode >= 0x59 && opcode <= 0x60) { // OR
         registers.arithmetic[opcode - 0x59] |= memory[++registers.ip];
         registers.flags.zero = (registers.arithmetic[opcode - 0x59] == 0);
     }
    else if (opcode >= 0x61 && opcode <= 0x68) { // XOR
       registers.arithmetic[opcode - 0x61] ^= memory[++registers.ip];
       registers.flags.zero = (registers.arithmetic[opcode - 0x61] == 0);
     }

    else if (opcode == 0x69) { // HLT
        registers.flags.halted = true;
    }


}


void CPU::set_memory(uint8_t new_memory[256]) {
    for (int i = 0; i < 256; i++) {
        memory[i] = new_memory[i];
    }
}
