#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif
#include <clocale>
#include "./include/BinarySearchTree.h"

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

void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "   محرك البحث المورفولوجي العربي" << endl;
    cout << "   Arabic Morphological Search Engine" << endl;
    cout << "========================================" << endl;
    cout << "1. إضافة جذر جديد (Add new root)" << endl;
    cout << "2. البحث عن جذر (Search for root)" << endl;
    cout << "3. عرض جميع الجذور (Display all roots)" << endl;
    cout << "4. إضافة مشتق لجذر (Add derivative to root)" << endl;
    cout << "5. عرض مشتقات جذر (Display root derivatives)" << endl;
    cout << "6. تحميل جذور من ملف (Load roots from file)" << endl;
    cout << "7. معلومات الشجرة (Tree statistics)" << endl;
    cout << "0. خروج (Exit)" << endl;
    cout << "========================================" << endl;
    cout << "اختر خيار (Choose option): ";
}

int main() {
    setupArabicConsole();

    BinarySearchTree tree;
    int choice;
    string input, rootName, derivative;

    cout << "مرحبا بك في محرك البحث المورفولوجي العربي" << endl;
    cout << "Welcome to Arabic Morphological Search Engine" << endl;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: {
                cout << "\nأدخل الجذر (Enter root): ";
                getline(cin, input);
                if(!input.empty()) {
                    Root newRoot(input);
                    tree.insert(newRoot);
                    cout << "✓ تم إضافة الجذر بنجاح (Root added successfully)" << endl;
                } else {
                    cout << "✗ الجذر فارغ (Empty root)" << endl;
                }
                break;
            }

            case 2: {
                cout << "\nأدخل الجذر للبحث (Enter root to search): ";
                getline(cin, input);
                if(tree.search(input)) {
                    cout << "✓ الجذر موجود (Root found)" << endl;
                } else {
                    cout << "✗ الجذر غير موجود (Root not found)" << endl;
                }
                break;
            }

            case 3: {
                cout << "\nجميع الجذور (All roots):" << endl;
                cout << "--------------------------------" << endl;
                if(tree.isEmpty()) {
                    cout << "الشجرة فارغة (Tree is empty)" << endl;
                } else {
                    tree.displayStructured();
                }
                break;
            }

            case 4: {
                cout << "\nأدخل الجذر (Enter root): ";
                getline(cin, rootName);
                Node* node = tree.getRootNode(rootName);
                if(node != nullptr) {
                    cout << "أدخل المشتق (Enter derivative): ";
                    getline(cin, derivative);
                    node->getRootObject().addderviation(derivative);
                    cout << "✓ تم إضافة المشتق بنجاح (Derivative added successfully)" << endl;
                } else {
                    cout << "✗ الجذر غير موجود (Root not found)" << endl;
                }
                break;
            }

            case 5: {
                cout << "\nأدخل الجذر (Enter root): ";
                getline(cin, rootName);
                tree.displayRootWithDerivatives(rootName);
                break;
            }

            case 6: {
                cout << "\nأدخل اسم الملف (Enter filename): ";
                getline(cin, input);
                if(tree.loadRootsFromFile(input)) {
                    cout << "✓ تم تحميل الملف بنجاح (File loaded successfully)" << endl;
                } else {
                    cout << "✗ فشل تحميل الملف (Failed to load file)" << endl;
                }
                break;
            }

            case 7: {
                cout << "\nمعلومات الشجرة (Tree Statistics):" << endl;
                cout << "--------------------------------" << endl;
                cout << "عدد الجذور (Number of roots): " << tree.getNodeCount() << endl;
                cout << "ارتفاع الشجرة (Tree height): "  << tree.getHeight() << endl;
                cout << "الشجرة فارغة؟ (Is empty?): "
                     << (tree.isEmpty() ? "نعم (Yes)" : "لا (No)") << endl;
                break;
            }

            case 0:
                cout << "\nشكراً لاستخدامك البرنامج (Thank you for using the program)" << endl;
                break;

            default:
                cout << "\n✗ خيار غير صحيح (Invalid choice)" << endl;
        }

    } while(choice != 0);

    return 0;
}