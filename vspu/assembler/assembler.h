#pragma once
#include "lexer/lexer.h"
#include "parser/parser.h"

class Assembler {
public:
	int8_t* assemble();
	Assembler(std::string source_code, std::string file_name);

};