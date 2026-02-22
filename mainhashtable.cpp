#include "./include/hashtable.h"
#include "./include/core_engine.h"
#include <locale>
using namespace std;

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

void setupArabicConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, ".UTF-8");
#endif
}

void printMenu() {
    cout << "\n========== Arabic Morphology Engine ==========" << endl;
    cout << "1. Insert a scheme"                               << endl;
    cout << "2. Load schemes from file"                        << endl;
    cout << "3. Search for a scheme"                           << endl;
    cout << "4. Delete a scheme"                               << endl;
    cout << "5. Generate a word from root + scheme"            << endl;
    cout << "6. Validate a word against a root"                << endl;
    cout << "7. Print all schemes"                             << endl;
    cout << "8. Exit"                                          << endl;
    cout << "==============================================" << endl;
    cout << "Choose an option : ";
}

int main() {
    setupArabicConsole();

    // FIX: use 'new' instead of malloc so the hashmap's std::vector
    //      is properly constructed before set_hashmap() calls .assign().
    struct hashmap* hashmap_ptr = new struct hashmap();
    set_hashmap(hashmap_ptr, 10000);

    int choice;
    do {
        printMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {

            case 1: {
                string scheme;
                cout << "Enter the scheme : ";
                getline(cin, scheme);
                insert(hashmap_ptr, scheme);
                cout << "Scheme inserted." << endl;
                break;
            }

            case 2: {
                // loadFromFile is now defined in hashtable.cpp
                loadFromFile(hashmap_ptr);
                break;
            }

            case 3: {
                string scheme;
                cout << "Enter the scheme to search : ";
                getline(cin, scheme);
                int pos = search(scheme, hashmap_ptr);
                if (pos == -1)
                    cout << "Scheme not found in the database." << endl;
                else
                    cout << "Scheme found at bucket index " << pos << "." << endl;
                break;
            }

            case 4: {
                string scheme;
                cout << "Enter the scheme to delete : ";
                getline(cin, scheme);
                del(scheme, hashmap_ptr);
                cout << "Done." << endl;
                break;
            }

            case 5: {
                string root;
                cout << "Enter the root : ";
                getline(cin, root);
                generate(root, hashmap_ptr);
                break;
            }

            case 6: {
                string word, root;
                cout << "Enter the word  : ";
                getline(cin, word);
                cout << "Enter the root  : ";
                getline(cin, root);
                validate(word, root, hashmap_ptr);
                break;
            }

            case 7: {
                cout << "\n--- All schemes in the database ---" << endl;
                // printAll is now defined in hashtable.cpp
                printAll(hashmap_ptr);
                break;
            }

            case 8: {
                cout << "Goodbye!" << endl;
                break;
            }

            default: {
                cout << "Invalid option. Please choose between 1 and 8." << endl;
                break;
            }
        }

    } while (choice != 8);

    delete hashmap_ptr;
    return 0;
}