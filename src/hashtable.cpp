#include "../include/hashtable.h"
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

// ── Initialisation ────────────────────────────────────────────────────────────

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

// ── Core helpers ──────────────────────────────────────────────────────────────

// FIX: cast to (unsigned char) so Arabic UTF-8 bytes are never negative
int hash_function(struct hashmap* hashmap_ptr, string key) {
    unsigned long long sum = 0;
    for (int i = 0; i < (int)key.length(); i++)
        sum += (unsigned char)key[i];
    return (int)(sum % hashmap_ptr->max_element);
}

vector<char> abst_function(string key) {
    vector<char> v;
    for (char c : key)
        v.push_back(c);
    return v;
}

// Replaces ف (FA), ع (AIN), ل (LAM) with root[0], root[1], root[2] tokens.
string algo_function(string key) {
    const string FA  = "\xd9\x81";   // ف
    const string AIN = "\xd8\xb9";   // ع
    const string LAM = "\xd9\x84";   // ل

    string ch = "";
    int rootLetterIdx = 0;
    int i = 0;
    while (i < (int)key.length()) {
        // Multi-byte UTF-8 lead byte
        if ((unsigned char)key[i] >= 0xC0 && i + 1 < (int)key.length()) {
            string twoBytes = key.substr(i, 2);
            if (twoBytes == FA || twoBytes == AIN || twoBytes == LAM) {
                ch += "root[" + to_string(rootLetterIdx) + "]";
                rootLetterIdx++;
            } else {
                ch += twoBytes;
            }
            i += 2;
        } else {
            ch += key[i];
            i++;
        }
    }
    return ch;
}

// ── CRUD ──────────────────────────────────────────────────────────────────────

// FIX: replaced malloc/free with new/delete throughout.
//      malloc skips constructors for std::string and std::vector, which causes
//      undefined behaviour (segfault) when those members are later assigned.
void insert(struct hashmap* hashmap_ptr, string key) {
    if (hashmap_ptr->num_element >= hashmap_ptr->max_element) {
        cout << "Database is full." << endl;
        return;
    }

    int hash_value = hash_function(hashmap_ptr, key);

    // Use 'new' so std::string / std::vector constructors run properly
    struct node* new_node = new struct node();
    setnode(new_node, key, abst_function(key), algo_function(key));

    if (hashmap_ptr->v[hash_value] == NULL) {
        hashmap_ptr->v[hash_value] = new_node;
    } else {
        // Chain at the end of the bucket
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

void del(string key, struct hashmap* hashmap_ptr) {
    int result = search(key, hashmap_ptr);
    if (result == -1) {
        cout << "Scheme not found — impossible to delete." << endl;
        return;
    }

    struct node* current_node = hashmap_ptr->v[result];
    struct node* prev_node    = NULL;

    while (current_node != NULL && current_node->key != key) {
        prev_node    = current_node;
        current_node = current_node->next;
    }

    if (current_node == NULL) return;

    if (prev_node == NULL)
        hashmap_ptr->v[result] = current_node->next;
    else
        prev_node->next = current_node->next;

    // Use 'delete' to match 'new' and properly destroy std::string / std::vector
    delete current_node;
    hashmap_ptr->num_element--;
         cout << "✓ Scheme deleted successfully: \"" << endl;
}

void update(string oldKey, string newKey, struct hashmap* hashmap_ptr) {
    // 1. Make sure old scheme exists
    int oldPos = search(oldKey, hashmap_ptr);
    if (oldPos == -1) {
        cout << "✗ Scheme \"" << oldKey << "\" not found — nothing to update." << endl;
        return;
    }

    // 2. Make sure new key is not already taken (avoid duplicates)
    if (search(newKey, hashmap_ptr) != -1) {
        cout << "✗ Scheme \"" << newKey << "\" already exists — choose a different name." << endl;
        return;
    }

    int newPos = hash_function(hashmap_ptr, newKey);

    if (oldPos == newPos) {
        // ── Same bucket: just update the node in-place ──────────────────────
        struct node* cur = hashmap_ptr->v[oldPos];
        while (cur && cur->key != oldKey) cur = cur->next;
        if (!cur) return;   // should never happen

        cur->key        = newKey;
        cur->value.abst = abst_function(newKey);
        cur->value.algo = algo_function(newKey);

        cout << "✓ Scheme updated: \"" << oldKey << "\" → \"" << newKey << "\"" << endl;
        cout << "  New algo: " << cur->value.algo << endl;

    } else {
        // ── Different bucket: detach from old bucket, attach to new one ──────

        // Detach node from oldPos bucket
        struct node* cur  = hashmap_ptr->v[oldPos];
        struct node* prev = nullptr;
        while (cur && cur->key != oldKey) { prev = cur; cur = cur->next; }
        if (!cur) return;

        if (prev == nullptr)
            hashmap_ptr->v[oldPos] = cur->next;
        else
            prev->next = cur->next;
        cur->next = nullptr;   // isolate node

        // Update its contents
        cur->key        = newKey;
        cur->value.abst = abst_function(newKey);
        cur->value.algo = algo_function(newKey);

        // Attach at end of newPos bucket
        if (hashmap_ptr->v[newPos] == nullptr) {
            hashmap_ptr->v[newPos] = cur;
        } else {
            struct node* tail = hashmap_ptr->v[newPos];
            while (tail->next) tail = tail->next;
            tail->next = cur;
        }

        cout << "✓ Scheme updated: \"" << oldKey << "\" → \"" << newKey << "\"" << endl;
        cout << "  Moved from bucket " << oldPos << " to bucket " << newPos << endl;
        cout << "  New algo: " << cur->value.algo << endl;
    }
}
// ── Display & file I/O (moved from main.cpp) ─────────────────────────────────

void printAll(struct hashmap* hashmap_ptr) {
    bool found = false;
    for (int i = 0; i < (int)hashmap_ptr->v.size(); i++) {
        struct node* current_node = hashmap_ptr->v[i];
        while (current_node != NULL) {
            cout << "  [" << i << "] key = " << current_node->key
                 << "  |  algo = " << current_node->value.algo << endl;
            current_node = current_node->next;
            found = true;
        }
    }
    if (!found)
        cout << "The database is empty." << endl;
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
        // Strip Windows-style carriage return if present
        if (!scheme.empty() && scheme.back() == '\r')
            scheme.pop_back();

        if (scheme.empty()) continue;

        if (hashmap_ptr->num_element >= hashmap_ptr->max_element) {
            cout << "Database is full. Stopped after inserting " << count << " scheme(s)." << endl;
            file.close();
            return;
        }
        insert(hashmap_ptr, scheme);
        count++;
    }
    file.close();
    cout << count << " scheme(s) loaded successfully." << endl;
}