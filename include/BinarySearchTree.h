#pragma once
#include "./Node.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class BinarySearchTree {
    Node* m_Root;

    Node* insert(Node* node, Root r);
    Node* deleteN(Node* node, Root r);
    Node* search(Node* node, string value);
    void inorder(Node* node);
    void inorder(Node* node, vector<Root>& roots);
    void destroy(Node* node);
    int getHeight(Node* node);
    int getNodeCount(Node* node);
    void printNodeBox(Node* node, const string& prefix, bool isLeft, bool isRight);
    int getBalance(Node* y);
    Node* foundMin(Node* y);

public:
    BinarySearchTree();
    ~BinarySearchTree();

    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* y);
    void insert(Root r);
    void deleteN(Root r);
    bool search(string value);
    Node* getRootNode(string value);
    void display();

    int getHeight();
    int getNodeCount();
    bool isEmpty();

    void displayStructured();
    void displayRootWithDerivatives(string rootName);

    bool loadRootsFromFile(const string& filename);
    vector<Root> getAllRoots();


    Node* getRoot() const { return m_Root; }
};
