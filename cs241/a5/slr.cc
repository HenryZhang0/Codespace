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
unordered_map<int, unordered_map<string, int>> transitions;
unordered_map<int, unordered_map<string, int>> reductions;

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
    if (s == ".TRANSITIONS") {
      break;
    }
    std::istringstream iss(s);
    std::string token;
    while(iss >> token) {
      input_sequence.push_back(token);
    }
  }
  
  

  while(std::getline(in, s)) {
    if (s == ".REDUCTIONS") {
      break;
    }
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while(iss >> token) {
      tokens.push_back(token);
    }
    try {
      int state = stoi(tokens[0]);
      string symbol = tokens[1];
      int next_state = stoi(tokens[2]);
      transitions[state][symbol] = next_state;
    } catch (exception e) {
      cout << "ERROR: " << s << endl;
    }
  }
  // get reductions
  while(std::getline(in, s)) {
    if (s == ".END") {
      break;
    }
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while(iss >> token) {
      tokens.push_back(token);
    }
    try {
      int state = stoi(tokens[0]);
      int rule_num = stoi(tokens[1]);
      string symbol = tokens[2];
      reductions[state][symbol] = rule_num;
    } catch (exception e) {
      cout << "ERROR: " << s << endl;
    }
  }

  // print reductions
  for (auto state : reductions) {
    cout << state.first << ": ";
    for (auto reduction : state.second) {
      cout << reduction.first << " -> " << reduction.second << ", ";
    }
    cout << endl;
  }
  cout << endl;

// SLR(1) parsing algorithm
// 1:  stateStack.push q0
// 2: for each symbol a in ` x a from left to right do
// 3: while Reduce[stateStack.top, a] is some production B → γ do
// 4: symStack.pop symbols in γ
// 5: stateStack.pop |γ| states
// 6: symStack.push B
// 7: stateStack.push δ[stateStack.top, B]
// 8: end while
// 9: symStack.push a
// 10: reject if δ[stateStack.top, a] is undefined
// 11: stateStack.push δ[stateStack.top, a]
// 12: end for
// 13: accept

  vector<int> state_stack;
  state_stack = {0};
  // symbol stack
  vector<string> symbol_stack;

  // print function
  // auto print = [&]() {
  //   for(int i = 0; i < symbol_stack.size(); ++i) {
  //     cout << symbol_stack[i] << " ";
  //   }
  //   cout << ". ";
  //   for(int j = 0; j < input_sequence.size(); ++j) {
  //     cout << input_sequence[j] << " ";
  //   }
  //   cout << endl;
  // };

  for (int i = 0; i <= input_sequence.size(); ++i) {
    // print state
    {
      for(int i = 0; i < symbol_stack.size(); ++i) {
        cout << symbol_stack[i] << " ";
      }
      cout << ". ";
      for(int j = i; j < input_sequence.size(); ++j) {
        cout << input_sequence[j] << " ";
      }
      cout << endl;
    }
    
    string a = input_sequence[i];
    if (i == input_sequence.size()) {
      a = "";
    }
    while (reductions[state_stack.back()].find(a) != reductions[state_stack.back()].end() || (a == "" && reductions[state_stack.back()].find(".ACCEPT") != reductions[state_stack.back()].end())) {
      int rule_num = reductions[state_stack.back()][a];
      Rule rule = rules[rule_num];
      

      for (int j = 0; j < rule.rhs.size(); ++j) {
        symbol_stack.pop_back();
        state_stack.pop_back();
      }
      
      symbol_stack.push_back(rule.lhs);
      state_stack.push_back(transitions[state_stack.back()][rule.lhs]);
      // reduction_sequence.push_back(rule.lhs + " -> " + to_string(rule_num));
      // print


      for(int i = 0; i < symbol_stack.size(); ++i) {
        cout << symbol_stack[i] << " ";
      }
      cout << ". ";
      for(int j = i; j < input_sequence.size(); ++j) {
        cout << input_sequence[j] << " ";
      }
      cout << endl;
    }

    if (a != "")
      symbol_stack.push_back(a);
    else
      break;
    if (transitions[state_stack.back()].find(a) == transitions[state_stack.back()].end()) {
      // if (reductions [state_stack.back()].find(".ACCEPT") != reductions[state_stack.back()].end()) {
      //   int rule_num = reductions[state_stack.back()][".ACCEPT"];
      //   Rule rule = rules[rule_num];
      //   for (int j = 0; j < rule.rhs.size(); ++j) {
      //     symbol_stack.pop_back();
      //     state_stack.pop_back();
      //   }
      //  symbol_stack.push_back(rule.lhs); 

      //   for(int i = 0; i < symbol_stack.size(); ++i) {
      //     cout << symbol_stack[i] << " ";
      //   }
      //   cout << ". ";
      //   for(int j = i; j < input_sequence.size(); ++j) {
      //     cout << input_sequence[j] << " ";
      //   }
      //   cout << endl;
      //   return 0;
      // }
      
      cout << "ERROR" << endl;
      return 0;
      
    }
    state_stack.push_back(transitions[state_stack.back()][a]);
  }

  // for (int i = 0; i < input_sequence.size(); ++i) {
  //   {
  //     cout << "State stack: ";
  //     for(int j = 0; j < state_stack.size(); ++j) {
  //       cout << state_stack[j] << " ";
  //     }
  //     cout << endl;
  //     for(int i = 0; i < reduction_sequence.size(); ++i) {
  //       cout << reduction_sequence[i] << " ";
  //     }
  //     cout << ". ";
  //     for(int j = i; j < input_sequence.size(); ++j) {
  //       cout << input_sequence[j] << " ";
  //     }
  //     cout << endl;
  //   }}

  
  cout << "ACCEPT" << endl;
  return 0;
}