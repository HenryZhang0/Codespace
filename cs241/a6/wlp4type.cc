
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

struct Procedure {
  string name;
  vector<string> params;
  string returnType;
  unordered_map<string, string> symbolTable;
};
unordered_map<string, Procedure> functionTable;
string currentFunction;

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
  if (node->lexeme != "!") {
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
vector<string> typeArglist(Node *node);
// factor → AMP lvalue
// factor → STAR factor
// factor → NEW INT LBRACK expr RBRACK
string typeId(Node *node) {
  if (functionTable[currentFunction].symbolTable.find(node->lexeme) == functionTable[currentFunction].symbolTable.end()) {
    cerr << "ERROR: undeclared variable " << node->lexeme << endl;
    exitt();
  }
  string type = functionTable[currentFunction].symbolTable[node->lexeme];
  node->type = type;
  return type;
}

string typeFunctionCall(Node *node) {
  if (functionTable.find(node->lexeme) == functionTable.end()) {
    cerr << "ERROR: undeclared function " << node->lexeme << endl;
    exitt();
  }
  Procedure proc = functionTable[node->lexeme];
  return proc.returnType;
}

string typeFactor(Node *node) {
  Node *factor;
  Node *expr;
  Node *id;
  Node *num;
  string type;
  if (node->rule == "factor ID") {
    type = typeId(node->children[0]);
    node->children[0]->type = type;
  } else if (node->rule == "factor NUM") {
    type = "int";
    node->children[0]->type = type;
  } else if (node->rule == "factor NULL") {
    type = "int*";
    node->children[0]->type = type;
  } else if (node->rule == "factor STAR factor") {
    type = typeFactor(node->children[1]);
    if (type != "int*") {
      cerr << "ERROR: factor is not an int*" << endl;
      exitt();
    }
    type = "int";
  } else if (node->rule == "factor AMP lvalue") {
    type = typeLvalue(node->children[1]);
    if (type != "int") {
      cerr << "ERROR: lvalue is not an int" << endl;
      exitt();
    }
    type = type + "*";
  } else if (node->children[0]->name == "factor LPAREN expr RPAREN") {
    type = typeExpr(node->children[1]);
  } else if (node->rule == "factor ID LPAREN RPAREN") {
    type = typeFunctionCall(node->children[0]);
  } else if (node->rule == "factor ID LPAREN arglist RPAREN") {
    Node *arglist = node->children[2];
    vector <string> argTypes;
    for (int i = 0; i < arglist->children.size(); i++) {
      argTypes.push_back(typeExpr(arglist->children[i]));
    }
    type = typeFunctionCall(node->children[0]);
  
    if (argTypes.size() != functionTable[node->children[0]->lexeme].params.size()) {
      cerr << "ERROR: arglist does not match function declaration" << endl;
      // destruct argTypes
      for (int i = 0; i < argTypes.size(); i++) {
        argTypes.pop_back();
      }
      exitt();
    }
    for (int i = 0; i < argTypes.size(); i++) {
      if (argTypes[i] != functionTable[node->children[0]->lexeme].params[i]) {
        cerr << "ERROR: arglist does not match function declaration" << endl;
        exitt();
      }
    }
  } else if (node->rule ==  "factor NEW INT LBRACK expr RBRACK") {
    type = typeExpr(node->children[3]);
    if (type != "int") {
      cerr << "ERROR: expr is not an int" << endl;
      exitt();
    }
  }
  node->type = type;
  return type;
}

string typeTerm(Node *node) {
  if (node->name != "term") return "not term";
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
  if (node->name != "expr") return "not expr";
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
  currentFunction = "wain";
  functionTable[currentFunction].name = "wain";

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

vector<string> typeParamlist(Node *node, bool declare = false) {
  string paramType = typeDcl(node->children[0]);
  if (declare) {
    functionTable[currentFunction].symbolTable[node->children[0]->children[1]->lexeme] = paramType;
  }
  vector<string> p1;
  if (node->rule == "paramlist dcl COMMA paramlist") {
    vector<string> p2 = typeParamlist(node->children[2]);
    p1.insert(p1.end(), p2.begin(), p2.end());
  }
  return p1;
}

vector<string> typeParams(Node *node, bool declare = false) {
  if (node->rule == "params paramlist") {
    return typeParamlist(node->children[0], declare);
  }
  return vector<string>();
}

void typeProcedure(Node *node) {
  if (node->name != "procedure") return;
  // add to procedure table
  string name = node->children[1]->lexeme;
  if (functionTable.find(name) != functionTable.end()) {
    cerr << "ERROR: procedure " << name << " already declared" << endl;
    exitt();
  }
  currentFunction = name;
  functionTable[name] = Procedure();
  functionTable[name].name = name;

  Node *params = node->children[3];
  functionTable[name].params = typeParams(params, true);

  Node *declarations = node->children[6];
  typeDcls(declarations);
  
  Node *statements = node->children[7];
  typeStatements(statements);
  
  Node *returnExpr = node->children[9];
  typeExpr(returnExpr);
  string returnType = typeExpr(returnExpr);
  functionTable[name].returnType = returnType;
}

void typeProcedures(Node *node) {
  if (node->name != "procedures") cout << "ERROR: not a procedure" << endl;
  if (node->rule == "procedures procedure procedures") {
    typeProcedure(node->children[0]);
    typeProcedures(node->children[1]);
  } else if (node->rule == "procedures main") {
    checkMain(node->children[0]);
  }
  return;
}

string typeDcl(Node *node) {
  if (node->name != "dcl") return "not dcl";
  Node *type = node->children[0];
  Node *id = node->children[1];
  // if already in symbol table, error
  if (functionTable[currentFunction].symbolTable.find(id->lexeme) != functionTable[currentFunction].symbolTable.end()) {
    cerr << "ERROR: duplicate variable " << id->lexeme << endl;
    exitt();
  }

  if (type->children.size() == 1) {
    functionTable[currentFunction].symbolTable[id->lexeme] = "int";
    id->type = "int";

    return "int";
  } else if (type->children.size() == 2) {
    id->type = "int*";
    functionTable[currentFunction].symbolTable[id->lexeme] = "int*";
    return "int*";
  } else {
    cerr << "ERROR: unknown type" << endl;
    exitt();
    return "unknown dcl";
  }
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

string typeLvalue(Node *node) {
  if (node->name != "lvalue") return "not lvalue";
  if (node->children.size() == 1) {
    Node *id = node->children[0];
    if (functionTable[currentFunction].symbolTable.find(id->lexeme) == functionTable[currentFunction].symbolTable.end()) {
      cerr << "ERROR: variable " << id->lexeme << " not declared" << endl;
      exitt();
    }
    string type = functionTable[currentFunction].symbolTable[id->lexeme];
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
  return "unknown lvalue";
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
  typeProcedures(node->children[1]);
}


int main() {
  root = scan(NULL);
  traverse(root);
  printNode(root);
  deleteNode(root);

  // print function table
  // for (auto it = functionTable.begin(); it != functionTable.end(); it++) {
  //   cout << "function: " << it->first << endl;
  //   cout << "return type: " << it->second.returnType << endl;
  //   cout << "symbol table: " << endl;
  //   for (auto it2 = it->second.symbolTable.begin(); it2 != it->second.symbolTable.end(); it2++) {
  //     cout << it2->first << " " << it2->second << endl;
  //   }
  // }
  return 0;
}