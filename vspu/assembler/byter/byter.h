#pragma once
#include "../parser/parser.h"
#include <unordered_map>
#include <string>
class Byter {
public:
	Byter(Program program);
	uint8_t* byte();
private:
	uint8_t stooc(std::string instruction);
	Program program;
};