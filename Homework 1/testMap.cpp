#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
	Map m;
	assert(m.empty());
	ValueType v = -1234.5;
	assert(!m.get("abc",v) && v == -1234.5);
	m.insert("xyz", 9876.5);
	assert(m.size() == 1);
	KeyType k = "hello";
	assert(m.get(0,k,v) && k == "xyz" && v == 9876.5);
	m.insertOrUpdate("xyz", 5000.0);
	m.insert("abc", 3.5);
	assert(m.contains("abc"));
	assert(m.get(0,k,v) && k == "xyz" && v == 5000);
	assert(m.size() == 2);

	Map other;
	other.insert("k", 50);
	other.insertOrUpdate("hello", 20);
	
	other.swap(m);
	assert(m.contains("k"));
	assert(other.size() == 2);
	cout << "Tests passed." << endl;
	return 0; 

} 
