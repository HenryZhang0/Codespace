#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <unordered_map>
#include "scanner.h"
using namespace std;

vector<Token> add = {{Token::Kind::ID, ""}, {Token::Kind::REG, ""}, {Token::Kind::COMMA, ""}, {Token::Kind::REG, ""}, {Token::Kind::COMMA, ""}, {Token::Kind::REG, ""}};
vector<Token> mult {{Token::Kind::ID, ""}, {Token::Kind::REG, ""}, {Token::Kind::COMMA, ""}, {Token::Kind::REG, ""}};
vector<Token> lw {{Token::Kind::ID,""}, {Token::Kind::REG,""}, {Token::Kind::COMMA,""}, {Token::Kind::INT,""}, {Token::Kind::LPAREN,""}, {Token::Kind::REG,""}, {Token::Kind::RPAREN,""}};
// vector<Token> beq {{Token::Kind::ID,""}, {Token::Kind::REG,""}, {Token::Kind::COMMA,""}, {Token::Kind::REG, ""}, {Token::Kind::COMMA,""}};
vector<Token> jr {{Token::Kind::ID,""}, {Token::Kind::REG,""}};
vector<Token> mfhi {{Token::Kind::ID,""}, {Token::Kind::REG,""}};

vector<vector<Token>> FORMATS {add, mult, lw, jr, mfhi};
unordered_map<string, int> OPCODES {
    {"add", 0}, {"sub", 0}, {"slt", 0}, {"sltu", 0},
    {"mult", 1}, {"div", 1}, {"divu", 1}, {"multu", 1},
    {"lw", 2}, {"sw", 2},
    {"jr", 3}, {"jalr", 3},
    {"mfhi", 4}, {"mflo", 4}, {"lis", 4}};

bool validFormat(vector<Token> line, string opcode){
    if (line[0].getKind() == Token::Kind::WORD) {
        return (
            line.size() == 2 
            && (line[1].getKind() == Token::Kind::INT 
            || line[1].getKind() == Token::Kind::HEXINT 
            || line[1].getKind() == Token::Kind::ID)
        );
    }
    if (line[0].getLexeme() == "beq" || line[0].getLexeme() == "bne") {
        return (
            line.size() == 6 
            && line[1].getKind() == Token::Kind::REG 
            && line[2].getKind() == Token::Kind::COMMA 
            && line[3].getKind() == Token::Kind::REG 
            && line[4].getKind() == Token::Kind::COMMA 
            && (line[5].getKind() == Token::Kind::INT 
            || line[5].getKind() == Token::Kind::HEXINT 
            || line[5].getKind() == Token::Kind::ID)
        );
    }
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
    return false;
}


#endif