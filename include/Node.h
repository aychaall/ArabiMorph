

using namespace std;
#include "./Root.h"

class Node {
   Root data;
   Node * left;
   Node * right;

public:
 Node();
 Node(Root r);
 Node(Root r,Node* l,Node* ri);
 string getData();
Root& getRootObject();
 void display();    
 Node* getLeft();
    Node* getRight();
    void setLeft(Node* l);
    void setRight(Node* r);
    void setData(Root r);
};