#include "newMap.h"
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

	Map::Map() {
        pairs = new KVPair[DEFAULT_MAX_ITEMS];
		m_s = DEFAULT_MAX_ITEMS;
        m_c = 0;
	}
    Map::Map(int sizeOfMap) {
        pairs = new KVPair[sizeOfMap];
        m_s = sizeOfMap;
        m_c = 0;
    }
    Map::Map(const Map& other) {
		m_s = other.m_s;
		m_c = other.m_c;
        pairs = new KVPair[other.m_s];
        for(int i = 0;i < m_s;i++)
			pairs[i] = other.pairs[i];
    }
    Map::~Map() {
        delete [] pairs;
    }
    void Map::operator=(const Map& other) {
		// delete [] this->pairs;
		m_s = other.m_s;
		m_c = other.m_c;
        pairs = new KVPair[m_s];
        for(int i = 0;i < m_s;i++)
			pairs[i] = other.pairs[i];      
    }
	bool Map::empty() const {
		return m_c == 0;
	}
	int Map::size() const {
		return m_c;
	}
	bool Map::inMap(const KeyType& key) const {
		for(int i = 0;i < m_c;i++) {
            if((pairs)[i].k == key) return true;
        }
		return false;
	}
	bool Map::insert(const KeyType& key, const ValueType& value) {
		if(m_c == m_s || inMap(key)) return false;
		pairs[m_c].k = key;
		pairs[m_c].v = value;
		m_c++;
		return true;
	}
	bool Map::update(const KeyType& key, const ValueType& value) {
		for(int i = 0;i < m_c;i++) {
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
		for(int i = 0;i < m_c;i++) {
			if(pairs[i].k == key) {
				index = i;	
			}
		}
		for(int i = index;i < m_c;i++) {
			KVPair temp = pairs[i];
			pairs[i] = pairs[i+1];
			pairs[i+1] = temp;
		}
		this->m_c--;
		return true;	
	}
	bool Map::contains(const KeyType& key) const { return inMap(key); }
	bool Map::get(const KeyType& key, ValueType& value) const {
		if(!inMap(key)) return false; 
		for(int i = 0;i < m_s;i++) if(pairs[i].k == key) value = pairs[i].v;
		return true;
	}
	bool Map::get(int i, KeyType& key, ValueType& value) const {
		if(i < 0 || i >= m_c) return false;
		for(int k = 0;k < m_c;k++) 
			if(k == i) {
				key = pairs[i].k;
				value = pairs[i].v;
			}
		return true;
	}
	void Map::swap(Map& other) {
        KVPair *temp = pairs;
        this->pairs = other.pairs;
        other.pairs = temp;
        int tempSize = this->m_s;
        this->m_s = other.m_s;
        other.m_s = tempSize;
        tempSize = this->m_c;
        this->m_c = other.m_c;
        other.m_c = tempSize;
	}	
