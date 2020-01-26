#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

typedef string KeyType;
typedef double ValueType;

class Map {
	public:
		Map();
		~Map();
		Map(const Map& other);
		Map& operator=(const Map& other);
		bool empty() const;
		int size() const;
		bool insert(const KeyType& key, const ValueType& value);
		bool update(const KeyType& key, const ValueType& value);
		bool insertOrUpdate(const KeyType& key, const ValueType& value);
		bool erase(const KeyType& key);
		bool contains(const KeyType& key) const;
		bool get(const KeyType& key, ValueType& value) const;
		bool get(int i, KeyType& key, ValueType& value) const;
		void swap(Map& other);
		void dump() const;
	private:
		struct KVPair {
			KeyType k;
			ValueType v;
			KVPair(): k(""), v(0.0) {}
		};
		struct Node {
			KVPair pair;
			Node* next;
			Node* prev;
			Node(): next(nullptr), prev(nullptr) {}	
		};
		int m_s;
		Node* head;
};

#endif




