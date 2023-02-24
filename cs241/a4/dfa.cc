#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
const std::string ALPHABET    = ".ALPHABET";
const std::string STATES      = ".STATES";
const std::string TRANSITIONS = ".TRANSITIONS";
const std::string INPUT       = ".INPUT";
const std::string EMPTY       = ".EMPTY";

using namespace std;
bool isChar(std::string s) {
  return s.length() == 1;
}
bool isRange(std::string s) {
  return s.length() == 3 && s[1] == '-';
}

// initialize unordered map
unordered_map<char, int> alphabet;
unordered_map<string, unordered_map<char, string>> transitions;
unordered_map<string, int> acceptings;
string init_state;
// Locations in the program that you should modify to store the
// DFA information have been marked with four-slash comments:
//// (Four-slash comment)
int main() {
  std::istream& in = std::cin;
  std::string s;

  std::getline(in, s); // Alphabet section (skip header)
  // Read characters or ranges separated by whitespace
  while(in >> s) {
    if (s == STATES) { 
      break; 
    } else {
      if (isChar(s)) {
        //// Variable 's[0]' is an alphabet symbol
        alphabet.insert({s[0], 1});
      } else if (isRange(s)) {
        for(char c = s[0]; c <= s[2]; ++c) {
          //// Variable 'c' is an alphabet symbol
          alphabet.insert({c, 1});
        }
      } 
    }
  }
  
  std::getline(in, s); // States section (skip header)
  // Read states separated by whitespace
  while(in >> s) {
    if (s == TRANSITIONS) { 
      break; 
    } else {
      static bool initial = true;
      bool accepting = false;
      if (s.back() == '!' && !isChar(s)) {
        accepting = true;
        s.pop_back();
      }
      //// Variable 's' contains the name of a state
      if (initial) {
        //// The state is initial
        initial = false;
        init_state = s;
        transitions.insert({s, {}});
      }
      if (accepting) {
        //// The state is accepting
        transitions.insert({s, {}});
        acceptings.insert({s, 1});
      }
    }
  }

  std::getline(in, s); // Transitions section (skip header)
  // Read transitions line-by-line
  while(std::getline(in, s)) {
    if (s == INPUT) { 
      // Note: Since we're reading line by line, once we encounter the
      // input header, we will already be on the line after the header
      break; 
    } else {
      std::string fromState, symbols, toState;
      std::istringstream line(s);
      line >> fromState;
      bool last;
      while(line >> s) {
        if(line.peek() == EOF) { // If we reached the last item on the line
          // Then it's the to-state
          toState = s;
        } else { // Otherwise, there is more stuff on the line
          // We expect a character or range
          if (isChar(s)) {
            symbols += s;
          } else if (isRange(s)) {
            for(char c = s[0]; c <= s[2]; ++c) {
              symbols += c;
            }
          }
        }
      }
      for ( char c : symbols ) {
        //// There is a transition from 'fromState' to 'toState' on 'c'
        transitions[fromState].insert({c, toState});
      }
    }
  }

  // Input section (already skipped header)
  while(in >> s) {
    //// Variable 's' contains an input string for the DFA
    string state = init_state;
    if (s != EMPTY) {
      for ( char c : s ) {
        if (transitions[state].find(c) == transitions[state].end()) {
          state = "";
          break;
        }
        state = transitions[state][c];
      }
    }
    cout << s << ' ';
    if (acceptings.find(state) != acceptings.end()) {
      cout << "true" << endl;
    } else {
      cout << "false" << endl;
    }
  }
}