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

  getline(in, s); // skip
  while(std::getline(in, s)) {
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

  string INPUT;

  vector<string> reduction_sequence;
  vector<string> input_sequence;

  while(std::getline(in, s)) {
    if (s == ".ACTIONS") {
      break;
    }
    std::istringstream iss(s);
    std::string token;
    while(iss >> token) {
      input_sequence.push_back(token);
    }
  }

  while(std::getline(in, s)) {
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

      for (string s : rule.rhs) {
        if (s == ".EMPTY") {
          continue;
        } else {
          // if (reduction_sequence.back() != s) {
          //   cout << "ERROR: " << s << " != " << reduction_sequence.back() << endl;
          //   return 0;
          // }
          reduction_sequence.pop_back();

        }
      }
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

  return 0;
}