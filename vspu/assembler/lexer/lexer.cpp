#include "lexer.h"
#include <cctype>

Cursor* Cursor::clone() {
	return new Cursor(*this);
}

void Lexer::advance(int *current) {
    (*current)++;
    if ((*current) == '\n') {
        pos.line++;
        pos.column = 0;
    }
    else pos.column++;
}

void Lexer::parse_digit(int* current) {
    std::string number = "";
    TokenType type;

    if (source_code[*current] == '0' && *current + 1 < source_code.size() && source_code[*current + 1] == 'x') {
        advance(current);
        advance(current);
        type = TokenType::HEX_ZERO;
        number += "0x";
    }
    else  type = TokenType::NUMBER;
    

    while (*current < source_code.size() && std::isdigit(source_code[*current])) {
        number += source_code[*current];
        advance(current);
    }

    tokens.push_back({
        type,
        pos.clone(),
        number
        });
}


std::vector<Token> Lexer::lex() {
    int index = 0;

    while (index < source_code.size()) {
        char current_char = source_code[index];
        if (isdigit(current_char)) {
            parse_digit(&index);
        }
        advance(&index);
    }

    return tokens;
}


Lexer::Lexer(std::string filename, std::string source_code) {
	this->filename = filename;
	this->source_code = source_code;
}
