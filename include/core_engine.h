#ifndef core_engine_H
#define core_engine_H
#include <bits/stdc++.h>
#include "hashtable.h"
#include "BinarySearchTree.h"
using namespace std;
void generate(string root, struct hashmap* hashmap_ptr, BinarySearchTree* tree);

void validate(string word, string root, struct hashmap* hashmap_ptr, BinarySearchTree* tree);

string apply_root(const string& algo, const string& root);
void displayMorphologicalFamily(string scheme, struct hashmap* hashmap_ptr, BinarySearchTree* tree) ;

#endif
