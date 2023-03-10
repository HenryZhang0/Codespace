#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Rule {
  string lhs;
  vector<string> rhs;
};
vector<Rule> rules;

int main() {
  std::istream& in = std::cin;
  std::string s;

  // read in rules line by line
  getline(in, s); // skip
  while(std::getline(in, s)) {
    // split line into tokens
    if (s == ".INPUT") {
      break;
    }
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while(iss >> token) {
      tokens.push_back(token);
    }
    string lhs = tokens[0];
    vector<string> rhs;
    for(int i = 1; i < tokens.size(); ++i) {
      rhs.push_back(tokens[i]);
    }
    rules.push_back({lhs, rhs});
  }

  // read input
  string INPUT;
  getline(in, INPUT);

  vector<string> reduction_sequence;
  vector<string> input_sequence;
  // split input into tokens
  std::istringstream iss(INPUT);
  std::string token;
  while(iss >> token) {
    input_sequence.push_back(token);
  }

// actions are of the form:
// shift means to remove the leftmost element of the input sequence, and add it to the right end of the reduction sequence.
// reduce n, where n is a non-negative integer, means to look up rule n in the CFG (where the first rule is rule 0); remove the right-hand-side of the rule from the right end of the reduction sequence, and add the left-hand-side of the rule to the right end of the reduction sequence.
// print means to print a line containing the reduction sequence, followed by a dot (.), followed by the input sequence. There should be a single space separating each printed element (including the dot) and a line feed should be printed after the last element of the input sequence. This action does not modify the reduction sequence or input sequence, and is simply used to view the current progress of parsing.

  // read actions line by line
  while(std::getline(in, s)) {
    // split line into tokens
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while(iss >> token) {
      tokens.push_back(token);
    }
    string action = tokens[0];
    if(action == "shift") {
      reduction_sequence.push_back(input_sequence[0]);
      input_sequence.erase(input_sequence.begin());
    } else if(action == "reduce") {
      int rule_num = stoi(tokens[1]);
      Rule rule = rules[rule_num];
      // remove rhs from reduction sequence
      // reverse order
      for (string s : rule.rhs) {
        if (s == ".EMPTY") {
          continue;
        } else {
          // if (reduction_sequence.back() != s) {
          //   cout << "ERROR: " << s << " != " << reduction_sequence.back() << endl;
          //   return 0;
          // }
          // print reduction sequence
          reduction_sequence.pop_back();

        }
      }
      // add lhs to reduction sequence
      reduction_sequence.push_back(rule.lhs);

    } else if(action == "print") {
      for(int i = 0; i < reduction_sequence.size(); ++i) {
        cout << reduction_sequence[i] << " ";
      }
      cout << ". ";
      for(int i = 0; i < input_sequence.size(); ++i) {
        cout << input_sequence[i] << " ";
      }
      cout << endl;
    } else if (action == ".END") {
      break;
    }
  }

  // print rules
  // for(int i = 0; i < rules.size(); ++i) {
  //   cout << rules[i].lhs << " -> ";
  //   for(int j = 0; j < rules[i].rhs.size(); ++j) {
  //     cout << rules[i].rhs[j] << " ";
  //   }
  //   cout << endl;
  // } 


    
  return 0;
}