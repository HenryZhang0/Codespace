#include <iostream>
#include <string>
#include <vector>
#include "scanner.h"
#include "constants.h"
#include "unordered_map"

using namespace std;

bool check_range(int64_t value) {
  return value >= -2147483648 && value <= 4294967295;
}
void output_word(int64_t word) {
  if (!check_range(word)) {
    std::cerr << "Number out of range: " << word << std::endl;
    throw 1;
  }
  bool readable = 0;
  if (readable) {
    for(int shift=24; shift>=0; shift-=8) {
      char c = (word >> shift) & 0xff;
      for (int i = 7; i >= 0; i--) {
        std::cout << ((c >> i) & 1);
      }
      cout << " ";
    }
    cout << endl;
  }
   else {
    for(int shift=24; shift>=0; shift-=8) {
      char c = (word >> shift) & 0xff;
      cout << c;
    }
  }
}

int register_format(int s, int t, int d, int f) {
  // cout << "register format: " << s << " " << t << " " << d << " " << f << endl;
  return s << 21 | t << 16 | d << 11 | f;
}

int immediate_format(int s, int t, int i, int f) {
  // cout << "immediate format: " << s << " " << t << " " << i << " " << f << endl;
  return f << 26 | s << 21 | t << 16 | (i & 0xffff);
}

int main() {
  std::string line;

  // my variables
  unordered_map<string, int> label_map;
  vector<vector<Token>> buffer;
  int programCounter = 0;



  try {
    while (getline(std::cin, line)) {
      // This example code just prints the scanned tokens on each line.
      std::vector<Token> tokenLine = scan(line);

      // for(int i=0; i<tokenLine.size(); i++) { ... }
      // PRINT 
      // for (auto &token : tokenLine) std::cout << token << ' ';
      // std::cout << std::endl;

// FIRST PASS ------------------------------------------------------------------------
      // validate format and grab labels

      std::vector<Token> tokenLineNoLabels;

      bool frontTokenIsLabel = true;
      for (auto &token : tokenLine) { // remove labels and add to label map
        if (token.getKind() != Token::Kind::LABEL || !frontTokenIsLabel) {
          tokenLineNoLabels.push_back(token);
          frontTokenIsLabel = false;
        } else {
          if (label_map.find(token.getLexeme()) != label_map.end()) {
            std::cerr << "Duplicate label: " << token.getLexeme() << std::endl;
            throw 1;
          }
          label_map[token.getLexeme()] = programCounter*4;
          // cout << "label: " << token.getLexeme() << " value: " << programCounter*4 << endl;
        }
      }

        // line is only LABEL:    continue
        if (tokenLineNoLabels.size() == 0) {
          continue;    
        } else {
          buffer.push_back(tokenLineNoLabels); // add to buffer
          programCounter++;
        }

      if (!validFormat(tokenLineNoLabels, tokenLineNoLabels[0].getLexeme())) {
        std::cerr << "Invalid format" << std::endl;
        throw 1;
      }

    }

// SECOND PASS ------------------------------------------------------------------------
    programCounter = 0;
    for (const vector<Token> &line : buffer) {
      programCounter++;
      string ins = line[0].getLexeme();
      if (ins == ".word") 
      {
        if (line[1].getKind() == Token::Kind::ID) {
          if (label_map.find(line[1].getLexeme() + ":") == label_map.end()) {
            std::cerr << "Undefined label: " << line[1].getLexeme() << std::endl;
            throw 2;
          }
          output_word(label_map[line[1].getLexeme() + ":"]);
        } else {
          output_word(line[1].toNumber());
        }
      }
      else if (ins == "add" || ins == "sub" || ins == "slt" || ins == "stlu")
      {
        // 000000 sssss ttttt ddddd 00000 ffffff
        int f;
        if (ins == "add") 
          f = 32; // 10 000
        else if (ins == "sub")
          f = 24; // 1 1000
        
        int d = line[1].toNumber();
        int s = line[3].toNumber();
        int t = line[5].toNumber();

        int instruction = register_format(s, t, d, f);
        output_word(instruction);
      }
      else if (ins == "mult" || ins == "div" || ins == "multu" || ins == "divu") 
      {
        // 000000 sssss ttttt 00000 00000 ffffff
        int f;
        if (ins == "mult") 
          f = 24; // 1 1000
        else if (ins == "div")
          f = 26; // 1 1010
        else if (ins == "multu")
          f = 25; // 1 1001
        else if (ins == "divu")
          f = 27; // 1 1011
        int s = line[1].toNumber();
        int t = line[3].toNumber();
        
        int instruction = register_format(s, t, 0, f);
        output_word(instruction);
      }
      else if (ins == "lw" || ins == "sw") 
      {
        // 100011 sssss ttttt iiiiiiiiiiiiiiii
        int f;
        if (ins == "lw") 
          f = 35; // 100 011
        else if (ins == "sw")
          f = 43; // 101 011
        int64_t s = line[1].toNumber();
        int64_t t = line[5].toNumber();
        int64_t i = line[3].toNumber();

        output_word(immediate_format(s, t, i, f));
      }
      else if (ins == "beq" || ins == "bne" ) 
      {
        int f;
        if (ins == "beq") 
          f = 4;
        else if (ins == "bne")
          f = 5;
        
        int64_t s = line[1].toNumber();
        int64_t t = line[3].toNumber();

        int64_t i;
        if (line[5].getKind() == Token::Kind::ID) {
          if (label_map.find(line[5].getLexeme() + ":") == label_map.end()) {
            std::cerr << "Label not found: " << line[5].getLexeme() << std::endl;
            throw 1;
          }
          i = label_map[line[5].getLexeme() + ":"]/4;
          
          // cout << i << " " << programCounter << endl;
          i = i - programCounter;
        } else {
          i = line[5].toNumber();
        }


        output_word(immediate_format(s, t, i, f));
      }
      
      else if (ins == "jr" || ins == "jalr") 
      {
        int f;
        if (ins == "jr") 
          f = 8;
        else if (ins == "jalr")
          f = 9;
        int64_t s = line[1].toNumber();
        output_word(register_format(s, 0, 0, f));
      }

      else if (ins == "mfhi" || ins == "mflo" || ins == "lis") 
      {
        int f;
        if (ins == "mfhi") 
          f = 16;
        else if (ins == "mflo")
          f = 18;
        else if (ins == "lis")
          f = 20;
        int64_t d = line[1].toNumber();
        output_word(register_format(0, 0, d, f));
      }
      
      else
      {
        cout << "WHAT!?: Invalid instruction: " << ins << endl; 
      }
      
    }
  } catch (ScanningFailure &f) {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  catch (std::out_of_range &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
  
  catch (int e) { // my exception
    std::cerr << "ERROR: " << e << std::endl;
    return 1;
  }



  return 0;
}
