#pragma once
#include <map>
#include <cstdint>

struct Flags {
    bool zero;
    int8_t parity;
    int8_t sign;
    bool carry;
    bool equal;
    bool halted;
};

struct Registers {
    uint8_t arithmetic[8];
    uint8_t comparison[8];
    uint8_t address[8];
    Flags flags;
    uint16_t ip;
};

class CPU {
public:
    CPU();
    void reset();
    void execute(uint8_t opcode);
    void set_memory(uint8_t new_memory[256]);
    int8_t* get_memory();
    Registers registers;



private:
    int8_t memory[256];
    short get_instruction_length(uint8_t opcode);
    void jump_by_instruction(uint8_t number);
    void jmp_if(bool condition);
    void do_operation_of(char operator_);
};
