#include "parser.h"
#include "../lexer/lexer.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {};


void Parser::advance(int* current) {
	(*current)++;
	if (tokens[*current].type == TokenType::NEWLINE) {
		pos.line++;
		pos.column = 0;
	}
	else {
		pos.column++;
	}
}


Value Parser::parse_value(int* current) {
    if (tokens[*current].type == TokenType::HASH) {

        ImmediateValue valueImmediate{};
        valueImmediate.type = ImmediateValueType::INTEGER;
        advance(current);
        if (tokens[*current].type != TokenType::NUMBER) {
            // Handle error: Expected number token after '#'
        }
        valueImmediate.value = std::stoi(tokens[*current].literal);
		//advance(current);

        return valueImmediate;

    }
    else if (tokens[*current].type == TokenType::HEX) {

        ImmediateValue valueHex{};
        valueHex.type = ImmediateValueType::HEXADECIMAL;
        valueHex.value = std::stoi(tokens[*current].literal, nullptr, 16);
		//advance(current);
        return valueHex;

    }
    else if (tokens[*current].literal == "r") {

		Register registered{};
        advance(current);
        if (tokens[*current].type != TokenType::HEX) {
            // Handle error: Expected hexadecimal token after 'r'
        }
        registered.value = std::stoi(tokens[*current].literal, nullptr, 16);
		//advance(current);
        return registered;

    }
    else {
		std::cerr << " Handle error: Unexpected token type \"" << tokens[*current].literal <<"\"\n";
		std::exit(0);
        // Handle error: Unexpected token type
    }
}


Program Parser::parse() {
	Program program = { {} };
	int index = 0;
	while (tokens[index].type != TokenType::END && tokens[index].type != TokenType::NEWLINE) {
		program.instructions.push_back(parse_instruction(&index));
	}

	return program;
}

Instruction Parser::parse_instruction(int* current) {
	Instruction instruction;
	if (tokens[*current].type != TokenType::INSTRUCTION) {
		// error handling
	}
	instruction.instruction = tokens[*current].literal;
	advance(current);

	while (*current < tokens.size() && tokens[*current].type != NEWLINE && tokens[*current].type != TokenType::END) {
		instruction.inputs.push_back(parse_value(current));
		if (*current < tokens.size() && (tokens[*current].type != NEWLINE && tokens[*current].type != TokenType::END)) {
			advance(current);
		}
		else {
			break;
		}

		if ((tokens[*current].type == COMMA)) advance(current);

	}
	if (*current < tokens.size() && (tokens[*current].type == NEWLINE)) {
		advance(current); 
	}

	return instruction;
}
