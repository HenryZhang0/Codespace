
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;


struct Node {
  std::string name;
  Node *parent;
  string lexeme;
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

 
  istringstream iss(s);
  string token;
  vector<string> tokens;
  while(iss >> token) {
    tokens.push_back(token);
  }
 
  newNode->name = tokens[0];
  newNode->parent = parent;
 
  if (isTerminal(tokens)) {
    newNode->lexeme = tokens[1];
    return newNode;
  }
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
Node* findChild(Node *node, string name) {
  for (int i = 0; i < node->children.size(); i++) {
    if (node->children[i]->name == name) {
      return node->children[i];
    }
  }
  return NULL;
}
void printNode(Node *node) {
  cout << node->name << endl;
  for (int i = 0; i < node->children.size(); i++) {
    printNode(node->children[i]);
  }
}


void checkMain(Node *node) {
  if (node->name != "main")  return;
  // find child with name "statement"
  Node *param1 = node->children[3];
  Node *param2 = node->children[5];
  Node *returnExpr = node->children[11];
  if (param1->children[1]->lexeme == param2->children[1]->lexeme) {
    cerr << "ERROR: wain params have same name " << param1->lexeme << " " << param2->lexeme << endl; 
    exit(1);
  }
  if (param2->children[0]->children[0]->name != "INT") {
    cerr << "ERROR: wain param 2 is not an int" << endl;
    exit(1);
  }
  if (returnExpr->children[0]->children[0]->children[0]->name != "NUM") {
    cerr << "ERROR: wain return is not an int" << endl;
    exit(1);
  } 
}
void traverse(Node *node) {
  checkMain(node);
  for (int i = 0; i < node->children.size(); i++) {
    traverse(node->children[i]);
  }
}

int main() {
  // read parse tree from stdin
  Node *root = scan(NULL);


  // printNode(root);
  traverse(root);
  return 0;
}