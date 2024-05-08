#include "assembler.h"

uint8_t* Assembler::assemble() {
	Lexer lexer(file_name, source_code);
	Parser parser(lexer.lex());
	Byter byter(parser.parse());

	return byter.byte();
}

Assembler::Assembler(std::string source_code, std::string file_name) :
	file_name(file_name),
	source_code(source_code) {};


