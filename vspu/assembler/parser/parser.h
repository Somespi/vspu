#pragma once
#include <vector> 
#include <string>
#include "../lexer/lexer.h"

struct Node {};
enum ImmediateValueType {
	INTEGER,
	HEXADECIMAL
};


struct Value { int value; };


struct Instruction : Node {
	std::string instruction;
	std::vector<Value> inputs;
};

struct Register : Value {
	
};

struct ImmediateValue : Value {

	ImmediateValueType type;
};

struct Program : Node {
	std::vector<Instruction> instructions;
};


class Parser {
public:
	Parser(std::vector<Token> tokens);
	Program parse();

private:
	std::vector<Token> tokens;
	Cursor pos;
	void advance(int* current);
	Value parse_value(int* current);
	Instruction parse_instruction(int* current);

};