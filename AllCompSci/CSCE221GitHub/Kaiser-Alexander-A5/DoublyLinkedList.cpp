#include "DoublyLinkedList.h"
#include <stdexcept>
// extend range_error from <stdexcept>
struct EmptyDLinkedListException : std::range_error {
  explicit EmptyDLinkedListException(char const* msg=NULL): range_error(msg) {}
};
// copy constructor
DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList& dll)//O(n)
{
  // Initialize the list
  header.next = &trailer; trailer.prev = &header;


  // Delete the whole list
  /* Complete this function */
  
DListNode* p = dll.header.next;
while (p != &(dll.trailer))
{
insertLast(p->getElem());
p = p->next;
}

}
  // Copy from dll
  /* Complete this function */

// assignment operator
DoublyLinkedList& DoublyLinkedList::operator=(const DoublyLinkedList& dll)//O(n)
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
  
DListNode* p = dll.header.next;
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
void DoublyLinkedList::insertFirst(int newobj)//O(1)
{ 
  DListNode *newNode = new DListNode(newobj, &header, header.next);
  header.next->prev = newNode;
  header.next = newNode;
}
// insert the object to the last of the linked list
void DoublyLinkedList::insertLast(int newobj)//O(1)
{
  DListNode *newNode = new DListNode(newobj, trailer.prev,&trailer);
  trailer.prev->next = newNode;
  trailer.prev = newNode;
}
// remove the first object of the list
int DoublyLinkedList::removeFirst()//O(1)
{ 
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  DListNode *node = header.next;
  node->next->prev = &header;
  header.next = node->next;
  int obj = node->obj;
  delete node;
  return obj;
}
// remove the last object of the list
int DoublyLinkedList::removeLast()//O(1)
{
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  DListNode *node = trailer.prev;
  node->prev->next = &trailer;
  trailer.prev = node->prev;
  int obj = node->obj;
  delete node;
  return obj;
}
// destructor
DoublyLinkedList::~DoublyLinkedList(){//O(n)
  DListNode *prev_node, *node = header.next;
  while (node != &trailer) {
    prev_node = node;
    node = node->next;
    delete prev_node;
  }
  header.next = &trailer;
  trailer.prev = &header;
}
// return the first object
int DoublyLinkedList::first() const//O(1)
{ 
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  return header.next->obj;
}
// return the last object
int DoublyLinkedList::last() const//O(1)
{
  if (isEmpty())
    throw EmptyDLinkedListException("Empty Doubly Linked List");
  return trailer.prev->obj;
}
// return the list length
int DoublyLinkedListLength(DoublyLinkedList& dll) {//O(n)
  DListNode *current = dll.getFirst();
  int count = 0;
  while(current != dll.getAfterLast()) {
    count++;
    current = current->getNext(); //iterate
  }
  return count;
}
// output operator
ostream& operator<<(ostream& out, const DoublyLinkedList& dll) {//O(n)
  
  /* Complete this function */
  DListNode* p = dll.getFirst();
  out << "-Infinity ";
  while(p != dll.getAfterLast()){
	  out << p->getElem() << " "; 
	  p = p->getNext();
  }
  out << "Infinity ";
  return out;
}


DListNode* DListNode::insert_before(int d) {
  DListNode* temp = new DListNode(d);  //temp pointer // 1 OPERATION

  if(prev!= NULL){   //if previous is not null // 2 OPERATION
    temp -> next = this;    //initialize next // 2 OPERATION
    temp -> prev = prev;   //initialize previous // 2 OPERATION
    prev -> next = temp;   // insert in the list  // 2 OPERATION
    prev = temp;          // 1 OPERATION
  }
  else{       //if null than do a simple insert
    temp -> next = this;  // 2 OPERATION
    prev = temp;   // 1 OPERATION
  }

  return temp; // 1 OPERATION
}

DListNode* DListNode::insert_after(int d) {
  DListNode* temp = new DListNode(d);   // 2 OPERATION
  if(next != NULL){   //if next is not null do this // 2 OPERATION
    temp -> next = next;      //initialize next and previous // 2 OPERATION
    temp -> prev = this;     // 2 OPERATION
    next -> prev = temp;   //make the connection to insert // 2 OPERATION
    next = temp; // 1 OPERATION
  }
  else{ //if null than do a simple insert
    temp -> prev = this;  // 2 OPERATION
    next = temp; // 1 OPERATION
  }

  return temp;  // 1 OPERATION
}

void DListNode::delete_before() {
  if(prev!= NULL){   //check if something is there  // 2 OPERATION
    DListNode* temp = prev;         // 1 OPERATION
    temp -> prev-> next = temp-> next;    //change the interconnections  // 4 OPERATION
    temp -> next -> prev = temp -> prev; // 4 OPERATION
    delete temp;        //delete the element // 1 OPERATION
  }
  else{
    cout << ">Error: Nothing is there :(" << endl;
  }
}

void DListNode::delete_after() {
  if(next != NULL){ //check if something is there
    DListNode* temp = next;  // 1 OPERATION
    next -> prev = this; //change the interconnections // 2 OPERATION
    next = next -> next; // 2 OPERATION
    delete temp;  //delete the element // 1 OPERATION
  }
  else{
    cout << ">Error: Nothing is there :(" << endl;
  }

}
