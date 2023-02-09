#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <unordered_map>
#include "scanner.h"
using namespace std;

vector<Token> add = {{Token::Kind::ID, ""}, {Token::Kind::REG, ""}, {Token::Kind::COMMA, ""}, {Token::Kind::REG, ""}, {Token::Kind::COMMA, ""}, {Token::Kind::REG, ""}};
vector<Token> mult {{Token::Kind::ID, ""}, {Token::Kind::REG, ""}, {Token::Kind::COMMA, ""}, {Token::Kind::REG, ""}};
vector<Token> lw {{Token::Kind::ID,""}, {Token::Kind::REG,""}, {Token::Kind::COMMA,""}, {Token::Kind::INT,""}, {Token::Kind::LPAREN,""}, {Token::Kind::REG,""}, {Token::Kind::RPAREN,""}};
vector<Token> beq {{Token::Kind::ID,""}, {Token::Kind::REG,""}, {Token::Kind::COMMA,""}, {Token::Kind::INT,""}};
vector<Token> jr {{Token::Kind::ID,""}, {Token::Kind::REG,""}};
vector<Token> binword {{Token::Kind::ID,""}, {Token::Kind::REG,""}};
vector<Token> hexword {{Token::Kind::ID,""}, {Token::Kind::HEXINT,""}};


vector<vector<Token>> FORMATS {add, mult, lw, beq, jr, binword, hexword};
unordered_map<string, int> OPCODES {{"add", 0}, {"sub", 1}, {"mult", 1}, {"lw", 2}, {"beq", 3}, {"jr", 4}, {"bin", 5}, {"hex", 6}};

bool validFormat(vector<Token> line, string opcode){
    // for (auto &format : FORMATS) {
    vector<Token> format = FORMATS[OPCODES[opcode]];
    if (line.size() != format.size()) return false;
    bool valid = true;
    for (int i = 0; i < line.size(); i++){
        if (line[i].getKind() != format[i].getKind()){
            valid = false;
            break;
        }
    }
    if (valid) return true;
    // }
    return false;
}


#endif