#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

enum SymbolType { NON_TERMINAL, TERMINAL };

struct Symbol {
    string name;
    SymbolType type;

    Symbol(string n, SymbolType t) : name(n), type(t) {}
    bool operator<(const Symbol& other) const { return name < other.name; }
};


map<string, string> symbolTable; // πίνακας συμβόλων
map<Symbol, set<string>> FIRST;
map<Symbol, set<string>> FOLLOW;
map<Symbol, map<string, vector<Symbol>>> parseTable;
stack<Symbol> parsingStack; // αρχική στοίβα για συντακτική ανάλυση
vector<string> tokens;
map<Symbol, vector<vector<Symbol>>> grammar;

void addRule(const Symbol& lhs, const vector<Symbol>& rhs) {
    grammar[lhs].push_back(rhs);
}

/*
Γραμματική (LL(1)):

E  → T E' ;
E' → + T E' | - T E' | ε
T  → F T'
T' → * F T' | / F T' | ε
F  → ( E ) | id | num
*/


void defineFirstAndFollow() {
    Symbol E("E", NON_TERMINAL), Ep("E'", NON_TERMINAL), T("T", NON_TERMINAL), Tp("T'", NON_TERMINAL), F("F", NON_TERMINAL);
    Symbol plus("+", TERMINAL), minus("-", TERMINAL), star("*", TERMINAL), div("/", TERMINAL), id("id", TERMINAL), open("(", TERMINAL), close(")", TERMINAL), eps("e", TERMINAL), num("num", TERMINAL), semi(";", TERMINAL);
    Symbol dbl("double", TERMINAL), flt("float", TERMINAL), in("int", TERMINAL), interval("Interval", TERMINAL), ivector("IntervalVector", TERMINAL);

    // FIRST
    FIRST[E] = {"(", "id", "num"};
    FIRST[Ep] = {"+", "-", "e"};
    FIRST[T] = {"(", "id", "num"};
    FIRST[Tp] = {"*", "/", "e"};
    FIRST[F] = {"(", "id", "num"};

    // FOLLOW
    FOLLOW[E] = {"$",")"};
    FOLLOW[Ep] = {"$", ";", ")"};
    FOLLOW[T] = {"+", "-", "$", ";", ")"};
    FOLLOW[Tp] = {"+", "-", "$", ";", ")"};
    FOLLOW[F] = {"*", "/", "+", "-", "$", ";", ")"};
}


void buildParseTable() {
    Symbol E("E", NON_TERMINAL), Ep("E'", NON_TERMINAL), T("T", NON_TERMINAL), Tp("T'", NON_TERMINAL), F("F", NON_TERMINAL);
    Symbol plus("+", TERMINAL), minus("-", TERMINAL), star("*", TERMINAL), div("/", TERMINAL), id("id", TERMINAL), open("(", TERMINAL), close(")", TERMINAL), eps("e", TERMINAL), num("num", TERMINAL), semi(";", TERMINAL);
    Symbol eq("=", TERMINAL);
    Symbol dbl("double", TERMINAL), flt("float", TERMINAL), in("int", TERMINAL), interval("Interval", TERMINAL), ivector("IntervalVector", TERMINAL);

    // πίνακας συντακτικής ανάλυσης
    parseTable[E]["id"] = {T, Ep, semi};
    parseTable[E]["("] = {T, Ep, semi};
    parseTable[E]["num"] = {T, Ep, semi};
    parseTable[E]["-"] = {T, Ep, semi};

    parseTable[Ep]["+"] = {plus, T, Ep};
    parseTable[Ep]["-"] = {minus, T, Ep};
    parseTable[Ep][";"] = {eps};
    parseTable[Ep]["$"] = {eps};
    parseTable[Ep][")"] = {eps};

    parseTable[T]["id"] = {F, Tp};
    parseTable[T]["("] = {F, Tp};
    parseTable[T]["num"] = {F, Tp};
    parseTable[T]["-"] = {F, Tp};

    parseTable[Tp]["*"] = {star, F, Tp};
    parseTable[Tp]["/"] = {div, F, Tp};
    parseTable[Tp]["+"] = {eps};
    parseTable[Tp]["-"] = {eps};
    parseTable[Tp][";"] = {eps};
    parseTable[Tp]["$"] = {eps};
    parseTable[Tp][")"] = {eps};


    parseTable[F]["id"] = {id};
    parseTable[F]["("] = {open, E, close};
    parseTable[F]["num"] = {num};
}


// το πραγματικό input του χρήστη
void load_input(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string input;
    getline(file, input); 
    file.close();

    cout << "Input loaded: " << input << endl;
}


// tokens
void loadTokens(const string& filename) {
    ifstream file(filename);
    string token;
    map<string, string> tokenMap = {
        {"T_id", "id"},
        {"T_plus", "+"},
        {"T_minus", "-"},
        {"T_mul", "*"},
        {"T_div", "/"},
        {"T_lparen", "("},
        {"T_rparen", ")"},
        {"T_semi", ";"},
        {"T_num", "num"},
        {"T_asign", "="},
        {"T_double", "double"},
        {"T_int", "int"},
        {"T_float", "float"},
        {"T_interval", "Interval"},
        {"T_IntervalVector", "IntervalVector"}
    };

    while (file >> token) {
        if (tokenMap.find(token) != tokenMap.end()) {
            tokens.push_back(tokenMap[token]);
        } else {
            cerr << "Error: Unrecognized token " << token << endl;
            exit(EXIT_FAILURE);
        }
    }

    tokens.push_back("$");
}


// stack automata / αυτόματο στοίβας
bool parse() {
    stack<Symbol> outputStack;
    parsingStack.push(Symbol("$", TERMINAL));
    parsingStack.push(Symbol("E", NON_TERMINAL));
    size_t index = 0;

    while (!parsingStack.empty()) {
        Symbol top = parsingStack.top();
        parsingStack.pop();
        string currentToken = tokens[index];

        cout << "Top of stack: " << top.name << ", Current token: " << currentToken << endl;

        if (top.type == TERMINAL) {
            if (top.name == currentToken) {
                index++;
                cout << "Match! Moving to next token." << endl;
                outputStack.push(top); // Αποθήκευση του τερματικού
                if (top.name == "id") {
                    symbolTable[currentToken] = "variable";
                } else if (top.name == "num") {
                    symbolTable[currentToken] = "integer";
                } else if (top.name == "+" || top.name == "-" || top.name == "*" || top.name == "/") {
                    symbolTable[currentToken] = "operator";
                } else if (top.name == ";") {
                    symbolTable[currentToken] = "semicolon";
                }
            } else {
                cerr << "Error: Unexpected token " << currentToken << " expected " << top.name << endl;
                return false;
            }
        } else {
            if (parseTable[top][currentToken].empty()) {
                cerr << "Error: No rule for " << top.name << " at " << currentToken << endl;
                return false;
            }
            auto rule = parseTable[top][currentToken];
            cout << "Applying rule: ";
            for (const auto& sym : rule) {
                cout << sym.name << " ";
            }
            cout << endl;

            for (auto it = rule.rbegin(); it != rule.rend(); ++it) {
                if (it->name != "e") {
                    parsingStack.push(*it);
                    cout << "Pushing to stack: " << it->name << endl;
                } else {
                    cout << "Epsilon rule applied." << endl;
                }
            }
        }
    }
    parsingStack = outputStack;
    return true;
}

struct AstNode {
    string name;
    string value;
    vector<AstNode*> children;

    AstNode(string n) : name(n), value("") {}
    AstNode(string n, string v) : name(n), value(v) {}
};


// Σημασιολογική ανάλυση
void semanticAnalysis(AstNode* root) {
    if (root == nullptr) {
        cout << "Error: No AST to perform semantic analysis on." << endl;
        return;
    }

    cout << "Starting Semantic Analysis..." << endl;

    stack<Symbol> tempStack;
    while (!parsingStack.empty()) {
        tempStack.push(parsingStack.top());
        parsingStack.pop();
    }
    parsingStack = tempStack;

    cout << "parsingStack size after reversal: " << parsingStack.size() << endl;

    size_t index = 0;  
    while (!parsingStack.empty() && index < tokens.size()) {
        // Παίρνουμε το τρέχον token από την tokens.
        string currentToken = tokens[index];
        Symbol symbol = parsingStack.top();
        parsingStack.pop();

        cout << "Processing token: " << currentToken << endl;

        if (symbol.type == TERMINAL) {
            if (symbol.name == "id") {
                if (symbolTable.find(currentToken) == symbolTable.end()) {
                    cerr << "Error: Undeclared variable " << currentToken << endl;
                } else {
                    cout << currentToken << " found in symbol table as variable.\n";
                }
            } 
            else if (symbol.name == "num") {
                if (symbolTable.find(currentToken) == symbolTable.end()) {
                    symbolTable[currentToken] = {"", "int"};
                    cout << "Added " << currentToken << " to symbol table as int.\n";
                } else {
                    cout << currentToken << " already exists in symbol table.\n";
                }
            } 
            else if (currentToken == "+" || currentToken == "*" || currentToken == "/" || currentToken == "-") {
                if (symbolTable.find(currentToken) == symbolTable.end()) {
                    symbolTable[currentToken] = {"", "operator"};
                    cout << "Added " << currentToken << " to symbol table as operator.\n";
                } else {
                    cout << currentToken << " already exists in symbol table.\n";
                }
            } 
            else if (currentToken == ";") {
                if (symbolTable.find(currentToken) == symbolTable.end()) {
                    symbolTable[currentToken] = {"", "semicolon"};
                    cout << "Added " << currentToken << " to symbol table as semicolon.\n";
                } else {
                    cout << currentToken << " already exists in symbol table.\n";
                }
            } 
        }

        index++;  
    }

    cout << "Semantic Analysis done." << endl;
}


void printSymbolTable() {
    cout << "\nSymbol Table Contents:\n";
    if (symbolTable.empty()) {
        cout << "Symbol table is empty.\n";
        return;
    }
    for (const auto& entry : symbolTable) {
        cout << "Name: " << entry.first << ", Type: " << entry.second << endl;
    }
    cout << "----------------------\n";
}



AstNode* createAstNode(string name, string value = "") {
    return new AstNode(name, value);
}

AstNode* buildAST(stack<Symbol> outputStack, const map<Symbol, vector<vector<Symbol>>>& grammar) {
    stack<AstNode*> astStack;

    // Αντιστροφή του outputStack για σωστή σειρά
    stack<Symbol> reversedOutput;
    while (!outputStack.empty()) {
        reversedOutput.push(outputStack.top());
        outputStack.pop();
    }

    cout << "Reversed Output Stack: ";
    stack<Symbol> temp = reversedOutput;
    while (!temp.empty()) {
        cout << temp.top().name << " ";
        temp.pop();
    }
    cout << endl;

    // Επεξεργασία της στοίβας
    while (!reversedOutput.empty()) {
        Symbol symbol = reversedOutput.top();
        reversedOutput.pop();

        if (symbol.type == TERMINAL) {
            // Δημιουργία φύλλου για τερματικό σύμβολο
            cout << "Creating leaf node for terminal: " << symbol.name << endl;
            astStack.push(new AstNode(symbol.name, symbol.name));
        } else if (symbol.type == NON_TERMINAL && grammar.count(symbol)) {
            // Δημιουργία κόμβου για μη-τερματικό σύμβολο
            AstNode* parent = new AstNode(symbol.name);
            cout << "Creating non-terminal node: " << symbol.name << endl;

            vector<vector<Symbol>> rules = grammar.at(symbol);

            if (!rules.empty()) {
                vector<Symbol> rule = rules[0];
                vector<AstNode*> children;

                cout << "Applying rule for: " << symbol.name << " -> ";
                for (const auto& sym : rule) {
                    cout << sym.name << " ";
                }
                cout << endl;

               if (astStack.size() >= rule.size()) {
                    for (auto it = rule.rbegin(); it != rule.rend(); ++it) {
                        if (!astStack.empty() && it->name != "ε") {
                            AstNode* child = astStack.top();
                            astStack.pop();
                            children.push_back(child);
                            cout << "Popped child: " << child->name << endl;
                        } else if (it->name == "e") {
                            cout << "Skipping epsilon symbol." << endl;
                        } else {
                            cerr << "Error: Stack is empty while processing children for rule." << endl;
                            return nullptr;
                        }
                    }

                    reverse(children.begin(), children.end());
                    parent->children = children;
                    cout << "Added children to " << parent->name << endl;
                } else {
                    cerr << "Error: Not enough children in stack for rule: " << symbol.name << endl;
                    return nullptr;
                }
        }
            astStack.push(parent); 
        } else {
            cerr << "Error: Unknown symbol type or missing grammar for symbol: " << symbol.name << endl;
        }
    }
    return astStack.top();
}

void printAST(AstNode* root, int depth = 0) {
    if (!root) {
        return;
    }

    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }

    cout << root->name;
    if (!root->value.empty()) {
        cout << " (value: " << root->value << ")";
    }
    cout << endl;

    for (auto* child : root->children) {
        printAST(child, depth + 1);
    }
}


int main() {
    Symbol E("E", NON_TERMINAL), Ep("E'", NON_TERMINAL), T("T", NON_TERMINAL), Tp("T'", NON_TERMINAL), F("F", NON_TERMINAL);
    Symbol plus("+", TERMINAL), minus("-", TERMINAL), star("*", TERMINAL), div("/", TERMINAL), id("id", TERMINAL), open("(", TERMINAL), close(")", TERMINAL), eps("e", TERMINAL), num("num", TERMINAL), semi(";", TERMINAL);
    Symbol eq("=", TERMINAL);
    Symbol dbl("double", TERMINAL), flt("float", TERMINAL), in("int", TERMINAL), interval("Interval", TERMINAL), ivector("IntervalVector", TERMINAL);

    // grammar rules
    addRule(E, {T, Ep,semi});
    addRule(Ep, {plus, T, Ep});
    addRule(Ep, {minus, T, Ep});
    addRule(Ep, {eps});
    addRule(T, {F, Tp});
    addRule(Tp, {star, F, Tp});
    addRule(Tp, {div, F, Tp});
    addRule(Tp, {eps});
    addRule(F, {id});
    addRule(F, {open, E, close});
    addRule(F, {minus, F});
    addRule(F, {num});
    addRule(F, {dbl});
    addRule(F, {flt});
    addRule(F, {in});
    addRule(F, {interval});
    addRule(F, {ivector});

    defineFirstAndFollow();
    buildParseTable();
    load_input("input_.txt");
    loadTokens("tokens.txt");

    if (parse()) {
        cout << "Parsing successful!" << endl;
    } else {
        cout << "Parsing failed!" << endl;
    }

    cout << endl;
    cout << endl;

    
    cout << "Symbols in parse stack (before AST): ";
    stack<Symbol> tempStack = parsingStack;
    while (!tempStack.empty()) {
        cout << tempStack.top().name << " ";
        tempStack.pop();
    }
    cout << endl;

    AstNode* root = buildAST(parsingStack, grammar);

    if (root == nullptr) {
        cerr << "Failed to construct AST!" << endl;
        return EXIT_FAILURE;
    }

    cout << "\nAbstract Syntax Tree:\n";
    printAST(root); 

    printSymbolTable();
    
    semanticAnalysis(root);
    return 0;
}



