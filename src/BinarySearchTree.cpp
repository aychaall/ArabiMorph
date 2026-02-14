#include "../include/BinarySearchTree.h"
#include <iostream>
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

void BinarySearchTree::displayStructured(){
    if(m_Root != nullptr) {
        m_Root->display();
    } else {
        cout << "الشجرة فارغة (Tree is empty)" << endl;
    }
}

void BinarySearchTree::displayRootWithDerivatives(string rootName){
    Node* node = getRootNode(rootName);
    if(node != nullptr) {
        cout << "الجذر (Root): " << rootName << endl;
        cout << "المشتقات (Derivatives):" << endl;
        node->display();
    } else {
        cout << "الجذر غير موجود (Root not found)" << endl;
    }
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

