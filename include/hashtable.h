#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <bits/stdc++.h>
using namespace std;
struct inside{
    vector<char> abst;
    string algo;
};

struct node {
    string key;
    struct inside value;
    node* next;

};

struct hashmap {
    long long max_element;
    long long num_element;
    vector<struct node*> v;

};

void setnode(struct node* node,string key,vector<char> abst,string algo);
void set_hashmap(struct hashmap* hashmap_ptr,long long max_element);
int hash_function(struct hashmap* hashmap_ptr, string key);
void insert(struct hashmap* hashmap_ptr, string key);
int search(string key,struct hashmap* hashmap_ptr);
void del (string key,struct hashmap* hashmap_ptr);
vector<char> abst_function(string key);
string algo_function(string key);
void printAll(struct hashmap* hashmap_ptr);
void loadFromFile(struct hashmap* hashmap_ptr);
void update(string oldKey, string newKey, struct hashmap* hashmap_ptr);
#endif