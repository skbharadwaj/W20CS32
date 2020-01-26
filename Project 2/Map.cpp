#include <iostream>
#include <string>
#include <cstdlib>
#include "Map.h"
// method declaration for combine and reassign
bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);
// default constructor setting size = 0 and head to a null pointer
Map::Map() {
	m_s = 0;
	head = nullptr;
}
/* Destructor. Deletes all nodes in the list */
Map::~Map() {
	Node* temp = head;
	while(temp != nullptr) {
		Node* next = temp->next;
		delete temp;
		temp = next;
	}
}
/* Copy constructor. Takes all the nodes from the Map other and copies them into
this one. Does so by looping over all values of other from zero to other.size() - 1
and inserting them into the map */
Map::Map(const Map& other) {
	if(this == &other) return;
	head = nullptr;
	for(int i = 0;i < other.m_s;i++) {
		KeyType k;
		ValueType v;
		other.get(other.m_s - 1 - i,k,v);
		insert(k,v);
	}
	m_s = other.m_s;
}
// assignment operator that takes advantage of copy constructor to create
// this as a copy of other
Map& Map::operator=(const Map& other) {
	if(this == &other) return *this;
	Map x(other);
	swap(x);
	m_s = other.m_s;
	return *this;
}
// returns whether list is empty
bool Map::empty() const {
	return m_s == 0;
}
// returns size of list
int Map::size() const {
	return m_s;
}
/* insert function that inserts new key-value pairs into the list. The 
advantage of a linked list is the constant insertion time, and this algorithm
takes advantage of that by inserting the new node as the head node every time, 
if the list doesn't already contain the key. The head is moved to the newly 
created node and the old head's previous is set as this new node, thus properly 
linking the nodes. Special case handled for the first node in the list. */
bool Map::insert(const KeyType& key, const ValueType& value) {
	if(contains(key)) return false;
	// increment the size if the key isn't in the list
	m_s++;
	KVPair kv; 	// create a new node and key value pair structure and put it in new node
	kv.k = key;
	kv.v = value;
	Node* nextNode = new Node;
	nextNode->pair = kv;
	// if the node being created is first, handle accordingly
	// by setting it to head and returning.
	if(head == nullptr) {
		nextNode->next = nullptr;
		nextNode->prev = nullptr;
		head = nextNode;
		return true;
	}
	// otherwise, make the new node the head, adjust the previous 
	// of the head node and next of this node, set to head, and return
	Node* temp = head;
	nextNode->next = head;
	nextNode->prev = nullptr;
	temp->prev = nextNode;
	head = nextNode;
	return true;
}
/* updates the value corresponding to key with the given value.
First, checks whether the key is present in the map, and if so, changes
the corresponding v value to that specified by the parameter.*/
bool Map::update(const KeyType& key, const ValueType& value) {
	// return if the key isn't present
	if(!contains(key)) return false;
	// look for the node, and adjust the value accordingly 
	Node* temp = head;
	while(temp != nullptr && temp->pair.k != key) 
		temp = temp->next;
	temp->pair.v = value;
	return true;
}
// inserts if key-valye pair not present in the list, and updates otherwise.
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	contains(key) ? update(key,value) : insert(key,value);
	return true;
}
/*
erases the given node containing the key value pair, if it is in the list.
Does so by first handling a special case in which the desired value is the 
first one, and then more generally finding the node and moving the previous
of the next and the next of the previous to delete the node from the list, and 
then deallocating it.
*/
bool Map::erase(const KeyType& key) {
	if(!contains(key)) return false;
	// if present, it will be erased, so decrement the size
	m_s--;
	// if the node to be deleted is head, check whether the 
	// head is the only node, and if so, delete it and make the list empty
	// if not, take the next node and make it head and delete the current head
	if(head->pair.k == key) {
		if(head->next == nullptr) {
			delete head;
			head = nullptr;
			return true;
		}
		Node *temp = head->next;
		delete head;
		head = temp;
		head->prev = nullptr;
		return true;	
	}
	// traversees to the node
	Node *curr = head;
	while(curr->pair.k != key)
		curr = curr->next;
	// end case: set the next of the 2nd last to nullptr 
	// and delete the current node
	if(curr->next == nullptr) {
		curr->prev->next = nullptr;
		delete curr;
		return true;
	}
	// middle case: exchange the previous and next's previous
	// and nexts to each other and delete the current node
	Node *nextAfter = curr->next; 
	Node *previous = curr->prev;
	nextAfter->prev = previous;
	previous->next = nextAfter;
	delete curr; 
	return true;
}

//loops through the list checking if any node contains key
bool Map::contains(const KeyType& key) const {
	Node* temp = head;
	while(temp != nullptr) { 
		if(temp->pair.k == key) return true;
		temp = temp->next;
	}
	return false;
}
/* copies the value of value corresponding to the given parameter
 key. If not found, returns false. */
bool Map::get(const KeyType& key, ValueType& value) const {
	if(!contains(key)) return false;
	Node* temp = head;
	// loops through and finds the appropriate key, and sets value to it
	// returns true once finished
	while(temp != nullptr) { 
		if(temp->pair.k == key) value = temp->pair.v;
		temp = temp->next;
	}
	return true;
}
/* gets the ith node, and copies the value of its key and value
into the parameter values key and value. This way, looping
through 0-size() will get values of the whole list.*/
bool Map::get(int i, KeyType& key, ValueType& value) const {
	Node* temp = head;
	// loop through and get the key and value corresponding to the 
	// ith node of the list. This way, if i ranges from 0 to size() - 1
	// the entire list is printed
	for(int k = 0;k < size();k++) {
		if(k == i) {
			key = temp->pair.k;
			value = temp->pair.v;	
			break;
		}
		temp = temp->next;
	}
	return true;
}
// swaps the contents of this list with the other
void Map::swap(Map& other) {
	// exchange head nodes and sizes to maintain constant time swap
	Node* temp = head;
	head = other.head;
	other.head = temp;
	int tempSize = size();
	m_s = other.size();
	other.m_s = tempSize;

}
void Map::dump() const {
	Node* temp = head;
	while(temp != nullptr) {
		cout << temp->pair.k << " " << temp->pair.v << endl;
		temp = temp->next;
	}
}
int main() { /*
	Map m;
	m.insert("manish", 4.0);
	m.insert("amara", 10.0);
	m.insert("lordan", 5.0);
	m.insert("krishnan", 2.0);
	m.insertOrUpdate("baldwins", 2.0);
	Map f;
	f.insert("Fred", 123);
	f.insert("Ethel", 456);
	f.insert("Lucy", 789);
	f.insert("Ricky", 321);
	m = f;
	m.dump();
	cout << endl; */
}
/* combines the maps m1 and m2 according to the following specifications:
If a key appears in exactly one of m1 and m2, then result must contain a 
pair consisting of that key and its corresponding value.
If a key appears in both m1 and m2, with the same corresponding value in 
both, then result must contain exactly one pair with that key and value. */
bool combine(const Map& m1, const Map& m2, Map& result) {
	Map x; // creates temporary map to protect against aliasing
	bool sameDiffVal = true; // keeps track of same key, different value pairs 
	// loops through the larger map and copies over nodes according to the spec
	for(int i = 0;i < (m2.size() >= m1.size() ? m2.size() : m1.size());i++) {
		KeyType k;
		ValueType v;
		// check bounds of both maps and add to temporary map 
		if(i < m2.size()) {
			m2.get(i,k,v); 
			if(!m1.contains(k)) x.insert(k,v);
			else if(m1.contains(k)) {
				ValueType v0;
				m1.get(k,v0);
				if(v == v0) x.insert(k,v);
				else sameDiffVal = false;
			}
		}
		if(i < m1.size()) {
			m1.get(i,k,v);
			if(!m2.contains(k)) x.insert(k,v);
		}		
	}
	// exchange x with result. x is deleted after the method ends
	x.swap(result);	
	return sameDiffVal;
}
// mixes up the key and value pairs in m and puts them 
// in result, such that no key-value pair is the same as 
// before but none are repeated.
void reassign(const Map& m, Map& result) {
	if(m.size() == 1) return; // 1 node map is unchanged
	// temporary map to protect against aliasing
	Map x;	
	// loops through the map and copies each element to an appropriate
	// position, which is the position reflected across the middle
	for(int i = 0;i < m.size()/2;i++) {
		ValueType v1;
		KeyType k1;
		ValueType v2;
		KeyType k2;
		m.get(i,k1,v1);
		m.get(m.size()-1-i,k2,v2);
		x.insert(k1, v2);
		x.insert(k2,v1);			
	}
	// if the size is odd, the middle node is left unchanged, so 
	// exchange it with the 2nd to last one 
	if(m.size() % 2 == 1) {
    	KeyType k;
   		ValueType v;
    	m.get(m.size()/2,k,v);
		x.insert(k,v);
    	KeyType k1;
    	ValueType v1;
    	x.get(x.size()-1,k1,v1);
    	x.insertOrUpdate(k1,v);
    	x.insertOrUpdate(k,v1);
   	}		
	// put into results the contents of x, x is deleted when method ends
	x.swap(result);
}








