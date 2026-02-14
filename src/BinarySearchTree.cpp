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
Node* BinarySearchTree::rotateRight(Node* y){
Node* x=y->getLeft();
Node *newleft=x->getRight();
y->setLeft(newleft);
x->setRight(y);
y->setheight(1+max(getHeight(y->getLeft()),getHeight(y->getRight())));
x->setheight(1+max(getHeight(x->getLeft()),getHeight(x->getRight())));
return x;
}
Node* BinarySearchTree::rotateLeft(Node* y){
Node* x=y->getRight();
Node *newRight=x->getLeft();
y->setRight(newRight);
x->setLeft(y);
y->setheight(1+max(getHeight(y->getLeft()),getHeight(y->getRight())));
x->setheight(1+max(getHeight(x->getLeft()),getHeight(x->getRight())));
return x;
}

int BinarySearchTree::getBalance(Node* y){
    if(y == nullptr) return 0;
    return getHeight(y->getLeft()) - getHeight(y->getRight());
}


Node* BinarySearchTree::insert(Node* node, Root r){
    if(node == nullptr) return new Node(r);
    if(r.getRoot() < node->getData())
        node->setLeft(insert(node->getLeft(), r));
    else if(r.getRoot() > node->getData())
        node->setRight(insert(node->getRight(), r));
    else return node;
    node->setheight(1+max(getHeight(node->getLeft()),getHeight(node->getRight())));
    int balance=getBalance(node);
  if (balance>1&&r.getRoot()<node->getLeft()->getData())
        return rotateRight(node);
    if (balance>1&& r.getRoot()>node->getLeft()->getData()) {
        node->setLeft(rotateLeft(node->getLeft()));
        return rotateRight(node);
    }
    if (balance<-1 &&r.getRoot()>node->getRight()->getData())
        return rotateLeft(node);
    if (balance<-1 &&r.getRoot()<node->getRight()->getData()) {  
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }
return node;
   
}
Node * BinarySearchTree::foundMin(Node* y){
    while(y->getLeft()!=NULL)
    y=y->getLeft();
    return y;
}
Node* BinarySearchTree::deleteN(Node* node, Root r){
   if(node == nullptr) return NULL;
    if(r.getRoot() < node->getData())
        node->setLeft(deleteN(node->getLeft(), r));
    else if(r.getRoot() > node->getData())
        node->setRight(deleteN(node->getRight(), r));
    else {
      if(node->getLeft() == NULL && node->getRight() == NULL){
            delete node;
            return NULL;
        }
        else if(node->getLeft() == NULL){
            Node* temp = node->getRight();
            delete node;
            return temp;
        }
        else if(node->getRight() == NULL){
            Node* temp = node->getLeft();
            delete node;
            return temp;
        }
      else{
        Node* succ=foundMin(node->getRight());
        node->setData(succ->getData());
        node->setRight(deleteN(node->getRight(),succ->getData()));
      }}
  node->setheight(1+max(getHeight(node->getLeft()),getHeight(node->getRight())));
    int balance=getBalance(node);
  if(balance > 1 && getBalance(node->getLeft()) >= 0)
        return rotateRight(node);
    if(balance > 1 && getBalance(node->getLeft()) < 0){
        node->setLeft(rotateLeft(node->getLeft()));
    return rotateRight(node);
    }
    if(balance < -1 && getBalance(node->getRight()) <= 0)
        return rotateLeft(node);
    if(balance < -1 && getBalance(node->getRight()) > 0){
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }
    
   
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
void BinarySearchTree::deleteN(Root r){
    m_Root=deleteN(m_Root,r);
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

void BinarySearchTree::printNodeBox(Node* node, const string& prefix, bool isLeft, bool isRight){
    if(node == nullptr) return;

    Root& r     = node->getRootObject();
    auto derivs = r.getDerivativesList();

    bool hasLeft  = node->getLeft()  != nullptr;
    bool hasRight = node->getRight() != nullptr;

    string branchLine = isLeft  ? "├── [يسار] "
                      : isRight ? "└── [يمين] "
                      :           "";

    cout << prefix << branchLine << endl;

    string boxPrefix = prefix + (isRight ? "    " : "│   ");
    string childPfx  = prefix + (isRight ? "    " : "│   ");

    size_t w = r.getRoot().size() + 4;
    for(auto& d : derivs){
        size_t cand = d.size() + 12;
        if(cand > w) w = cand;
    }
    if(w < 18) w = 18;

    string bar(w, '=');
    string thin(w, '-');

    cout << boxPrefix << "╔" << bar << "╗" << endl;
    cout << boxPrefix << "║  " << r.getRoot()
         << string(w > r.getRoot().size()+2 ? w - r.getRoot().size() - 2 : 0, ' ')
         << "  ║" << endl;

    if(!derivs.empty()){
        cout << boxPrefix << "╠" << thin << "╣" << endl;
        for(int i = 0; i < (int)derivs.size(); i++){
            bool last    = (i == (int)derivs.size() - 1);
            string br    = last ? "  └─ " : "  ├─ ";
            string fq    = " (×" + to_string(r.getFrequency(derivs[i])) + ")";
            string content = br + derivs[i] + fq;
            int pad = (int)w - (int)derivs[i].size() - (int)fq.size() - 5;
            cout << boxPrefix << "║" << content
                 << string(pad > 0 ? pad : 0, ' ') << "║" << endl;
        }
    }
    cout << boxPrefix << "╚" << bar << "╝" << endl;

    if(hasLeft)  printNodeBox(node->getLeft(),  childPfx, true,  false);
    if(hasRight) printNodeBox(node->getRight(), childPfx, false, true);
}

void BinarySearchTree::displayStructured(){
    if(m_Root == nullptr){
        cout << "\n  ╔══════════════════════════╗" << endl;
        cout <<   "  ║   ∅  الشجرة فارغة        ║" << endl;
        cout <<   "  ╚══════════════════════════╝\n" << endl;
        return;
    }

    int total  = getNodeCount();
    int height = getHeight();

    cout << "\n  ╔══════════════════════════════════╗" << endl;
    cout <<   "  ║     شجرة البحث الثنائي           ║" << endl;
    cout <<   "  ║  عدد العقد: " << total
         << "   |   الارتفاع: " << height
         << string(20 - to_string(total).size() - to_string(height).size(), ' ')
         << "║" << endl;
    cout <<   "  ╚══════════════════════════════════╝" << endl;

    cout << "\n         [الجذر الرئيسي]" << endl;
    cout <<   "               │" << endl;

    Root& r     = m_Root->getRootObject();
    auto derivs = r.getDerivativesList();

    size_t w = r.getRoot().size() + 4;
    for(auto& d : derivs){ size_t c = d.size()+12; if(c>w) w=c; }
    if(w < 18) w = 18;

    string bar(w, '=');
    string thin(w, '-');

    cout << "  ╔" << bar << "╗" << endl;
    cout << "  ║  " << r.getRoot()
         << string(w > r.getRoot().size()+2 ? w - r.getRoot().size() - 2 : 0, ' ')
         << "  ║" << endl;

    if(!derivs.empty()){
        cout << "  ╠" << thin << "╣" << endl;
        for(int i = 0; i < (int)derivs.size(); i++){
            bool last    = (i == (int)derivs.size()-1);
            string br    = last ? "  └─ " : "  ├─ ";
            string fq    = " (×" + to_string(r.getFrequency(derivs[i])) + ")";
            string content = br + derivs[i] + fq;
            int pad = (int)w - (int)derivs[i].size() - (int)fq.size() - 5;
            cout << "  ║" << content << string(pad>0?pad:0,' ') << "║" << endl;
        }
    }
    cout << "  ╚" << bar << "╝" << endl;

    if(m_Root->getLeft())  printNodeBox(m_Root->getLeft(),  "│   ", true,  false);
    if(m_Root->getRight()) printNodeBox(m_Root->getRight(), "│   ", false, true);

    cout << endl;
}

void BinarySearchTree::displayRootWithDerivatives(string rootName){
    Node* node = getRootNode(rootName);

    if(node == nullptr){
        cout << "\n  ╔══════════════════════════╗" << endl;
        cout <<   "  ║  الجذر غير موجود: " << rootName
             << string(6 > rootName.size() ? 6-rootName.size() : 0, ' ') << "  ║" << endl;
        cout <<   "  ╚══════════════════════════╝\n" << endl;
        return;
    }

    Root& r     = node->getRootObject();
    auto derivs = r.getDerivativesList();

    size_t w = max(rootName.size(), (size_t)24);
    for(auto& d : derivs){ size_t c = d.size()+12; if(c>w) w=c; }

    string bar(w, '=');
    string thin(w, '-');

    cout << "\n  ╔" << bar << "╗" << endl;
    cout <<   "  ║  " << rootName
         << string(w > rootName.size()+2 ? w-rootName.size()-2 : 0,' ')
         << "  ║" << endl;
    cout <<   "  ╠" << thin << "╣" << endl;
    cout <<   "  ║  عدد المشتقات: " << derivs.size()
         << string(w > 18 ? w-18 : 0,' ') << "  ║" << endl;
    cout <<   "  ╠" << thin << "╣" << endl;

    if(derivs.empty()){
        cout << "  ║  -- لا توجد مشتقات"
             << string(w > 20 ? w-20 : 0, ' ') << "  ║" << endl;
    } else {
        for(int i = 0; i < (int)derivs.size(); i++){
            bool last    = (i == (int)derivs.size()-1);
            string br    = last ? "  └─ " : "  ├─ ";
            string fq    = " (×" + to_string(r.getFrequency(derivs[i])) + ")";
            string content = br + derivs[i] + fq;
            int pad = (int)w - (int)derivs[i].size() - (int)fq.size() - 5;
            cout << "  ║" << content << string(pad>0?pad:0,' ') << "║" << endl;
        }
    }
    cout << "  ╚" << bar << "╝\n" << endl;
}

bool BinarySearchTree::loadRootsFromFile(const string& filename){
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "خطأ في فتح الملف: " << filename << endl;
        return false;
    }

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
    cout << "تم تحميل " << count << " جذر" << endl;
    return true;
}

vector<Root> BinarySearchTree::getAllRoots(){
    vector<Root> roots;
    inorder(m_Root, roots);
    return roots;
}

int BinarySearchTree::getHeight(Node* node){
    if(node == nullptr) return 0;
    return node->getHeight(); 
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