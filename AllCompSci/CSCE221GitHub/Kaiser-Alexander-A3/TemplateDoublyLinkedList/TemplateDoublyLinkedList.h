#include <cstdlib>
#include <iostream>
#include <stdexcept>
using namespace std;
template <typename T>
class DoublyLinkedList; // class declaration

// list node
template <typename T>
class DListNode {
private: T obj;
  DListNode *prev, *next;
  friend class DoublyLinkedList<T>;
public:
  DListNode(T e="", DListNode *p = NULL, DListNode *n = NULL)
    : obj(e), prev(p), next(n) {}
  T getElem() const { return obj; }
  DListNode * getNext() const { return next; }
  DListNode * getPrev() const { return prev; }
};

// doubly linked list
template <typename T>
class DoublyLinkedList {
protected: DListNode<T> header, trailer;
public:
  DoublyLinkedList<T>() : header(""), trailer("") // constructor
  { header.next = &trailer; trailer.prev = &header; }
  DoublyLinkedList(const DoublyLinkedList& dll); // copy constructor
  ~DoublyLinkedList(); // destructor
  DoublyLinkedList& operator=(const DoublyLinkedList& dll); // assignment operator
  // return the pointer to the first node
  DListNode<T> *getFirst() const { return header.next; } 
  // return the pointer to the trailer
  const DListNode<T> *getAfterLast() const { return &trailer; }
  // return if the list is empty
  bool isEmpty() const { return header.next == &trailer; }
  T first() const; // return the first object
  T last() const; // return the last object
  void insertFirst(T newobj); // insert to the first of the list
  T removeFirst(); // remove the first node
  void insertLast(T newobj); // insert to the last of the list
  T removeLast(); // remove the last node
};
// output operator
template <typename T>
ostream& operator<<(ostream& out, const DoublyLinkedList<T>& dll);

// extend range_error from <stdexcept>
struct EmptyDLinkedListException : std::range_error {
  explicit EmptyDLinkedListException(char const* msg=NULL): range_error(msg) {}
};
// copy constructor
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& dll)
{
  // Initialize the list
  header.next = &trailer; trailer.prev = &header;
 /* 
    DListNode* head = new DListNode;
    head->prev = dll.header.prev;
    head->obj = dll.header.obj;
    head->next = dll.header.next;

    DListNode* tail = new DListNode;
    tail->prev = dll.trailer.prev;
    tail->obj = dll.trailer.obj;
    tail->next = dll.trailer.next;

    DListNode* curr = new DListNode;
    curr->prev = NULL;
	curr->next = dll.header.next;

    while(curr != tail) {

        DListNode* n = new DListNode;
        curr->next = n;
        n = curr->prev->next;
        curr = curr->next;
    }

    curr = tail;
	
	
*/


  // Delete the whole list
  /* Complete this function */
  
DListNode<T>* p = dll.header.next;
while (p != &(dll.trailer))
{
insertLast(p->getElem());
p = p->next;
}

}
  // Copy from dll
  /* Complete this function */

// assignment operator
template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList<T>& dll)
{
if (this == &dll) // Check for self assignment
{
return *this;
}
while (!isEmpty()) // Delete the current list contents
{
removeLast();
}

  // Delete the whole list
  /* Complete this function */
  
DListNode<T>* p = dll.header.next;
while (p != &(dll.trailer))
{
insertLast(p->getElem());
p = p->next;
}
return *this;
}
  // Copy from dll
  /* Complete this function */

// insert the object to the first of the linked list
template <typename T>
void DoublyLinkedList<T>::insertFirst(T newobj)
{ 
  DListNode<T> *newNode = new DListNode<T>(newobj, &header, header.next);
  header.next->prev = newNode;
  header.next = newNode;
}
// insert the object to the last of the linked list
template <typename T>
void DoublyLinkedList<T>::insertLast(T newobj)
{
  DListNode<T>*newNode = new DListNode<T>(newobj, trailer.prev,&trailer);
  trailer.prev->next = newNode;
  trailer.prev = newNode;
}
// remove the first object of the list
template <typename T>
T DoublyLinkedList<T>::removeFirst()
{ 
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  DListNode<T> *node = header.next;
  node->next->prev = &header;
  header.next = node->next;
  T obj = node->obj;
  delete node;
  return obj;
}
// remove the last object of the list
template <typename T>
T DoublyLinkedList<T>::removeLast()
{
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  DListNode<T> *node = trailer.prev;
  node->prev->next = &trailer;
  trailer.prev = node->prev;
  T obj = node->obj;
  delete node;
  return obj;
}
// destructor
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList<T>()
{
  DListNode<T> *prev_node, *node = header.next;
  while (node != &trailer) {
    prev_node = node;
    node = node->next;
    delete prev_node;
  }
  header.next = &trailer;
  trailer.prev = &header;
}
// return the first object
template <typename T>
T DoublyLinkedList<T>::first() const
{ 
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  return header.next->obj;
}
// return the last object
template <typename T>
T DoublyLinkedList<T>::last() const
{
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  return trailer.prev->obj;
}
// return the list length
template <typename T>
int DoublyLinkedListLength(DoublyLinkedList<T>& dll) {
  DListNode<T> *current = dll.getFirst();
  int count = 0;
  while(current != dll.getAfterLast()) {
    count++;
    current = current->getNext(); //iterate
  }
  return count;
}
// output operator
template <typename T>
ostream& operator<<(ostream& out, const DoublyLinkedList<T>& dll) {
  
  /* Complete this function */
  DListNode<T>* p = dll.getFirst();
  while(p != dll.getAfterLast()){
	  out << p->getElem() << " "; 
	  p = p->getNext();
  }
  return out;
}
