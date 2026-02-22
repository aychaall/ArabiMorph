/**
 * ============================================================
 *  Arabic Morphological Search Engine — Unified Main
 * ============================================================
 *  AVL Tree  (BinarySearchTree) : indexes Arabic roots
 *  Hash Table (hashmap)         : stores morphological schemes
 *  Core Engine (core_engine.cpp): generate() and validate()
 *    own all derivation logic AND the AVL storage — main just
 *    calls them. No logic is repeated here.
 * ============================================================
 */

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif
#include <clocale>
#include <iostream>
#include <string>

#include "./include/core_engine.h"

using namespace std;

void setupArabicConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, ".UTF-8");
#else
    setlocale(LC_ALL, "en_US.UTF-8");
#endif
}

void printSeparator() {
    cout << "================================================" << endl;
}

void printMainMenu() {
    cout << "\n";
    printSeparator();
    cout << "  Arabic Morphological Engine  /  محرك الصرف العربي" << endl;
    printSeparator();
    cout << "--- ROOT MANAGEMENT (AVL Tree) ---" << endl;
    cout << "  1. Insert a root" << endl;
    cout << "  2. Search for a root" << endl;
    cout << "  3. Delete a root" << endl;
    cout << "  4. Display all roots (in-order)" << endl;
    cout << "  5. Display root + its validated derivatives" << endl;
    cout << "  6. Load roots from file" << endl;
    cout << "  7. Tree statistics" << endl;
    cout << endl;
    cout << "--- SCHEME MANAGEMENT (Hash Table) ---" << endl;
    cout << "  8. Insert a scheme" << endl;
    cout << "  9. Search for a scheme" << endl;
    cout << " 10. Delete a scheme" << endl;
    cout << " 11. Display all schemes" << endl;
    cout << " 12. Load schemes from file" << endl;
    cout << " 17. Update a scheme" << endl;
    cout << endl;
    cout << "--- MORPHOLOGICAL ENGINE ---" << endl;
    cout << " 13. Generate word(s) from root + chosen scheme(s)" << endl;
    cout << " 14. Validate a word against a root" << endl;
     cout << " 15. Display morphological family (all roots for a scheme)" << endl;

    cout << endl;
    cout << "  0. Exit" << endl;
    printSeparator();
    cout << "Choose: ";
}

int main() {
    setupArabicConsole();

    BinarySearchTree tree;

    struct hashmap* hm = new struct hashmap();
    set_hashmap(hm, 10000);

    cout << "\nWelcome to the Arabic Morphological Search Engine" << endl;
    cout << "مرحباً بكم في محرك البحث المورفولوجي العربي\n" << endl;

    int choice;
    string input;

    do {
        printMainMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {

            // ── ROOT MANAGEMENT ──────────────────────────────────────────────

            case 1: {
                cout << "Enter root: ";
                getline(cin, input);
                if (input.empty()) { cout << "✗ Empty input." << endl; break; }
                tree.insert(Root(input));
                cout << "✓ Root \"" << input << "\" inserted." << endl;
                break;
            }

            case 2: {
                cout << "Enter root to search: ";
                getline(cin, input);
                cout << (tree.search(input) ? "✓ Found." : "✗ Not found.") << endl;
                break;
            }

            case 3: {
                cout << "Enter root to delete: ";
                getline(cin, input);
                if (!tree.search(input)) { cout << "✗ Not found." << endl; break; }
                tree.deleteN(Root(input));
                cout << "✓ Root \"" << input << "\" deleted." << endl;
                break;
            }

            case 4: {
                if (tree.isEmpty()) { cout << "Tree is empty." << endl; break; }
                cout << "\nAll roots (in-order):" << endl;
                tree.display();
                break;
            }

            case 5: {
                cout << "Enter root: ";
                getline(cin, input);
                tree.displayRootWithDerivatives(input);
                break;
            }

            case 6: {
                cout << "Enter filename: ";
                getline(cin, input);
                if (tree.loadRootsFromFile(input))
                    cout << "✓ Loaded. Tree has " << tree.getNodeCount() << " root(s)." << endl;
                else
                    cout << "✗ Could not open \"" << input << "\"." << endl;
                break;
            }

            case 7: {
                printSeparator();
                cout << "  Number of roots : " << tree.getNodeCount() << endl;
                cout << "  Tree height     : " << tree.getHeight()    << endl;
                cout << "  Empty?          : " << (tree.isEmpty() ? "Yes" : "No") << endl;
                printSeparator();
                break;
            }

            // ── SCHEME MANAGEMENT ────────────────────────────────────────────

            case 8: {
                cout << "Enter scheme (e.g. فاعل): ";
                getline(cin, input);
                if (input.empty()) { cout << "✗ Empty input." << endl; break; }
                insert(hm, input);
                cout << "✓ Scheme \"" << input << "\" inserted." << endl;
                break;
            }

            case 9: {
                cout << "Enter scheme to search: ";
                getline(cin, input);
                int pos = search(input, hm);
                if (pos != -1) cout << "✓ Found (bucket " << pos << ")." << endl;
                else           cout << "✗ Not found." << endl;
                break;
            }

            case 10: {
                cout << "Enter scheme to delete: ";
                getline(cin, input);
                del(input, hm);
                break;
            }

            case 11: {
                cout << "\n--- All schemes ---" << endl;
                printAll(hm);
                break;
            }

            case 12: {
                loadFromFile(hm);
                break;
            }

            // ── MORPHOLOGICAL ENGINE ─────────────────────────────────────────
            // All logic lives in core_engine.cpp — main only reads root/word
            // from the user and forwards them. AVL storage is done inside the
            // engine functions, not here.

            case 13: {
                cout << "Enter root: ";
                getline(cin, input);
                // generate() validates root in AVL, loops over user-chosen
                // schemes, builds each word, and stores it in the AVL node.
                generate(input, hm, &tree);
                break;
            }

            case 14: {
                cout << "Enter word : ";
                string word;
                getline(cin, word);
                cout << "Enter root : ";
                getline(cin, input);
                // validate() scans all schemes, prints OUI/NON + scheme name,
                // and stores the word in the AVL node if matched.
                validate(word, input, hm, &tree);
                break;
            }  
              case 15: {
                cout << "Enter scheme: ";
                getline(cin, input);
                displayMorphologicalFamily(input, hm, &tree);
                break;
            }
            case 17: {
                cout << "Enter the scheme to update (old name): ";
                getline(cin, input);
                string newScheme;
                cout << "Enter the new scheme name: ";
                getline(cin, newScheme);
                if (newScheme.empty()) { cout << "✗ Empty input." << endl; break; }
                update(input, newScheme, hm);
                break;
            }


            case 0:
                cout << "\nGoodbye! / وداعاً!" << endl;
                break;

            default:
                cout << "✗ Invalid option. Choose 0–14." << endl;
        }

    } while (choice != 0);

    delete hm;
    return 0;
}