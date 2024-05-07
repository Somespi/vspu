#pragma once
#include <vector>
#include <string> 

enum TokenType {
	END, 
	INSTRUCTION,
	REGISTER,
	HASH,
	HEX_ZERO,
	NUMBER, 
	COMMA
};

class Cursor {
public:
	int line;
	int column;
	std::string filename;
	Cursor* clone();
};

typedef struct {
	TokenType type;
	Cursor* pos;
	std::string literal;
} Token;

class Lexer {
private:
	Cursor pos;
	std::vector<Token> tokens;
	std::string filename;
	std::string source_code;
	void parse_digit(int* current);
	void advance(int* current);
public:
	std::vector<Token> lex();
	Lexer(std::string filename, std::string source_code);

};
