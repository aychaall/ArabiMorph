#include "../include/core_engine.h"
#include <string>
#include <iostream>
#include <cctype>
using namespace std;



string apply_algo(const string& algo, const string& root) {
    string result = "";
    int j = 0;
    while (j < (int)algo.length()) {
        if (j + 7 <= (int)algo.length() && algo.substr(j, 5) == "root[") {
            char digitChar = algo[j + 5];
            if (isdigit(digitChar) && algo[j + 6] == ']') {
                int idx        = digitChar - '0';
                int byteOffset = idx * 2;   // each Arabic char = 2 UTF-8 bytes
                if (byteOffset + 1 < (int)root.length()) {
                    result += root[byteOffset];
                    result += root[byteOffset + 1];
                }
                j += 7;   // skip past "root[X]"
                continue;
            }
        }
        result += algo[j];
        j++;
    }
    return result;
}



void generate(string root, struct hashmap* hashmap_ptr, BinarySearchTree* tree) {
    Node* rootNode = tree->getRootNode(root);
    if (!rootNode) {
        cout << "✗ Root \"" << root << "\" not found in AVL tree." << endl;
        cout << "  Insert it first (option 1)." << endl;
        return;
    }

   cout<<"Please define the number of schemes you want to generate with the selected root : "<<endl;
    int n;cin>>n;
    string scheme;
    vector<string>v(n);
    for(int i=0;i<n;i++){
        cout<<"Give the scheme number = "<<i+1<<endl;
        cin>>scheme;
    
       
        int pos = search(scheme, hashmap_ptr);
        if (pos == -1) {
            cout << "✗ Scheme \"" << scheme << "\" not found in hash table." << endl;
            continue;
        }

        struct node* current_node = hashmap_ptr->v[pos];
        while (current_node->key != scheme){
           current_node = current_node->next;
        }
   
        string word = apply_algo(current_node->value.algo, root);

        cout << "  Root   : " << root   << endl;
        cout << "  Scheme : " << scheme << endl;
        cout << "  Word   : " << word   << endl;

        rootNode->getRootObject().addderviation(word);
        cout << "✓ \"" << word << "\" stored as derivative of \""
             << root << "\" in the AVL tree." << endl;
    }
}


void validate(string word, string root, struct hashmap* hashmap_ptr, BinarySearchTree* tree) {
    for (int i = 0; i < (int)hashmap_ptr->v.size(); i++) {
        struct node* current_node = hashmap_ptr->v[i];
        while (current_node != NULL) {
            string generated = apply_algo(current_node->value.algo, root);
            if (generated == word) {
                cout << "OUI — \"" << word << "\" matches scheme \""
                     << current_node->key << "\" with root \"" << root << "\"" << endl;

               
                Node* rootNode = tree->getRootNode(root);
                if (rootNode) {
                    rootNode->getRootObject().addderviation(word);
                    cout << "✓ \"" << word << "\" stored as derivative of \""
                         << root << "\" in the AVL tree." << endl;
                } else {
                    cout << "  ℹ Root \"" << root
                         << "\" not in AVL tree "
                         << endl;
                }
                return;
            }
            current_node = current_node->next;
        }
    }
    cout << "NON — \"" << word << "\" does not match any scheme for root \""
         << root << "\"" << endl;
}
void displayMorphologicalFamily(string scheme, struct hashmap* hashmap_ptr, BinarySearchTree* tree) {

    int pos = search(scheme, hashmap_ptr);
    if (pos == -1) {
        cout << "✗ Scheme \"" << scheme << "\" not found in hash table." << endl;
        return;
    }
    struct node* current_node = hashmap_ptr->v[pos];
    while ( current_node ->key != scheme)  current_node  =  current_node ->next;
   
    string algo =  current_node ->value.algo;

    vector<Root> allRoots = tree->getAllRoots();

    cout << "\n╔══════════════════════════════════════════════╗" << endl;
    cout << "║  Morphological Family  /  العائلة الصرفية   ║" << endl;
    cout << "║  Scheme: " << scheme << endl;
    cout << "╚══════════════════════════════════════════════╝" << endl;

    bool anyFound = false;
    for (Root& r : allRoots) {
        string expectedWord = apply_algo(algo, r.getRoot());

        auto derivs = r.getDerivatives();
        if (derivs.count(expectedWord)) {
            anyFound = true;
            cout << "  Root: " << r.getRoot()
                 << "  →  Word: " << expectedWord
                 << "  (freq: " << derivs[expectedWord] << ")" << endl;
        }
    }
    if (!anyFound)
        cout << "  No validated derivatives found for scheme \"" << scheme << "\"." << endl;
}
