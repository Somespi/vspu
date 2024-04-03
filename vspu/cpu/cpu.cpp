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
    int counter = 0;
    if (number == 0) registers.ip = 0;
    for (registers.ip = 0; registers.ip < 256; registers.ip++) {
        if (memory[registers.ip] == 0x00) {
            ++counter;
            if (counter == number + 1) {
                ++registers.ip;  
                return;
            }
        }
    }
    registers.ip = 256;
}

void CPU::execute(uint8_t opcode) {

    if (registers.flags.halted) std::exit(0);
    if (opcode == 0x00) ++registers.ip; 
    else if (opcode == 0x01) { // MOV
        registers.ip++; 
        uint8_t reg1 = memory[registers.ip];  
        registers.arithmetic[reg1 - 1] = memory[++registers.ip];
        registers.ip++;
    }

    else if (opcode == 0x02) { // ADD
        registers.ip++;
        uint8_t reg1 = memory[registers.ip];
        registers.arithmetic[reg1 - 1] += memory[++registers.ip];
        registers.ip++;
    }

    else if (opcode == 0x03) { // SUB
        registers.ip++;
        uint8_t reg1 = memory[registers.ip];
        registers.arithmetic[reg1 - 1] -= memory[++registers.ip];
        registers.ip++;
    }

    else if (opcode == 0x25) { // JMP
        jump_by_instruction(memory[++registers.ip]);
    }
    else if (opcode == 0x26) { // JZ, JE
        if (registers.flags.zero) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }
    }

    else if (opcode == 0x27) { // JNZ
        if (!registers.flags.zero) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }

    }

    else if (opcode == 0x28) { // JC
        if (registers.flags.carry) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }

    }

    else if (opcode == 0x29) { // JNC
        if (!registers.flags.carry) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }

    }

    else if (opcode == 0x30) { // JS
        if (registers.flags.sign == 1) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }

    }


    else if (opcode == 0x31) { // JNS
        if (registers.flags.sign == -1) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }

    }

    else if (opcode == 0x32) { // JP
        if (registers.flags.parity) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }

    }

    else if (opcode == 0x33) { // JNP
        if (!registers.flags.parity) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }

    }
    else if (opcode == 0x34) { // CMP
        registers.ip++;

        if (registers.ip < 256 && memory[registers.ip] != 0x00) {
            int8_t reg1 = registers.arithmetic[get_memory()[registers.ip] - 1];

            registers.ip++;
            if (registers.ip < 256 && memory[registers.ip] != 0x00) {
                int8_t reg2 = registers.arithmetic[get_memory()[registers.ip] - 1];

                int8_t result = reg1 - reg2;
                registers.flags.carry = (result < 0);
                registers.flags.sign = ((result < 0) ? -1 : 1);
                registers.flags.parity = ((result % 2 == 0) ? 1 : 0);
                registers.flags.zero = (result == 0);
                registers.flags.equal = (reg1 == reg2);

                while (registers.ip < 256 && memory[registers.ip] != 0x00)  registers.ip++;
                //registers.ip++;  
            }
            else {
                registers.flags.halted = true;
            }
        }
        else {
            registers.flags.halted = true;
        }
    }


    else if (opcode == 0x35) { // MULT
        registers.ip++;
        uint8_t reg1 = memory[registers.ip];
        registers.arithmetic[reg1 - 1] *= memory[++registers.ip];
        registers.ip++;
        }

    else if (opcode == 0x43) { // DIV
        registers.ip++;
        uint8_t reg1 = memory[registers.ip];
        registers.arithmetic[reg1 - 1] /= memory[++registers.ip];
        registers.ip++;
        }

    else if (opcode == 0x51) { // AND
          registers.ip++;
          uint8_t reg1 = memory[registers.ip];
          registers.arithmetic[reg1 - 1] &= memory[++registers.ip];
          registers.ip++;
          }
    else if (opcode == 0x59) { // OR
        registers.ip++;
        uint8_t reg1 = memory[registers.ip];
        registers.arithmetic[reg1 - 1] |= memory[++registers.ip];
        registers.ip++;
        }

    else if (opcode == 0x61) { // XOR
         registers.ip++;
         uint8_t reg1 = memory[registers.ip];
         registers.arithmetic[reg1 - 1] ^= memory[++registers.ip];
         registers.ip++;
    }

    else if (opcode == 0x69) { // HLT
        registers.flags.halted = true;
    }

    else if (opcode == 0x70) { //JEQ
        if (registers.flags.equal) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
        }
    }

    else if (opcode == 0x71) { // JNE

        if (!registers.flags.equal) {
            jump_by_instruction(memory[++registers.ip]);
        }
        else {
            ++registers.ip;
            ++registers.ip;
            ++registers.ip;
        }
     }


}


void CPU::set_memory(uint8_t new_memory[256]) {
    for (int i = 0; i < 256; i++) {
        memory[i] = new_memory[i];
    }
}
