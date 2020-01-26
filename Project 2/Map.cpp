#include <iostream>
#include <string>
#include <cstdlib>
#include "Map.h"
bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

Map::Map() {
	m_s = 0;
	head = nullptr;
}
Map::~Map() {
	Node* temp = head;
	while(temp != nullptr) {
		Node* next = temp->next;
		delete temp;
		temp = next;
	}
}
Map::Map(Map& other) {
	head = nullptr;
	for(int i = 0;i < other.m_s;i++) {
		KeyType k;
		ValueType v;
		other.get(other.m_s - 1 - i,k,v);
		insert(k,v);
	}
	m_s = other.m_s;
}
Map& Map::operator=(const Map& other) {
	Node *temp = head;
	while(temp != nullptr) {
		Node* other = temp->next;
		delete temp;
		temp = other;
	}
	for(int i = 0;i < other.m_s;i++) {
		KeyType k;
		ValueType v;
		other.get(other.m_s - 1 - i,k,v);
		insert(k,v);
	}
	m_s = other.m_s;
	return *this;	
}
bool Map::empty() const {
	return m_s == 0;
}
int Map::size() const {
	return m_s;
}
bool Map::insert(const KeyType& key, const ValueType& value) {
	if(contains(key)) return false;
	m_s++;
	KVPair kv;
	kv.k = key;
	kv.v = value;
	Node* nextNode = new Node;
	nextNode->pair = kv;
	if(head == nullptr) {
		nextNode->next = nullptr;
		nextNode->prev = nullptr;
		head = nextNode;
		return true;
	}
	Node* temp = head;
	nextNode->next = head;
	nextNode->prev = nullptr;
	temp->prev = nextNode;
	head = nextNode;
	return true;
}
bool Map::update(const KeyType& key, const ValueType& value) {
	if(!contains(key)) return false;
	Node* temp = head;
	while(temp != nullptr && temp->pair.k != key) 
		temp = temp->next;
	temp->pair.v = value;
	return true;
}
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	contains(key) ? update(key,value) : insert(key,value);
	return true;
}
bool Map::erase(const KeyType& key) {
	if(!contains(key)) return false;
	m_s--;
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

	Node *curr = head;
	while(curr->pair.k != key)
		curr = curr->next;
	if(curr->next == nullptr) {
		curr->prev->next = nullptr;
		delete curr;
		return true;
	}

	Node *nextAfter = curr->next; 
	Node *previous = curr->prev;


	nextAfter->prev = previous;
	previous->next = nextAfter;
	delete curr; 
	return true;
}
bool Map::contains(const KeyType& key) const {
	Node* temp = head;
	while(temp != nullptr) { 
		if(temp->pair.k == key) return true;
		temp = temp->next;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
	if(!contains(key)) return false;
	Node* temp = head;
	while(temp != nullptr) { 
		if(temp->pair.k == key) value = temp->pair.v;
		temp = temp->next;
	}
	return true;
}
bool Map::get(int i, KeyType& key, ValueType& value) const {
	Node* temp = head;
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
void Map::swap(Map& other) {
	Node* temp = head;
	head = other.head;
	other.head = temp;
	int tempSize = size();
	m_s = other.size();
	other.m_s = tempSize;

}
void Map::dump() {
	Node* temp = head;
	while(temp != nullptr) {
		cout << temp->pair.k << " " << temp->pair.v << endl;
		temp = temp->next;
	}
}
int main() {
	Map m;
	m.insert("manish", 4.0);
	m.insert("amara", 10.0);
	m.insert("lordan", 5.0);
	m.insert("krishnan", 2.0);
	m.insertOrUpdate("baldwins", 2.0);
	KeyType k;
	Map x;
	x.insert("Fred", 123);
	x.insert("Ethel", 456);
	x.insert("Lucy", 789);
	x.insert("Ricky", 321);
	Map r;
	r = m;
	r.dump();
}


bool combine(const Map& m1, const Map& m2, Map& result) {
	int original_size = result.size();
	for(int i = 0;i < original_size;i++) {
		KeyType k;	
		ValueType v;
		result.get(0,k,v);
		result.erase(k);
	}
	bool sameDiffVal = true;
	for(int i = 0;i < (m2.size() >= m1.size() ? m2.size() : m1.size());i++) {
		KeyType k;
		ValueType v;
		if(i < m2.size()) {
			m2.get(i,k,v); 
			if(!m1.contains(k)) result.insert(k,v);
			else if(m1.contains(k)) {
				ValueType v0;
				m1.get(k,v0);
				if(v == v0) result.insert(k,v);
				else sameDiffVal = false;
			}
		}
		if(i < m1.size()) {
			m1.get(i,k,v);
			if(!m2.contains(k)) result.insert(k,v);
		}		
	}
	return sameDiffVal;
	return true;	
}

void reassign(const Map& m, Map& result) {
    int original_size = result.size();
    for(int i = 0;i < original_size;i++) {
        KeyType k;
        ValueType v;
        result.get(0,k,v);
        result.erase(k);
    }
	for(int i = 0;i < m.size()/2;i++) {
		ValueType v1;
		KeyType k1;
		ValueType v2;
		KeyType k2;
		m.get(i,k1,v1);
		m.get(m.size()-1-i,k2,v2);
		result.insert(k1, v2);
		result.insert(k2,v1);			
	}	
	if(m.size() % 2 == 1) {
    	KeyType k;
   		ValueType v;
    	m.get(m.size()/2,k,v);
		result.insert(k,v);
    	KeyType k1;
    	ValueType v1;
    	result.get(result.size()-1,k1,v1);
    	result.update(k1,v);
    	result.update(k,v1);
   	}	
}








