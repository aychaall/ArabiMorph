#include "../include/BinarySearchTree.h"
#include <iostream>
#include <functional>
using namespace std;

BinarySearchTree::BinarySearchTree() : m_Root(nullptr) {}
BinarySearchTree::~BinarySearchTree() {
    destroy(m_Root);
}
void BinarySearchTree::destroy(Node* node) {
    if(node == nullptr) return;
    destroy(node->getLeft());
    destroy(node->getRight());
    delete node;
}

Node* BinarySearchTree::insert(Node* node, Root r){
    if(node == nullptr) return new Node(r);

    if(r.getRoot() < node->getData())
        node->setLeft(insert(node->getLeft(), r));
    else if(r.getRoot() > node->getData())
        node->setRight(insert(node->getRight(), r));

    return node;
}

Node* BinarySearchTree::search(Node* node, string value){
    if(node == nullptr) return nullptr;

    if(node->getData() == value) return node;
    else if(value < node->getData()) return search(node->getLeft(), value);
    else return search(node->getRight(), value);
}

void BinarySearchTree::inorder(Node* node){
    if(node == nullptr) return;
    inorder(node->getLeft());
    cout << node->getData() << " ";
    inorder(node->getRight());
}

void BinarySearchTree::inorder(Node* node, vector<Root>& roots){
    if(node == nullptr) return;
    inorder(node->getLeft(), roots);
    roots.push_back(Root(node->getData()));
}

void BinarySearchTree::insert(Root r){
    m_Root = insert(m_Root, r);
}

bool BinarySearchTree::search(string value){
    return search(m_Root, value) != nullptr;
}

Node* BinarySearchTree::getRootNode(string value){
    return search(m_Root, value);
}

void BinarySearchTree::display(){
    inorder(m_Root);
    cout << endl;
}

// ─────────────────────────────────────────────
//  Internal helpers
// ─────────────────────────────────────────────

// Draws a single node as a framed box:
//   ╔══════════════╗
//   ║  كتب         ║
//   ╠══════════════╣  <- only if derivatives exist
//   ║  ├─ كاتب x2  ║
//   ║  └─ مكتبة x1 ║
//   ╚══════════════╝
static void drawNodeBox(const string& rootName,
                        const vector<string>& derivs,
                        const function<int(const string&)>& freq)
{
    // measure the widest line (ASCII width only — Arabic chars are wide)
    size_t w = rootName.size() + 4;
    for(auto& d : derivs){
        size_t cand = d.size() + 10; // "  ├─  (xN)"
        if(cand > w) w = cand;
    }
    if(w < 20) w = 20;

    string bar(w, '=');
    string thinBar(w, '-');

    // top border
    cout << "  \u2554" << bar << "\u2557" << endl;

    // root name row
    cout << "  \u2551  " << rootName
         << string(w - rootName.size() - 2, ' ')
         << "  \u2551" << endl;

    if(!derivs.empty()){
        // divider
        cout << "  \u2560" << thinBar << "\u2563" << endl;

        // each derivative
        for(int i = 0; i < (int)derivs.size(); i++){
            bool last = (i == (int)derivs.size() - 1);
            string branch = last ? "  \u2514\u2500 " : "  \u251C\u2500 ";
            string freqStr = " (x" + to_string(freq(derivs[i])) + ")";
            string line = branch + derivs[i] + freqStr;
            size_t pad = w - derivs[i].size() - freqStr.size() - 4;
            cout << "  \u2551" << line << string(pad > w ? 0 : pad, ' ') << "\u2551" << endl;
        }
    }

    // bottom border
    cout << "  \u255A" << bar << "\u255D" << endl;
}

// Recursively prints the tree with branch lines
void BinarySearchTree::printNodeBox(Node* node, const string& prefix, bool isLeft, bool isRight){
    if(node == nullptr) return;

    Root& r    = node->getRootObject();
    auto derivs = r.getDerivativesList();
    auto freqFn = [&](const string& d){ return r.getFrequency(d); };

    bool hasLeft  = node->getLeft()  != nullptr;
    bool hasRight = node->getRight() != nullptr;

    // branch line leading into this node
    string branchLine = isLeft  ? "\u251C\u2500\u2500 [L] "   // ├── [L]
                      : isRight ? "\u2514\u2500\u2500 [R] "   // └── [R]
                      :           "";

    cout << prefix << branchLine << endl;

    // figure out the prefix for the node box and its children
    string boxPrefix  = prefix + (isRight ? "    " : "\u2502   ");
    string childPfx   = prefix + (isRight ? "    " : "\u2502   ");

    // print the node box
    // reuse drawNodeBox but prepend the boxPrefix on each line
    // We'll do it inline for simplicity
    size_t w = r.getRoot().size() + 4;
    for(auto& d : derivs){
        size_t cand = d.size() + 12;
        if(cand > w) w = cand;
    }
    if(w < 18) w = 18;

    string bar(w, '=');
    string thinBar(w, '-');

    cout << boxPrefix << "\u2554" << bar << "\u2557" << endl;
    cout << boxPrefix << "\u2551  " << r.getRoot()
         << string(w > r.getRoot().size()+2 ? w - r.getRoot().size() - 2 : 0, ' ')
         << "  \u2551" << endl;

    if(!derivs.empty()){
        cout << boxPrefix << "\u2560" << thinBar << "\u2563" << endl;
        for(int i = 0; i < (int)derivs.size(); i++){
            bool last = (i == (int)derivs.size() - 1);
            string branch = last ? "  \u2514\u2500 " : "  \u251C\u2500 ";
            string freqStr = " (x" + to_string(r.getFrequency(derivs[i])) + ")";
            string content = branch + derivs[i] + freqStr;
            int pad = (int)w - (int)derivs[i].size() - (int)freqStr.size() - 4;
            cout << boxPrefix << "\u2551" << content
                 << string(pad > 0 ? pad : 0, ' ') << "\u2551" << endl;
        }
    }
    cout << boxPrefix << "\u255A" << bar << "\u255D" << endl;

    // recurse into children
    if(hasLeft)  printNodeBox(node->getLeft(),  childPfx, true,  false);
    if(hasRight) printNodeBox(node->getRight(), childPfx, false, true);
}

// ─────────────────────────────────────────────
//  Public display functions
// ─────────────────────────────────────────────

void BinarySearchTree::displayStructured(){
    if(m_Root == nullptr){
        cout << "\n  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << endl;
        cout <<   "  \u2551   \u2205  Tree is empty               \u2551" << endl;
        cout <<   "  \u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D\n" << endl;
        return;
    }

    int total  = getNodeCount();
    int height = getHeight();

    // header banner
    cout << "\n  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << endl;
    cout <<   "  \u2551   Binary Search Tree                  \u2551" << endl;
    cout <<   "  \u2551   Nodes: " << total
         << "   |   Height: " << height
         << string(20 - to_string(total).size() - to_string(height).size(), ' ')
         << "\u2551" << endl;
    cout <<   "  \u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D" << endl;

    // root label
    cout << "\n           [ROOT]" << endl;
    cout <<   "             \u2502" << endl;

    // draw root node box
    Root& r    = m_Root->getRootObject();
    auto derivs = r.getDerivativesList();

    size_t w = r.getRoot().size() + 4;
    for(auto& d : derivs){ size_t c = d.size()+12; if(c>w) w=c; }
    if(w < 18) w = 18;
    string bar(w,'='), thin(w,'-');

    cout << "  \u2554" << bar << "\u2557" << endl;
    cout << "  \u2551  " << r.getRoot()
         << string(w > r.getRoot().size()+2 ? w-r.getRoot().size()-2 : 0,' ')
         << "  \u2551" << endl;

    if(!derivs.empty()){
        cout << "  \u2560" << thin << "\u2563" << endl;
        for(int i=0;i<(int)derivs.size();i++){
            bool last = (i==(int)derivs.size()-1);
            string br  = last ? "  \u2514\u2500 " : "  \u251C\u2500 ";
            string fq  = " (x"+to_string(r.getFrequency(derivs[i]))+")";
            string content = br + derivs[i] + fq;
            int pad = (int)w-(int)derivs[i].size()-(int)fq.size()-4;
            cout << "  \u2551" << content << string(pad>0?pad:0,' ') << "\u2551" << endl;
        }
    }
    cout << "  \u255A" << bar << "\u255D" << endl;

    // children
    if(m_Root->getLeft())  printNodeBox(m_Root->getLeft(),  "\u2502   ", true,  false);
    if(m_Root->getRight()) printNodeBox(m_Root->getRight(), "\u2502   ", false, true);

    cout << endl;
}

void BinarySearchTree::displayRootWithDerivatives(string rootName){
    Node* node = getRootNode(rootName);

    if(node == nullptr){
        cout << "\n  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << endl;
        cout <<   "  \u2551  Not found: " << rootName
             << string(14 > rootName.size() ? 14-rootName.size() : 0, ' ') << "  \u2551" << endl;
        cout <<   "  \u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D\n" << endl;
        return;
    }

    Root& r    = node->getRootObject();
    auto derivs = r.getDerivativesList();

    size_t w = max(rootName.size(), (size_t)24);
    for(auto& d : derivs){ size_t c=d.size()+12; if(c>w) w=c; }
    string bar(w,'='), thin(w,'-');

    cout << "\n  \u2554" << bar << "\u2557" << endl;
    cout <<   "  \u2551  " << rootName
         << string(w > rootName.size()+2 ? w-rootName.size()-2 : 0,' ')
         << "  \u2551" << endl;
    cout <<   "  \u2560" << thin << "\u2563" << endl;
    cout <<   "  \u2551  Derivatives: " << derivs.size()
         << string(w > 16 ? w-16 : 0,' ') << "  \u2551" << endl;
    cout <<   "  \u2560" << thin << "\u2563" << endl;

    if(derivs.empty()){
        cout << "  \u2551  -- No derivatives"
             << string(w > 18 ? w-18 : 0, ' ') << "  \u2551" << endl;
    } else {
        for(int i=0;i<(int)derivs.size();i++){
            bool last = (i==(int)derivs.size()-1);
            string br  = last ? "  \u2514\u2500 " : "  \u251C\u2500 ";
            string fq  = " (x"+to_string(r.getFrequency(derivs[i]))+")";
            string content = br + derivs[i] + fq;
            int pad = (int)w-(int)derivs[i].size()-(int)fq.size()-4;
            cout << "  \u2551" << content << string(pad>0?pad:0,' ') << "\u2551" << endl;
        }
    }
    cout << "  \u255A" << bar << "\u255D\n" << endl;
}

bool BinarySearchTree::loadRootsFromFile(const string& filename){
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "خطأ في فتح الملف (Error opening file): " << filename << endl;
        return false;
    }
    
    file.imbue(locale("en_US.UTF-8"));
    
    string line;
    int count = 0;
    while(getline(file, line)) {
        if(!line.empty()) {
            Root r(line);
            insert(r);
            count++;
        }
    }
    
    file.close();
    cout << "تم تحميل " << count << " جذر (Loaded " << count << " roots)" << endl;
    return true;
}


vector<Root> BinarySearchTree::getAllRoots(){
    vector<Root> roots;
    inorder(m_Root, roots);
    return roots;
}

int BinarySearchTree::getHeight(Node* node){
    if(node == nullptr) return 0;
    
    int leftHeight = getHeight(node->getLeft());
    int rightHeight = getHeight(node->getRight());
    
    return 1 + max(leftHeight, rightHeight);
}

int BinarySearchTree::getHeight(){
    return getHeight(m_Root);
}

int BinarySearchTree::getNodeCount(Node* node){
    if(node == nullptr) return 0;
    return 1 + getNodeCount(node->getLeft()) + getNodeCount(node->getRight());
}

int BinarySearchTree::getNodeCount(){
    return getNodeCount(m_Root);
}

bool BinarySearchTree::isEmpty(){
    return m_Root == nullptr;
}