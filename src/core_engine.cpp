#include "../include/core_engine.h"
#include <string>
#include <iostream>
#include <cctype>
using namespace std;

// ── apply_root ────────────────────────────────────────────────────────────────
// Replaces every root[X] token in the algo string with the X-th Arabic
// character (2 UTF-8 bytes each) from root, returning the built word.

string apply_root(const string& algo, const string& root) {
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

// ── generate ──────────────────────────────────────────────────────────────────
// Asks the user to pick schemes one at a time for the given root.
// For each scheme:
//   1. Looks it up in the hash table.
//   2. Applies it to the root to produce a word.
//   3. Stores the word as a validated derivative in the AVL tree node.
// Stops when the user types "done".

void generate(string root, struct hashmap* hashmap_ptr, BinarySearchTree* tree) {
    // Locate the root node in the AVL tree
    Node* rootNode = tree->getRootNode(root);
    if (!rootNode) {
        cout << "✗ Root \"" << root << "\" not found in AVL tree." << endl;
        cout << "  Insert it first (option 1)." << endl;
        return;
    }

    cout << "Enter schemes one at a time. Type 'done' to stop." << endl;

    while (true) {
        cout << "Enter scheme (or 'done'): ";
        string scheme;
        getline(cin, scheme);

        if (scheme == "done" || scheme == "Done" || scheme == "DONE")
            break;
        if (scheme.empty())
            continue;

        // Search scheme in hash table
        int pos = search(scheme, hashmap_ptr);
        if (pos == -1) {
            cout << "✗ Scheme \"" << scheme << "\" not found in hash table." << endl;
            continue;
        }

        // Walk chain to exact matching node
        struct node* current_node = hashmap_ptr->v[pos];
        while (current_node != NULL && current_node->key != scheme)
            current_node = current_node->next;

        if (current_node == NULL) {
            cout << "✗ Scheme \"" << scheme << "\" not found." << endl;
            continue;
        }

        // Build the word
        string word = apply_root(current_node->value.algo, root);

        cout << "  Root   : " << root   << endl;
        cout << "  Scheme : " << scheme << endl;
        cout << "  Word   : " << word   << endl;

        // Store in AVL tree node — this is the link between the two structures
        rootNode->getRootObject().addderviation(word);
        cout << "✓ \"" << word << "\" stored as derivative of \""
             << root << "\" in the AVL tree." << endl;
    }
}

// ── validate ──────────────────────────────────────────────────────────────────
// Checks whether 'word' morphologically belongs to 'root' by trying every
// scheme in the hash table.
// If a match is found:
//   - Prints OUI + the matching scheme.
//   - Stores the word as a validated derivative in the AVL tree node.
// If no scheme matches, prints NON.

void validate(string word, string root, struct hashmap* hashmap_ptr, BinarySearchTree* tree) {
    for (int i = 0; i < (int)hashmap_ptr->v.size(); i++) {
        struct node* current_node = hashmap_ptr->v[i];
        while (current_node != NULL) {
            string generated = apply_root(current_node->value.algo, root);
            if (generated == word) {
                cout << "OUI — \"" << word << "\" matches scheme \""
                     << current_node->key << "\" with root \"" << root << "\"" << endl;

                // Store in AVL tree node — link between the two structures
                Node* rootNode = tree->getRootNode(root);
                if (rootNode) {
                    rootNode->getRootObject().addderviation(word);
                    cout << "✓ \"" << word << "\" stored as derivative of \""
                         << root << "\" in the AVL tree." << endl;
                } else {
                    cout << "  ℹ Root \"" << root
                         << "\" not in AVL tree — insert it (option 1) to track derivatives."
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
    // Step 1: verify scheme exists and get its algo
    int pos = search(scheme, hashmap_ptr);
    if (pos == -1) {
        cout << "✗ Scheme \"" << scheme << "\" not found in hash table." << endl;
        return;
    }
    struct node* sn = hashmap_ptr->v[pos];
    while (sn && sn->key != scheme) sn = sn->next;
    if (!sn) { cout << "✗ Scheme not found." << endl; return; }

    string algo = sn->value.algo;

    // Step 2: walk every root in the AVL tree (in-order)
    vector<Root> allRoots = tree->getAllRoots();

    cout << "\n╔══════════════════════════════════════════════╗" << endl;
    cout << "║  Morphological Family  /  العائلة الصرفية   ║" << endl;
    cout << "║  Scheme: " << scheme << endl;
    cout << "╚══════════════════════════════════════════════╝" << endl;

    bool anyFound = false;
    for (Root& r : allRoots) {
        // Generate the expected word for this root + scheme
        string expectedWord = apply_root(algo, r.getRoot());

        // Check if this word is among the root's validated derivatives
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
