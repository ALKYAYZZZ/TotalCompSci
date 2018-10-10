#include <cstddef> // to define NULL
class BinaryNode {
private:
friend class BinaryTree;
int element;
BinaryNode *left, *right;
public:
//constructor
BinaryNode(int el=0, BinaryNode *lt=NULL,
BinaryNode *rt=NULL) :
element(el), left(lt), right(rt) { }
// member functions
BinaryNode *getLeft() { return left; }
BinaryNode *getRight() { return right; }
int getElem() { return element; }
int size(BinaryNode *t);
int height(BinaryNode *t);
BinaryNode *copy();
};

int BinaryNode::size(BinaryNode *t) { //recursive
if (t == NULL)
return 0;
else
return 1 + size(t->left) + size(t->right);
}
int BinaryNode::height(BinaryNode *t) { //
if (t == NULL)
return -1;
else {
int hlf = height(t->left);
int hrt = height(t->right);
return (hlf > hrt) ? 1+hlf : 1+hrt;
}
}
BinaryNode *BinaryNode::copy( ) { // recursive
BinaryNode *root = new BinaryNode(element);
if (left != NULL)
root->left = left->copy();
if (right != NULL)
root->right = right->copy();
return root;
}