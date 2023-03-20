
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;


struct Node {
  std::string name;
  Node *parent;
  std::vector<Node*> children;
};

bool isTerminal(vector<string> s) {
  if (s[1] == ".EMPTY") {
    return true;
  }
  for (int i = 0; i < s[0].length(); i++) {
    if (s[0][i] < 'A' || s[0][i] > 'Z') {
      return false;
    }
  }
  return true;
}

Node* scan(Node *parent) {
  Node *newNode = new Node();

  string s;
  if (!getline(cin, s)) {
    return NULL;
  }
  // cout << "HERE" << endl;

  newNode->name = s;
  newNode->parent = parent;
 
  istringstream iss(s);
  string token;
  vector<string> tokens;
  while(iss >> token) {
    tokens.push_back(token);
  }
 
  if (isTerminal(tokens)) {
    return newNode;
  }
  cout << newNode->name << endl;
  for (int i = 1; i < tokens.size(); i++) {
    Node *child = scan(newNode);
    newNode->children.push_back(child);
  }
  
  return newNode;
}

// void printNode(Node *node) {
//   // cout << node->name << endl;
//   for (int i = 0; i < node->children.size(); i++) {
//     cout << node->name << " children: " << node->children.size() << endl;
//     printNode(node->children[i]);
//   }
// }

void printNode(Node *node) {
  cout << node->name << endl;
  for (int i = 0; i < node->children.size(); i++) {
    printNode(node->children[i]);
  }
}

int main() {
  // read parse tree from stdin
  Node *root = scan(NULL);

  // print parse tree to stdout
  // cout << root->name << endl;
  // for (int i = 0; i < root->children.size(); i++) {
  //   cout << root->children[i]->name << endl;
  // }

  printNode(root);
  return 0;
}