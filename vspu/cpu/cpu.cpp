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

void CPU::jmp_if(bool condition) {
	if (condition) {
		jump_by_instruction(memory[++registers.ip]);
	}
	else {
		while (!get_memory()[registers.ip] == 0x00) ++registers.ip;
	}
}

void CPU::do_operation_of(char operator_) {
	int8_t result;
	registers.ip++;
	uint8_t reg1 = memory[registers.ip];
	registers.ip++;
	uint8_t reg2 = memory[registers.ip];

	switch (operator_) {
	case 'm':
		result = reg2;
		break;
	case '-':
		result = reg1 - reg2;
		break;

	case '+':
		result = reg1 + reg2;
		break;

	case '*':
		result = reg1 * reg2;
		break;

	case '/':
		result = reg1 / reg2;
		break;

	case '^':
		result = reg1 ^ reg2;
		break;

	case '&':
		result = reg1 & reg2;
		break;

	case '|':
		result = reg1 | reg2;
		break;
	default:
		break;
	}
	registers.arithmetic[reg1 - 1] = result;
	registers.ip++;
}

void CPU::execute(uint8_t opcode) {

	if (registers.flags.halted) std::exit(0);

	switch (opcode) {
	case 0x00: // NOP
		++registers.ip;
		break;

	case 0x01: { // MOV
		do_operation_of('m');
		break;
	}

	case 0x02: { // ADD
		do_operation_of('+');
		break;
	}

	case 0x03: { // SUB
		do_operation_of('-');
		break;
	}

	case 0x04: { // MULT
		do_operation_of('*');
		break;
	}
	case 0x05: { // DIV
		do_operation_of('/');
		break;
	}
	case 0x06: { // XOR
		do_operation_of('^');
		break;
	}
	case 0x07: { // OR
		do_operation_of('|');
		break;
	}
	case 0x08: { // AND
		do_operation_of('&');
		break;
	}

	case 0x09: {
		jump_by_instruction(memory[++registers.ip]);
		break;
	}
	case 0xA: { // JEQ, JZ
		jmp_if(registers.flags.zero);
		break;
	}
	case 0xB: { // JNE, JNZ
		jmp_if(!registers.flags.zero);
		break;
	}
	case 0xC: { // JC
		jmp_if(registers.flags.carry);
		break;
	}
	case 0xD: { // JNC
		jmp_if(!registers.flags.carry);
		break;
	}
	case 0xE: { // JS
		jmp_if(registers.flags.sign == 1);
		break;
	}
	case 0xF: { // JNS
		jmp_if(registers.flags.sign == -1);
		break;
	}
	case 0x10: { // JP
		jmp_if(registers.flags.parity);
		break;
	}
	case 0x11: { // JNP
		jmp_if(!registers.flags.parity);
		break;
	}

	case 0x12: { // CMP
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

				while (registers.ip < 256 && memory[registers.ip] != 0x00) registers.ip++;
				//registers.ip++;  
			}
			else {
				registers.flags.halted = true;
			}
		}
		else {
			registers.flags.halted = true;
		}
		break;
	}
	case 0x13: { // HLT
		registers.flags.halted = true;
		break;
	}
	}

}

void CPU::set_memory(uint8_t new_memory[256]) {
	for (int i = 0; i < 256; i++) {
		memory[i] = new_memory[i];
	}
}