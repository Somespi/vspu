#pragma once
#include <vector> 
#include <string>

struct Node {};
enum ImmediateValueType {
	INTEGER,
	HEXADECIMAL
};

struct Instruction : Node {
	std::string instruction;
	std::vector<Value> inputs;
};


struct Value {int value; };


struct Register : Value {
	
};

struct ImmediateValue : Value {

	ImmediateValueType type;
};

struct Program : Node {
	std::vector<Instruction> instructions;
};
