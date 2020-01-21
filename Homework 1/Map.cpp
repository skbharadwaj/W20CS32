#include "Map.h"
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

	Map::Map() {
		m_s = 0;
	}
	bool Map::empty() const {
		return m_s == 0;
	}
	int Map::size() const {
		return m_s;
	}
	bool Map::inMap(const KeyType& key) const {
	        for(int i = 0;i < m_s;i++) 
            		if(pairs[i].k == key) return true;
		return false;
	}
	bool Map::insert(const KeyType& key, const ValueType& value) {
		if(m_s == DEFAULT_MAX_ITEMS || inMap(key)) return false;
		pairs[m_s].k = key;
		pairs[m_s].v = value;
		m_s++;
		return true;
	}
	bool Map::update(const KeyType& key, const ValueType& value) {
		for(int i = 0;i < m_s;i++) {
			if(pairs[i].k == key) { 
				pairs[i].v = value;
				return true;
			}
		}
		return false;
	}
	bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
		return inMap(key) ? update(key, value) : insert(key, value);
	}
	bool Map::erase(const KeyType& key) {
		if(!inMap(key)) return false;
		int index = -1;
		for(int i = 0;i < m_s;i++) {
			if(pairs[i].k == key) {
				index = i;	
			}
		}
		for(int i = index;i < m_s;i++) {
			KVPair temp = pairs[i];
			pairs[i] = pairs[i+1];
			pairs[i+1] = temp;
		}
		this->m_s--;
		return true;	
	}
	bool Map::contains(const KeyType& key) const { return inMap(key); }
	bool Map::get(const KeyType& key, ValueType& value) const {
		if(!inMap(key)) return false;
		for(int i = 0;i < m_s;i++) if(pairs[i].k == key) value = pairs[i].v;
		return true;
	}
	bool Map::get(int i, KeyType& key, ValueType& value) const {
		if(i < 0 || i >= m_s) return false;
		for(int k = 0;k < m_s;k++) 
			if(k == i) {
				key = pairs[i].k;
				value = pairs[i].v;
			}
		return true;
	}
	void Map::swap(Map& other) {
		KVPair temp;
		for(int i = 0;i < m_s;i++) {
			temp = pairs[i];
			pairs[i] = other.pairs[i];
			other.pairs[i] = temp;
		}
		int tempSize = other.m_s;
		other.m_s = this->m_s;
		m_s = tempSize; 
	}	

