#include "vspu.h"
#include "cpu/cpu.h"
#include "assembler/lexer/lexer.h"
#include "assembler/parser/parser.h"

int main() {
    CPU cpu;
    
    uint8_t memory[256] = {
        0x01, 0x01, 0x02, // MOV reg1, 2
        0x01, 0x02, 0x06, // MOV reg2, 6
        0x02, 0x01, 0x02, // ADD reg1, 2
        0x12, 0x01, 0x02, // CMP reg1, reg2
        0xB, 0x02, // JNE 2
       
    };


    /*cpu.set_memory(memory);

    while (cpu.registers.ip <= 256) {
        std::cout << (int)cpu.registers.ip << "\n";
        uint8_t opcode = cpu.get_memory()[cpu.registers.ip];
        cpu.execute(opcode);
       
    }*/

   Lexer lexer("<stdout>",
        R"(MOV r0x34, #223
           ADD r0x2, #6
           
        )");
    
    Parser parser(lexer.lex());
    Program parsed = parser.parse();
    for (auto& instruction : parsed.instructions) {
        std::cout << "{\n"
            << "    Instruction: " << instruction.instruction << ",\n"
            << "    Inputs: {" << "\n";
            for (auto& input : instruction.inputs) {
                std::cout << "        " << input.value << ",\n";
            }
            std::cout << "    }\n}\n";
    } 

    return 0;
}
