#pragma once
#include <bits/stdc++.h>
using namespace std;

class Root {
    string rootname;
   unordered_map<string,int>derive;
public:
 Root();
 Root(string s);
string getRoot();
unordered_map<string, int> getDerivatives();
int getDerivativeCount();
int getFrequency(string derivative);
vector<string> getDerivativesList(); 
 void addderviation(string s);
 void displayDerivatives();
void display(); 

};