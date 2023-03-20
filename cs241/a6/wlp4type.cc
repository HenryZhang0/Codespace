
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;


struct Node {
  std::string name;
  string rule;
  Node *parent;
  string lexeme;
  string type;
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
  newNode->rule = s;
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

Node* findChild(Node *node, string name) {
  for (int i = 0; i < node->children.size(); i++) {
    if (node->children[i]->name == name) {
      return node->children[i];
    }
  }
  return NULL;
}

void printNode(Node *node) {
  cout << node->rule;
  if (node->lexeme != "") {
    // cout << " " << node->lexeme;
  }
  if (node->type != "") {
    cout << " : " << node->type;
  }
  cout << endl;
  for (int i = 0; i < node->children.size(); i++) {
    printNode(node->children[i]);
  }
}


void typeDcl(Node *node) {
  if (node->name != "dcl") return;
  Node *type = node->children[0];
  Node *id = node->children[1];
  if (type->children[0]->name == "INT") {
    id->type = "int";
  } else if (type->children[0]->name == "INTSTAR") {
    id->type = "int*";
  } else {
    cerr << "ERROR: unknown type" << endl;
    exit(1);
  }
}

string typeFactor(Node *node) {
  if (node->name != "factor") return "";
  Node *factor;
  Node *expr;
  Node *id;
  Node *num;
  if (node->children.size() == 1) {

    factor = node->children[0];
    if (factor->name == "ID") {
      // id = factor->children[0];
      // node->type = id->type;
      // return id->type;
      return "temp";
    } else if (factor->name == "NUM") {
      node->type = "int";
      factor->type = "int";
      return "int";
    } else if (factor->name == "NULL") {
      node->type = "null";
      return "null";
    } else {
      cerr << "ERROR: unknown factor type" << factor->name << endl;
      exit(1);
    }
  }
  return "";
  // else if (node->children.size() == 2) {
  //   factor = node->children[1];
  //   string type = typeFactor(factor);
  //   if (type != "int*") {
  //     cerr << "ERROR: factor is not an int*" << endl;
  //     exit(1);
  //   }
  //   node->type = "int";
  //   return "int";
  // } else {
  //   expr = node->children[1];
  //   string type = typeExpr(expr);
  //   if (type != "int") {
  //     cerr << "ERROR: expr is not an int" << endl;
  //     exit(1);
  //   }
  //   node->type = "int";
  //   return "int";
  // }
}
string typeTerm(Node *node) {
  if (node->name != "term") return "";
  Node *factor;
  Node *term;
  if (node->children.size() == 1) {
    factor = node->children[0];
    string type = typeFactor(factor);
    node->type = type;
    return type;
  } else {
    term = node->children[0];
    factor = node->children[2];
    string type1 = typeTerm(term);
    string type2 = typeFactor(factor);
    node->type = type1;
    return type1;
  }
  
}

string typeExpr(Node *node) {
  if (node->name != "expr") return "";
  Node *term;
  Node *expr;
  if (node->children.size() == 1) {
    term = node->children[0];
    string type = typeTerm(term);
    node->type = type;
    return type;
  } else {
    expr = node->children[0];
    term = node->children[2];
    string type1 = typeExpr(expr);
    string type2 = typeTerm(term);
    if (type1 != type2) {
      cerr << "ERROR: expr types do not match" << endl;
      exit(1);
    }
    node->type = type1;
    return type1;
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
  string returnType = typeExpr(returnExpr);
  if (returnType != "int") {
    cerr << "ERROR: wain return is not an int" << endl;
    exit(1);
  } 
}


void traverse(Node *node) {
  checkMain(node);
  typeDcl(node);
  for (int i = 0; i < node->children.size(); i++) {
    traverse(node->children[i]);
  }
}

void deleteNode(Node *node) {
  for (int i = 0; i < node->children.size(); i++) {
    deleteNode(node->children[i]);
  }
  delete node;
}

int main() {
  Node *root = scan(NULL);
  traverse(root);
  printNode(root);

  deleteNode(root);
  return 0;
}