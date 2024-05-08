#pragma once
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "byter/byter.h"

class Assembler {
public:
	uint8_t* assemble();
	Assembler(std::string source_code, std::string file_name);
private:
	std::string source_code;
	std::string file_name;
};