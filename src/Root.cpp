#include "../include/Root.h"

using namespace std;
 
Root::Root(){}
Root::Root(string s){
    rootname=s;
}
 string Root::getRoot(){
    return rootname;
 }
unordered_map<string, int> Root::getDerivatives() {
    return derive;
}

int Root::getDerivativeCount() {
    return derive.size();
}
int Root::getFrequency(string derivative) {
    if(derive[derivative]) {
        return derive[derivative];
    }
    return 0;
}
vector<string> Root::getDerivativesList() {
    vector<string> list;
    for(auto pair : derive) {
        list.push_back(pair.first);
    }
    return list;
}
 void Root::addderviation(string s){ 
    derive[s]++;
 }

void Root::displayDerivatives() {
    if(derive.empty()) {
        cout << "  لا توجد مشتقات (No derivatives)" << endl;
        return;
    }
    
    for(auto& a : derive) {
        cout << "  - " << a.first << " (تكرار/frequency: " << a.second << ")" << endl;
    }
}

void Root::display() {
    cout << "الجذر (Root): " << rootname << endl;
    cout << "عدد المشتقات (Derivative count): " << getDerivativeCount() << endl;
    if(!derive.empty()) {
        cout << "المشتقات (Derivatives):" << endl;
        displayDerivatives();
    }

}
