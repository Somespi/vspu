#include "lexer.h"

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
        if (*current + 1 < source_code.size() && std::isdigit(source_code[*current + 1])) {
            advance(current);
        }
        else {
            break;
        }
    }

    tokens.push_back({
        type,
        pos.clone(),
        number
        });
}

void Lexer::parse_instruction(int* current) {
    std::string instruction;

    while (*current < source_code.size() && std::isalpha(source_code[*current])) {
        instruction += tolower(source_code[*current]);
        if (*current + 1 < source_code.size() && std::isalpha(source_code[*current + 1])) {
            advance(current);
        }
        else {
            break;
        }
    }
    tokens.push_back({ TokenType::INSTRUCTION, 
       pos.clone(),
       instruction
        });
}


void Lexer::parse_comment(int* current) {
    tokens.push_back({ TokenType::SEMICOLON, pos.clone(), ";" });
    advance(current);

    while (*current < source_code.size() && source_code[*current] != '\n') {
        if ((*current + 1 < source_code.size() && source_code[*current + 1]  != '\n')) {
            advance(current);
        }
        else {
            break;
        }
    }

}

std::vector<Token> Lexer::lex() {
    int index = 0;

    while (index < source_code.size()) {
        char current_char = source_code[index];
        if (isdigit(current_char)) {
            parse_digit(&index);
        }
        else if (current_char == ',') {
            tokens.push_back({TokenType::COMMA, pos.clone(), ","});
        }
        else if (current_char == ';') {
            parse_comment(&index);
        }
        else if (current_char == '#') {
            tokens.push_back({ TokenType::HASH, pos.clone(), "#" });
        }
        else if (isspace(current_char) || current_char == '\n') {
            // ignore
        }
        else if (isalpha(current_char)) {
            parse_instruction(&index);
        }
        else {
            std::cerr << filename << ": Unrecognied token \"" << current_char << "\" at " << pos.column << ':' << pos.line;
            std::exit(0);
        }
        advance(&index);
    }
    tokens.push_back({ TokenType::END, pos.clone(), "\0" });
    return tokens;
}


Lexer::Lexer(std::string filename, std::string source_code) {
	this->filename = filename;
    pos.filename = filename;
	this->source_code = source_code;
}
