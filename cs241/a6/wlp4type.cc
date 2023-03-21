
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
using namespace std;

struct Node {
  std::string name;
  string rule;
  Node *parent;
  string lexeme;
  string type;
  std::vector<Node*> children;
};
Node *root;
bool doneDeclarations = false;
// symbol table
unordered_map<string, string> symbolTable;

void deleteNode(Node *node) {
  for (int i = 0; i < node->children.size(); i++) {
    deleteNode(node->children[i]);
  }
  delete node;
}

void exitt() {
  deleteNode(root);
  exit(1);
}

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

string typeExpr(Node *node);
string typeFactor(Node *node);
string typeTerm(Node *node);
void typeStatement(Node *node);
void typeStatements(Node *node);
void typeDcls(Node *node);
string typeDcl(Node *node);
string typeLvalue(Node *node);
// factor → AMP lvalue
// factor → STAR factor
// factor → NEW INT LBRACK expr RBRACK
string typeFactor(Node *node) {
  if (node->name != "factor") return "";
  Node *factor;
  Node *expr;
  Node *id;
  Node *num;
  if (node->children.size() == 1) {

    factor = node->children[0];
    if (factor->name == "ID") {
      string type = symbolTable[factor->lexeme];
      factor->type = type;
      node->type = type;
      return type;
    } else if (factor->name == "NUM") {
      node->type = "int";
      factor->type = "int";
      return "int";
    } else if (factor->name == "NULL") {
      node->type = "int*";
      factor->type = "int*";
      return "int*";
    } else {
      cerr << "ERROR: unknown factor type" << factor->name << endl;
      exitt();
    }
  } else if (node->children.size() == 2) { 
    if (node->children[0]->name == "STAR") { // STAR factor
      string type = typeFactor(node->children[1]);
      // remove the star in type
      string star = type.substr(type.length() - 1, 1);
      string newType = type.substr(0, type.length() - 1);
      if (star != "*") {
        cerr << "ERROR: factor is not an int*" << endl;
        exitt();
      }
      node->type = newType;
      return newType;
    } else if (node->children[0]->name == "AMP") { // AMP lvalue
      string type = typeLvalue(node->children[1]);
      node->type = type + "*";
      return type + "*";
    }
    // factor = node->children[1];
    // string type = typeFactor(factor);
    // if (type != "int*") {
    //   cerr << "ERROR: factor is not an int*" << endl;
    //   exitt();
    // }
    // node->type = "int";
    return "int";
  } else if (node->children.size() == 3) {
    if (node->children[0]->name == "LPAREN") {
      expr = node->children[1];
      string type = typeExpr(expr);
      node->type = type;
      return type;
    } 
    
    return "int";
  } else if (node->children.size() == 4) {
    return "int";
  } else if (node->children.size() == 5) { // NEW INT LBRACK expr RBRACK
      string type = typeExpr(node->children[3]);
      if (type != "int") {
        cerr << "ERROR: expr is not an int" << endl;
        exitt();
      }
      node->type = "int*";
      return "int*";
  }
  return "";
  // else if (node->children.size() == 2) {
  //   factor = node->children[1];
  //   string type = typeFactor(factor);
  //   if (type != "int*") {
  //     cerr << "ERROR: factor is not an int*" << endl;
  //     exitt();
  //   }
  //   node->type = "int";
  //   return "int";
  // } else {
  //   expr = node->children[1];
  //   string type = typeExpr(expr);
  //   if (type != "int") {
  //     cerr << "ERROR: expr is not an int" << endl;
  //     exitt();
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
// expr → expr PLUS term
// expr → expr MINUS term
// term → term STAR factor
// term → term SLASH factor
// term → term PCT factor

string typeExpr(Node *node) {
  if (node->name != "expr") return "";
  Node *term;
  Node *oper;
  Node *expr;
  if (node->children.size() == 1) {
    term = node->children[0];
    string type = typeTerm(term);
    node->type = type;
    return type;
  } else {
    expr = node->children[0];
    oper = node->children[1];
    term = node->children[2];
    string type1 = typeExpr(expr);
    string type2 = typeTerm(term);
    string op = oper->name;
    if (type1 == "int*" && type2 == "int" && (op == "PLUS" || op == "MINUS")) {
      node->type = "int*";
      return "int*";
    } else if (type1 == "int" && type2 == "int*" && op == "PLUS") {
      node->type = "int*";
      return "int*";
    } else if (type1 == "int*" && type2 == "int*" && op == "MINUS") {
      node->type = "int";
      return "int";
    } else if (type1 == "int" && type2 == "int") {
      node->type = "int";
      return "int";
    } else {
      cerr << "ERROR: expr types do not match" << endl;
      exitt();
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
  Node *declarations = node->children[8];
  Node *statements = node->children[9];
  Node *returnExpr = node->children[11];
  typeDcl(param1);
  typeDcl(param2);
  typeDcls(declarations);
  typeStatements(statements);
  typeExpr(returnExpr);
  if (param1->children[1]->lexeme == param2->children[1]->lexeme) {
    cerr << "ERROR: wain params have same name " << param1->lexeme << " " << param2->lexeme << endl; 
    exitt();
  }
  if (param2->children[0]->children[0]->name != "INT" || param2->children[0]->children.size() == 2) {
    cerr << "ERROR: wain param 2 is not an int" << endl;
    exitt();
  }
  string returnType = typeExpr(returnExpr);
  if (returnType != "int") {
    cerr << "ERROR: wain return is not an int" << endl;
    exitt();
  } 
}

string typeDcl(Node *node) {
  if (node->name != "dcl") return "";
  Node *type = node->children[0];
  Node *id = node->children[1];
  // if already in symbol table, error
  if (symbolTable.find(id->lexeme) != symbolTable.end()) {
    cerr << "ERROR: duplicate variable " << id->lexeme << endl;
    exitt();
  }
  if (type->children.size() == 1) {
    id->type = "int";
    symbolTable[id->lexeme] = "int";
    return "int";
  } else if (type->children.size() == 2) {
    id->type = "int*";
    symbolTable[id->lexeme] = "int*";
    return "int*";
  } else {
    cerr << "ERROR: unknown type" << endl;
    exitt();
  }
  return "";
}

void typeDcls (Node *node) {
  if (node->name != "dcls") return;
  if (node->rule == "dcls .EMPTY") {
    return;
  } else {
    Node *dcls = node->children[0];
    typeDcls(dcls);
    Node *dcl = node->children[1];
    string dcl_type = typeDcl(dcl);


    Node *rvalue = node->children[3];
    if (rvalue->name == "NUM") {
      rvalue->type = "int";
    } else if (rvalue->name == "NULL") {
      rvalue->type = "int*";
    }
    
    string lvaluetype = dcl_type; // tdb
    string rvaluetype = rvalue->type;

    if ((lvaluetype == rvaluetype)) {

    } else {
      cerr << "!ERROR: lvalue and rvalue do not match: " << lvaluetype << " " << rvaluetype << endl;
      exitt();
    }

  }
}
// lvalue → ID
// lvalue → STAR factor
// lvalue → LPAREN lvalue RPAREN
string typeLvalue(Node *node) {
  if (node->name != "lvalue") return "";
  if (node->children.size() == 1) {
    Node *id = node->children[0];
    string type = symbolTable[id->lexeme];
    node->type = type;
    id->type = type;
    return type;
  } else if (node->children.size() == 2) { // STAR factor
    Node *factor = node->children[1];
    string type = typeFactor(factor);  
    string star = type.substr(type.length() - 1, 1);
    string newType = type.substr(0, type.length() - 1);
    if (star != "*") {
      cerr << "ERROR: factor is not an int*" << endl;
      exitt();
    }
    node->type = newType;
    return newType;
  } else if (node->children.size() == 3) {
    string type = typeLvalue(node->children[1]);
    node->type = type;
    return type;
  }
  return "";
}

void typeStatement(Node *node) {
  Node *state = node->children[0];
  if (state->name == "lvalue") {
    string type = typeLvalue(state);
    string rvalue = typeExpr(node->children[2]);
    if (type != rvalue) {
      cerr << "ERROR: lvalue and rvalue do not match: " << type << ", " << rvalue << endl;
      exitt();
    }
  } else {
    if (state->name == "IF") {
      typeStatements(state->children[5]);
      typeStatements(state->children[9]);
    } else if (state->name == "WHILE") {
      typeStatements(state->children[5]);
    } else if (state->name == "PRINTLN") {
      typeExpr(state->children[2]);
    } else if (state->name == "DELETE") {
      typeExpr(state->children[3]);
    }
  }
}

void typeStatements(Node *node) {
  if (node->name != "statements") return;
  if (node->rule == "statements .EMPTY") {
    return;
  } else {
    typeStatements(node->children[0]);
    Node *statement = node->children[1];
    typeStatement(statement);
  }
}

void traverse(Node *node) {
  checkMain(node);
  // typeStatements(node);
  for (int i = 0; i < node->children.size(); i++) {
    traverse(node->children[i]);
  }
}


int main() {
  root = scan(NULL);
  traverse(root);
  printNode(root);
  // print symbol table
  // for (auto it = symbolTable.begin(); it != symbolTable.end(); it++) {
  //   cout << it->first << " " << it->second << endl;
  // }
  
  deleteNode(root);
  return 0;
}