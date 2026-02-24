#include "../include/hashtable.h"
#include <bits/stdc++.h>
#include <fstream>
using namespace std;



void setnode(struct node* node, string key, vector<char> abst, string algo) {
    node->key        = key;
    node->value.abst = abst;
    node->value.algo = algo;
    node->next       = NULL;
}

void set_hashmap(struct hashmap* hashmap_ptr, long long max_element) {
    hashmap_ptr->max_element = max_element;
    hashmap_ptr->num_element = 0;
    hashmap_ptr->v.assign(max_element, nullptr);
}


int hash_function(struct hashmap* hashmap_ptr, string key) {
    unsigned long long sum = 0;
    for (int i = 0; i < (int)key.length(); i++)
        sum += (unsigned char)key[i];
    return (int)(sum % hashmap_ptr->max_element);
}

vector<char> abst_function(string key){
    vector<char>v;
    for(int i=0;i< (int)key.length();i++){
        v.push_back(key[i]);
    }
    return v;
}

// Replaces ف (FA), ع (AIN), ل (LAM) with root[0], root[1], root[2] tokens.
string algo_function(string key) {
    const string FA  = "\xd9\x81";   // ف
    const string AIN = "\xd8\xb9";   // ع
    const string LAM = "\xd9\x84";   // ل

    string ch = "";
    int rootLetterIdx = 0;

    for (int i = 0; i < (int)key.length()-1; i += 2) {       
            string twoBytes = key.substr(i, 2);// arabic caracters are written in two bytes
            if (twoBytes == FA || twoBytes == AIN || twoBytes == LAM) {
                ch += "root[" + to_string(rootLetterIdx) + "]";
                rootLetterIdx++;
            } else {
                ch += twoBytes;
            }
       
      
    
    }
    return ch;
}


void insert(struct hashmap* hashmap_ptr, string key) {
    if (hashmap_ptr->num_element >= hashmap_ptr->max_element) {
        cout << "Database is full." << endl;
        return;
    }

    int hash_value = hash_function(hashmap_ptr, key);

    struct node* new_node = new struct node();
    setnode(new_node, key, abst_function(key), algo_function(key));

    if (hashmap_ptr->v[hash_value] == NULL) {
        hashmap_ptr->v[hash_value] = new_node;
    } else {
     
        struct node* current_node = hashmap_ptr->v[hash_value];
        while (current_node->next != NULL)
            current_node = current_node->next;
        current_node->next = new_node;
    }

    hashmap_ptr->num_element++;
}

int search(string key, struct hashmap* hashmap_ptr) {
    int hash_value = hash_function(hashmap_ptr, key);
    struct node* current_node = hashmap_ptr->v[hash_value];
    while (current_node != NULL) {
        if (current_node->key == key)
            return hash_value;
        current_node = current_node->next;
    }
    return -1;
}

void del (string key,struct hashmap* hashmap_ptr)
{
    int result = search(key,hashmap_ptr);
    if(result == -1){
        cout<<"impossible !"<<endl;
        return;
    }
    if(hashmap_ptr->v[result]->key== key){
        if(hashmap_ptr->v[result]->next != NULL){
            struct node* temp = hashmap_ptr->v[result]->next;
            delete hashmap_ptr->v[result];
            hashmap_ptr->v[result]=temp;
            hashmap_ptr->num_element--;
        }
        else {
             delete hashmap_ptr->v[result];
             hashmap_ptr->v[result]=NULL;
        }
       
    }
    else {
        struct node* current_node = hashmap_ptr->v[result];
        struct node* prev_node = NULL;                          
            while(current_node->next!= NULL){
                prev_node = current_node;                       
                current_node = current_node->next;
                if(current_node->key == key){
                    if(current_node->next== NULL){
                        delete current_node;
                        current_node = NULL;
                        prev_node->next = NULL;                 
                    }
                    else{
                        struct node* temp = current_node->next;
                        delete current_node;
                        current_node =temp;
                        prev_node->next = temp;
                    }
            }}
    }
}
void update(string oldKey, string newKey, struct hashmap* hashmap_ptr) {
    
    if (search(oldKey, hashmap_ptr) == -1) {
        cout << "✗ Scheme \"" << oldKey << "\" not found — nothing to update." << endl;
        return;
    }

    if (search(newKey, hashmap_ptr) != -1) {
        cout << "✗ Scheme \"" << newKey << "\" already exists — choose a different name." << endl;
        return;
    }
    del(oldKey, hashmap_ptr);
    insert(hashmap_ptr, newKey);

    cout << "✓ Scheme updated: \"" << oldKey << "\" → \"" << newKey << "\"" << endl;
}

void printAll(struct hashmap* hashmap_ptr) {
    if (hashmap_ptr->num_element==0){ 
         cout << "The database is empty." << endl;
         return;}
    for (int i = 0; i < (int)hashmap_ptr->v.size(); i++) {
        struct node* current_node = hashmap_ptr->v[i];
        while (current_node != NULL) {
            cout << "  [" << i << "] key = " << current_node->key
                 << "  |  algo = " << current_node->value.algo << endl;
            current_node = current_node->next;
         
        }
    }
}

void loadFromFile(struct hashmap* hashmap_ptr) {
    string filename;
    cout << "Enter the file path : ";
    getline(cin, filename);

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open file \"" << filename << "\"." << endl;
        return;
    }

    int count = 0;
    string scheme;
    while (getline(file, scheme)) {
        if (!scheme.empty() && scheme.back() == '\r')
            scheme.pop_back();

        if (scheme.empty()) continue;

        if (hashmap_ptr->num_element == hashmap_ptr->max_element) {
            cout << "Database is full. Stopped after inserting " << count << " schemes." << endl;
            file.close();
            return;
        }
        insert(hashmap_ptr, scheme);
        count++;
    }
    file.close();
    cout << count << " schemes loaded successfully." << endl;
}