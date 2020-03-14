#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h
#include <list>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>


using namespace std;
template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    // constructor with default load factor of 0.5
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    // destructor
    ~ExpandableHashMap();
    // reset the map to the default values
    void reset();
    // returns the size of the map
    int size() const;
    // adds a key value pair to the map
    void associate(const KeyType& key, const ValueType& value);
      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }
      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    // Node to put into the HashMap
    struct HashNode {
        HashNode(KeyType key, ValueType val) : k(key), v(val) {}
        KeyType k;
        ValueType v;
    };
    // member variables for load factor, total associations, buckets, etc.
    double maxLoadFactor;
    int numBuckets = 8, totalCount = 0;
    list<HashNode*>* m_hashmap;
    double getCurrentLoadFactor() const;
    unsigned int getBucketNumber(const KeyType& key) const {
        unsigned int hasher(const KeyType& k);  // prototype
        unsigned int h = hasher(key);
        return h % numBuckets;
    }

};

// returns the load factor
template<typename KeyType, typename ValueType>
double ExpandableHashMap<KeyType, ValueType>::getCurrentLoadFactor() const {
    return ((double)(size()))/(numBuckets);
}

// constructor that takes the load factor as a parameter and constructs an
// instance of the HashMap, keeping the initial number of buckets at 8
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    maxLoadFactor = maximumLoadFactor > 0 ? maximumLoadFactor : 0.5;
    numBuckets = 8;
    totalCount = 0;
    m_hashmap = new list<HashNode*>[numBuckets];
}

// destructor
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    for(int i = 0;i < numBuckets;i++) m_hashmap[i].clear();
    delete [] m_hashmap;
}

// resets the map to the default values
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    for(int i = 0;i < numBuckets;i++) m_hashmap[i].clear();
    numBuckets = 8;
    delete [] m_hashmap;
    m_hashmap = new list<HashNode*>[numBuckets];
}

// returns the size of the map
template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return totalCount;
}


// adds a new key value pair to the map and resets the number of buckets
// and expands the map if necessary. The runtime for this function in the
// average case is O(1), but can be as high as O(N) for N elements in the map
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    if(find(key) == nullptr) {
        HashNode* newNode = new HashNode(key, value);
        int whichBucket = getBucketNumber(key);
        m_hashmap[whichBucket].push_back(newNode);
        totalCount++;
    }

    else {
        for(int i = 0;i < numBuckets;i++) {
            typename list<HashNode*>::iterator p;
            for(p = m_hashmap[i].begin();p != m_hashmap[i].end();p++) {
                HashNode* curr = *p;
                if(curr->k == key) curr->v = value;
            }
        }
    }

    if(getCurrentLoadFactor() > maxLoadFactor) {
        list<HashNode*>* newHashMap = new list<HashNode*>[2*numBuckets];
        numBuckets *= 2;
        for(int i = 0;i < numBuckets/2;i++) {
            typename list<HashNode*>::iterator p;
            for(p = m_hashmap[i].begin();p != m_hashmap[i].end();p++) {
                HashNode* currNode = *p;
                int whichBucket = getBucketNumber(currNode->k);
                newHashMap[whichBucket].push_back(currNode);
            }
        }
        m_hashmap = newHashMap;

    }

}

// finds and returns a pointer to the ValueType corresponding to the key
// parameter. In the average case, the runtime of this function is O(1)
template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int buck = getBucketNumber(key);
    typename list<HashNode*>::iterator p;
    for(p = m_hashmap[buck].begin();p != m_hashmap[buck].end();p++) {
        HashNode* curr = *p;
        if(curr->k == key) return &(curr->v);
    }
    return nullptr;
}

#endif



