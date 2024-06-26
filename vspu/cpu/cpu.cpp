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

	if (number == 0) {
		registers.ip = 0;
		return;
	}

	int current_instr = 0;
	registers.ip = 0;

	while (registers.ip < 256 && current_instr < number) {
		short len = get_instruction_length(get_memory()[registers.ip]);
		registers.ip += len;
		current_instr++;
	}

	if (current_instr == number) {
		return; 
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

	int8_t result = 0;
	registers.ip++;
	uint8_t reg1 = memory[registers.ip];
	registers.ip++;
	uint8_t reg2 = memory[registers.ip];
	
	switch (operator_) {
	case 'm':
		result = reg2;
		break;
	case '-':
		result = registers.arithmetic[reg1 - 1] - reg2;
		break;

	case '+':
		result = registers.arithmetic[reg1 - 1] + reg2;
		break;

	case '*':
		result = registers.arithmetic[reg1 - 1] * reg2;
		break;

	case '/':
		result = registers.arithmetic[reg1 - 1] / reg2;
		break;

	case '^':
		result = registers.arithmetic[reg1 - 1] ^ reg2;
		break;

	case '&':
		result = registers.arithmetic[reg1 - 1] & reg2;
		break;

	case '|':
		result = registers.arithmetic[reg1 - 1] | reg2;
		break;
	default:
		registers.flags.halted = true;
		break;
	}

	registers.arithmetic[reg1 - 1] = result;
	registers.ip++;

}

short CPU::get_instruction_length(uint8_t opcode) {
	std::map<uint8_t, short> instruction_to_length;
	instruction_to_length[0x00] = 1;
	instruction_to_length[0x01] = 3;
	instruction_to_length[0x02] = 3;
	instruction_to_length[0x03] = 3;
	instruction_to_length[0x04] = 3;
	instruction_to_length[0x05] = 3;
	instruction_to_length[0x06] = 3;
	instruction_to_length[0x08] = 3;
	instruction_to_length[0x09] = 2;
	instruction_to_length[0x0A] = 2;
	instruction_to_length[0x0B] = 2;
	instruction_to_length[0x0C] = 2;
	instruction_to_length[0x0D] = 2;
	instruction_to_length[0x0E] = 2;
	instruction_to_length[0x0F] = 2;
	instruction_to_length[0x10] = 2;
	instruction_to_length[0x11] = 2;
	instruction_to_length[0x12] = 3;
	instruction_to_length[0x13] = 1;

	return instruction_to_length[opcode];
}


void CPU::execute(uint8_t opcode) {
	if (registers.flags.halted) std::exit(1);

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
		jmp_if(registers.flags.equal);
		break;
	}
	case 0xB: { // JNE, JNZ
		jmp_if(!registers.flags.equal);
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
				registers.ip++;
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