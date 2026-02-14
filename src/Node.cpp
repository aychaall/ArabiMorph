
using namespace std;
#include"../include/Node.h"

Node::Node(){
};
Node::Node(Root r){
   data=r;
    left=NULL;
    right=NULL;
    height=1;
};
Node::Node(Root r,Node* l,Node* ri){
      data=r;
    left=l;
    right=ri;
    height=1;
};
string Node::getData(){
    return data.getRoot();
};
Node* Node::getLeft() { return left; };
Node* Node::getRight() { return right; };
int Node::getHeight() { return height; };
void Node::setLeft(Node* l) { left = l; };
void Node::setRight(Node* r) { right = r; };
void Node::setheight(int x) { height= x; };
Root& Node::getRootObject() {
    return data;
}
void Node::setData(Root r) {
    data = r;
}

void Node::display() {
    cout << "الجذر (Root): " << data.getRoot() <<" ";
    cout<<height<<endl;
    data.displayDerivatives();
    cout << "----------------" << endl;

    if(left != nullptr) left->display();
    if(right != nullptr) right->display();
}
