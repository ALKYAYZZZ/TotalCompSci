#include "BinaryNode.h"
class BinaryTree {
private:
BinaryNode *root;
void deleteTree(BinaryNode *root);
public: // class BinaryTree (cont)
BinaryTree( ) { root = NULL; }
BinaryTree(int el) { root = new BinaryNode(el); }
~BinaryTree() { deleteTree(root); root = NULL; }
// functions
BinaryNode *getRoot( ) { return root; }
bool isEmpty( ) { return root == NULL; }
int size( ) { return (root == NULL) ?
0 : root->size(root); }
int height( ) { return (root == NULL) ?
0 : root->height(root); }
void copy(BinaryTree& rhs) {
if (this != &rhs) {
deleteTree(root); // make tree empty
if (rhs.root != NULL)
root = rhs.root->copy();
}
}
void merge(int rootItem, BinaryTree& t1,
BinaryTree& t2);
};

class BinarySearchTree { // uses the class BinaryNode
private:
BinaryNode *root;
// some private functions will be here
int Key,SearchCost;
public:
// constructor
BinarySearchTree( ) { root = NULL; Key = 0; SearchCost = 0;}
~BinarySearchTree( ) { deleteTree(root); root = NULL; Key = 0; SearchCost = 0;}
int size() {return (root==NULL) ? 0 : root->size(root);}
int height() {return (root==NULL) ?
0 : root->height(root);}
void insert(int x) //throw(DuplicateItem)
{ root = insert(x, root); }
void remove(int x) //throw(ItemNotFound)
{ root = remove(x, root); }
BinaryNode *find(int x) //throw(ItemNotFound)
{ return find(x, root); }
bool isEmpty( ){ return root == NULL; }
};


class TreeOperation {
public:
virtual void processNode(int elem) = 0;
};

class PrintTreeNode : public TreeOperation
{
public:
void processNode(int element) {
cout << element << " ";
}
};



// merge t1, t2 and root (with rootItem)
void BinaryTree::merge(int rootItem, BinaryTree& t1,
BinaryTree& t2)
{
if (t1.root == t2.root && t1.root != NULL) {
cerr << "Left tree == right tree; "
<< " merge aborted" << endl;
return;
}
if (this != &t1 && this != &t2) deleteTree(root);
root = new BinaryNode(rootItem, t1.root,
t2.root);
//remove aliases
if (this != &t1) t1.root = NULL;
if (this != &t2) t2.root = NULL;
}

// delete a tree rooted at "root"
void BinaryTree::deleteTree(BinaryNode *root)
{ // postorder traversal
if (root == NULL) return; // nothing to delete
if (root->left != NULL)
deleteTree(root->left);
if (root->right != NULL)
deleteTree(root->right);
delete root;
}

//this is a continuation of the class BinaryNode
//TreeOperation is a class
//with one function: processNode()
void BinaryNode::preOrderTraversal(TreeOperation& op)
{
op.processNode(element);
if (left != NULL) left->preOrderTraversal(op);
if (right != NULL) right->preOrderTraversal(op);
}

void BinaryNode::inOrderTraversal(TreeOperation& op)
{
if (left != NULL) left->inOrderTraversal(op);
op.processNode(element);
if (right != NULL) right->inOrderTraversal(op);
}

void BinaryNode::postOrderTraversal(TreeOperation& op)
{
if (left != NULL) left->postOrderTraversal(op);
if (right != NULL) right->postOrderTraversal(op);
op.processNode(element);
}

// private function
// it finds a node containing the smallest item
BinaryNode *BinarySearchTree::findMin(BinaryNode *t)
throw(EmptyTree)
{
if (t == NULL) throw EmptyTree();
while (t->left != NULL) t = t->left;
return t;
}

// private function
// it finds a node containing element x
BinaryNode *BinarySearchTree::find(int x,
BinaryNode *t) throw(ItemNotFound )
{
while (t != NULL) {
if (x < t->element) t = t->left;
else if (x > t->element) t = t->right;
else return t; // found
}
throw ItemNotFound();
}

// private function
// it inserts a new node containing the element x
BinaryNode *BinarySearchTree::insert(int x,
BinaryNode *t) throw(DuplicateItem)
{
if (t == NULL) t = new BinaryNode(x);
else if (x < t->element)
t->left = insert(x, t->left);
else if (x > t->element)
t->right = insert(x, t->right);
else
throw DuplicateItem();
return t;
}

BinaryNode *BinarySearchTree::removeMin(BinaryNode *t)
throw(ItemNotFound) // private function
{
if (t == NULL) throw ItemNotFound();
if (t->left != NULL)
t->left = removeMin(t->left);
else {
BinaryNode *node = t;
t = t->right;
delete node;
}
return t;
}

BinaryNode *BinarySearchTree::remove(int x, BinaryNode *t)
throw(ItemNotFound) // private function
{
if (t == NULL) throw ItemNotFound();
if (x < t->element)
t->left = remove(x, t->left);
else if (x > t->element)
t->right = remove(x, t->right);
else if (t->left != NULL && t->right != NULL) {
// item x is found; t has two children
t->element = findMin(t->right)->element;
t->right = removeMin(t->right);
} else { //t has only one child
BinaryNode *node = t;
t = (t->left != NULL) ? t->left : t->right;
delete node;
}
return t;
}



