#include <iostream>
#include <string>
#include <vector>
#include "scanner.h"
#include "constants.h"
#include "unordered_map"

using namespace std;

int main() {
  std::string line;

  // my variables
  unordered_map<string, int> label_map;

  try {
    while (getline(std::cin, line)) {
      // This example code just prints the scanned tokens on each line.
      std::vector<Token> tokenLine = scan(line);

      // for(int i=0; i<tokenLine.size(); i++) { ... }
      // PRINT 
      // for (auto &token : tokenLine) std::cout << token << ' ';


// FIRST PASS ------------------------------------------------------------------------
      // validate format and grab labels

      std::vector<Token> tokenLineNoLabels;

      for (auto &token : tokenLine) { // remove labels and add to label map
        if (token.getKind() != Token::Kind::LABEL) {
          tokenLineNoLabels.push_back(token);
        } else {
          if (label_map.find(token.getLexeme()) != label_map.end()) {
            std::cerr << "Duplicate label: " << token.getLexeme() << std::endl;
            throw 1;
          }
          label_map[token.getLexeme()] = 0;
        }
      }

      if (!validFormat(tokenLineNoLabels, tokenLineNoLabels[0].getLexeme())) {
        std::cerr << "Invalid format" << std::endl;
        throw 1;
      }
// SECOND PASS ------------------------------------------------------------------------

      for (auto &token : tokenLine) std::cout << token << ' ';

    }
  } catch (ScanningFailure &f) {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  
  catch (int e) { // my exception
    std::cerr << "ERROR: " << e << std::endl;
    return 1;
  }

  return 0;
}
