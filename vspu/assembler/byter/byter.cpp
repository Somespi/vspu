#include "byter.h"

Byter::Byter(Program program): program(program) {}

uint8_t* Byter::byte() {
    uint8_t* opcodes = new uint8_t[256];
    size_t index = 0;

    for (auto& instruction : program.instructions) {
        opcodes[index++] = stooc(instruction.instruction);
        for (auto& input : instruction.inputs) {
            opcodes[index++] = (uint8_t)(input.value);
        }
    }
    for (int i = index; i < 256; i++) opcodes[i] = 0;
    return opcodes;
}

uint8_t Byter::stooc(std::string instruction) {
    static const std::unordered_map<std::string, uint8_t> instructionMap = {
        {"nop", 0x00},
        {"mov", 0x01},
        {"add", 0x02},
        {"sub", 0x03},
        {"mult", 0x04},
        {"div", 0x05},
        {"xor", 0x06},
        {"or", 0x07},
        {"and", 0x08},
        {"jeq", 0x0A},
        {"jz", 0x0A},
        {"jne", 0x0B},
        {"jnz", 0x0B},
        {"jc", 0x0C},
        {"jnc", 0x0D},
        {"js", 0x0E},
        {"jns", 0x0F},
        {"jp", 0x10},
        {"jnp", 0x11},
        {"cmp", 0x12},
        {"hlt", 0x13}
    };


    auto it = instructionMap.find(instruction);
    if (it != instructionMap.end()) {
        return it->second;
    }
    else {
        return 0x00; // nop
    }
}