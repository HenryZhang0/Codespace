#include "wlp4data.h"


#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <queue>

using namespace std;

struct Rule {
  string lhs;
  vector<string> rhs;
};
struct Node {
  int rule_num;
};
vector<Node> parse_tree;

// print parse tree in reverse
  // for (int i = parse_tree.size() - 1; i >= 0; --i) {
  //   // print rule 
  //   Rule rule = rules[parse_tree[i].rule_num];
  //   cout << rule.lhs << " ";
  //   for (auto r : rule.rhs) {
  //     cout << r << " ";
  //   }
  //   cout << endl;
  // }

vector<Rule> rules;
unordered_map<int, unordered_map<string, int>> transitions;
unordered_map<int, unordered_map<string, int>> reductions;
unordered_map<string, queue<string>> nonterminals;
void printTree(int index, int indention = 0) {
  if (index < 0) {
    return;
  }
  Rule rule = rules[parse_tree[index].rule_num];
  // print rule
  for (int i = 0; i < indention; ++i)  cout << " ";
  
  cout << rule.lhs << " ";

  for (auto r : rule.rhs) {
    cout << r << " ";
  }
  cout << endl;

  int last_reduction = index;
  for (auto r : rule.rhs) {
    // if r is a terminal
    if (nonterminals.find(r) != nonterminals.end()) {
      for (int i = 0; i < indention; ++i)  cout << " ";
      cout << r << " " << nonterminals[r].front() << endl;
      nonterminals[r].pop();
    } else {
      // find index of r in parse_tree
      for (int i = index - 1; i >= 0; --i) {
        if (parse_tree[i].rule_num == -1) {
          continue;
        }
        Rule rule2 = rules[parse_tree[i].rule_num];
        if (rule2.lhs == r) {
          // parse_tree[i].rule_num = -1;
          printTree(i, indention);
          break;
        }
      }
    }
  }
}
int main() {
  // std::istream& in = std::cin;
  std::istringstream in(WLP4_COMBINED);
  std::string s;

  // get rules from WLP4_CFG

  getline(in, s); // skip
  while(std::getline(in, s)) {
    if (s == ".TRANSITIONS") {
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

  // print rules
  // for (int i = 0; i < rules.size(); ++i) {
  //   cout << rules[i].lhs << " -> ";
  //   for (int j = 0; j < rules[i].rhs.size(); ++j) {
  //     cout << rules[i].rhs[j] << " ";
  //   }
  //   cout << endl;
  // }
  // return 0;


  
  

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

  // print transitions
  // for (auto it = transitions.begin(); it != transitions.end(); ++it) {
  //   cout << it->first << ": ";
  //   for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
  //     cout << it2->first << " -> " << it2->second << ", ";
  //   }
  //   cout << endl;
  // }
  // return 0;

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
  // for (auto it = reductions.begin(); it != reductions.end(); ++it) {
  //   cout << it->first << ": ";
  //   for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
  //     cout << it2->first << " -> " << it2->second << ", ";
  //   }
  //   cout << endl;
  // }
  // return 0;

  vector<string> reduction_sequence;
  vector<string> input_sequence;
  vector<string> lexeme_sequence;
  input_sequence.push_back("BOF");
  lexeme_sequence.push_back("BOF");
nonterminals["BOF"].push("BOF");
nonterminals["EOF"].push("EOF");

  std::istream& input = std::cin;
  while(std::getline(input, s)) {
    // first word is input_sequence
    // print second word
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while(iss >> token) {
      tokens.push_back(token);
    }
    nonterminals[tokens[0]].push(tokens[1]);
    input_sequence.push_back(tokens[0]);
    lexeme_sequence.push_back(tokens[1]);
  }
  input_sequence.push_back("EOF");
  lexeme_sequence.push_back("EOF");

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
  vector<string> symbol_stack;

  // contruct parse tree
  
  int parse_tree_index = 0;
  

  for (int i = 0; i <= input_sequence.size(); ++i) {
    // print state
    // {
    //   for(int i = 0; i < symbol_stack.size(); ++i) {
    //     cout << symbol_stack[i] << " ";
    //   }
    //   cout << ". ";
    //   for(int j = i; j < input_sequence.size(); ++j) {
    //     cout << input_sequence[j] << " ";
    //   }
    //   cout << endl;
    // }
    
    string a = "";
    if (i != input_sequence.size()) {
      a = input_sequence[i];
    }
    while (reductions[state_stack.back()].find(a) != reductions[state_stack.back()].end() || (a == "" && reductions[state_stack.back()].find(".ACCEPT") != reductions[state_stack.back()].end())) {
      int rule_num = reductions[state_stack.back()][a];
      Rule rule = rules[rule_num];
      // print rule
      // cout << rule.lhs << " -> ";
      // for (auto r : rule.rhs) {
      //   cout << r << " ";
      // }
      // cout << endl;
      
      Node node = {rule_num};
      parse_tree.push_back(node);

      for (auto r : rule.rhs) {
        if (r == ".EMPTY") {
          continue;
        }
        symbol_stack.pop_back();
        state_stack.pop_back();
      }
      
      symbol_stack.push_back(rule.lhs);
      state_stack.push_back(transitions[state_stack.back()][rule.lhs]);
      
      // print
      // for(int i = 0; i < symbol_stack.size(); ++i) {
      //   cout << symbol_stack[i] << " ";
      // }
      // cout << ". ";
      // for(int j = i; j < input_sequence.size(); ++j) {
      //   cout << input_sequence[j] << " ";
      // }
      // cout << endl;
    }
    

    if (a != "")
      symbol_stack.push_back(a);
    else
      break;
    if (transitions[state_stack.back()].find(a) == transitions[state_stack.back()].end()) {
      cerr << "ERROR at " << i << endl;
      return 0;
    }
    state_stack.push_back(transitions[state_stack.back()][a]);
  }

  
  printTree(parse_tree.size() - 1);
  return 0;
}