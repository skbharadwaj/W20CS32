#ifndef NEWMAP_H
#define NEWMAP_H

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;
typedef string KeyType;
typedef double ValueType;

class Map {
			
	public:
		static const int DEFAULT_MAX_ITEMS = 240; 
		Map();
        Map(int sizeOfMap);
        Map(Map& other);
        ~Map();
		bool empty() const;
		int size() const;
		bool insert(const KeyType& key, const ValueType& value);
		bool update(const KeyType& key, const ValueType& value);
		bool insertOrUpdate(const KeyType& key, const ValueType& value);
		bool erase(const KeyType& key);
		bool contains(const KeyType& key) const;
		bool get(const KeyType& key, ValueType& value) const;
		bool get(int i, KeyType& key, ValueType& value) const;
        void operator=(const Map& other);
		void swap(Map& other);
	private:
		struct KVPair {
			KeyType k;
			ValueType v;
		};
		int m_s, m_c;
		KVPair *pairs;
		bool inMap(const KeyType& key) const;
};
	

#endif
