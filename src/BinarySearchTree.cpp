#include "../include/BinarySearchTree.h"
#include <algorithm>
#include <iostream>
using namespace std;

BinarySearchTree::BinarySearchTree() : m_Root(nullptr) {}

BinarySearchTree::~BinarySearchTree() { destroy(m_Root); }

void BinarySearchTree::destroy(Node* node) {
    if (!node) return;
    destroy(node->getLeft());
    destroy(node->getRight());
    delete node;
}

Node* BinarySearchTree::rotateRight(Node* y) {
    Node* x = y->getLeft();
    Node* newleft = x->getRight();
    y->setLeft(newleft);
    x->setRight(y);
    y->setheight(1 + max(getHeight(y->getLeft()), getHeight(y->getRight())));
    x->setheight(1 + max(getHeight(x->getLeft()), getHeight(x->getRight())));
    return x;
}

Node* BinarySearchTree::rotateLeft(Node* y) {
    Node* x = y->getRight();
    Node* newRight = x->getLeft();
    y->setRight(newRight);
    x->setLeft(y);
    y->setheight(1 + max(getHeight(y->getLeft()), getHeight(y->getRight())));
    x->setheight(1 + max(getHeight(x->getLeft()), getHeight(x->getRight())));
    return x;
}

int BinarySearchTree::getBalance(Node* y) {
    if (!y) return 0;
    return getHeight(y->getLeft()) - getHeight(y->getRight());
}

Node* BinarySearchTree::insert(Node* node, Root r) {
    if (!node) return new Node(r);
    if (r.getRoot() < node->getData())
        node->setLeft(insert(node->getLeft(), r));
    else if (r.getRoot() > node->getData())
        node->setRight(insert(node->getRight(), r));
    else return node;

    node->setheight(1 + max(getHeight(node->getLeft()), getHeight(node->getRight())));
    int balance = getBalance(node);

    if (balance > 1 && r.getRoot() < node->getLeft()->getData())
        return rotateRight(node);
    if (balance > 1 && r.getRoot() > node->getLeft()->getData()) {
        node->setLeft(rotateLeft(node->getLeft()));
        return rotateRight(node);
    }
    if (balance < -1 && r.getRoot() > node->getRight()->getData())
        return rotateLeft(node);
    if (balance < -1 && r.getRoot() < node->getRight()->getData()) {
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }
    return node;
}

Node* BinarySearchTree::foundMin(Node* y) {
    while (y->getLeft()) y = y->getLeft();
    return y;
}

Node* BinarySearchTree::deleteN(Node* node, Root r) {
    if (!node) return nullptr;
    if (r.getRoot() < node->getData())
        node->setLeft(deleteN(node->getLeft(), r));
    else if (r.getRoot() > node->getData())
        node->setRight(deleteN(node->getRight(), r));
    else {
        if (!node->getLeft() && !node->getRight()) { delete node; return nullptr; }
        else if (!node->getLeft()) { Node* t = node->getRight(); delete node; return t; }
        else if (!node->getRight()) { Node* t = node->getLeft(); delete node; return t; }
        else {
            Node* succ = foundMin(node->getRight());
            node->setData(succ->getData());
            node->setRight(deleteN(node->getRight(), succ->getData()));
        }
    }
    node->setheight(1 + max(getHeight(node->getLeft()), getHeight(node->getRight())));
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->getLeft()) >= 0) return rotateRight(node);
    if (balance > 1 && getBalance(node->getLeft()) < 0) {
        node->setLeft(rotateLeft(node->getLeft()));
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->getRight()) <= 0) return rotateLeft(node);
    if (balance < -1 && getBalance(node->getRight()) > 0) {
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }
    return node;
}

Node* BinarySearchTree::search(Node* node, string value) {
    if (!node) return nullptr;
    if (node->getData() == value) return node;
    else if (value < node->getData()) return search(node->getLeft(), value);
    else return search(node->getRight(), value);
}

void BinarySearchTree::inorder(Node* node) {
    if (!node) return;
    inorder(node->getLeft());
    cout << node->getData() << " ";
    inorder(node->getRight());
}

void BinarySearchTree::inorder(Node* node, vector<Root>& roots) {
    if (!node) return;
    inorder(node->getLeft(), roots);
    roots.push_back(Root(node->getData()));
    inorder(node->getRight(), roots);
}

void BinarySearchTree::insert(Root r) { m_Root = insert(m_Root, r); }
void BinarySearchTree::deleteN(Root r) { m_Root = deleteN(m_Root, r); }
bool BinarySearchTree::search(string value) { return search(m_Root, value) != nullptr; }
Node* BinarySearchTree::getRootNode(string value) { return search(m_Root, value); }

void BinarySearchTree::display() { inorder(m_Root); cout << endl; }

void BinarySearchTree::printNodeBox(Node*, const string&, bool, bool) {}
void BinarySearchTree::displayStructured() {}
void BinarySearchTree::displayRootWithDerivatives(string) {}

bool BinarySearchTree::loadRootsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    string line;
    while (getline(file, line))
        if (!line.empty()) insert(Root(line));
    file.close();
    return true;
}

vector<Root> BinarySearchTree::getAllRoots() {
    vector<Root> roots;
    inorder(m_Root, roots);
    return roots;
}

int BinarySearchTree::getHeight(Node* node) {
    if (!node) return 0;
    return node->getHeight();
}

int BinarySearchTree::getHeight() { return getHeight(m_Root); }

int BinarySearchTree::getNodeCount(Node* node) {
    if (!node) return 0;
    return 1 + getNodeCount(node->getLeft()) + getNodeCount(node->getRight());
}

int BinarySearchTree::getNodeCount() { return getNodeCount(m_Root); }
bool BinarySearchTree::isEmpty() { return m_Root == nullptr; }
