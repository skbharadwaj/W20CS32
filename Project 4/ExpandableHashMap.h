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
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
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
    void print() const;

private:
    struct HashNode {
        HashNode(KeyType key, ValueType val) : k(key), v(val) {}
        KeyType k;
        ValueType v;
    };
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

template<typename KeyType, typename ValueType>
double ExpandableHashMap<KeyType, ValueType>::getCurrentLoadFactor() const {
    return ((double)(size()))/(numBuckets);
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    maxLoadFactor = maximumLoadFactor > 0 ? maximumLoadFactor : 0.5;
    numBuckets = 8;
    totalCount = 0;
    m_hashmap = new list<HashNode*>[numBuckets];
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    for(int i = 0;i < numBuckets;i++) m_hashmap[i].clear();
    delete [] m_hashmap;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    for(int i = 0;i < numBuckets;i++) m_hashmap[i].clear();
    numBuckets = 8;
    delete [] m_hashmap;
    m_hashmap = new list<HashNode*>[numBuckets];
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return totalCount;
}

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

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::print() const {
    for(int i = 0;i < numBuckets;i++) {
        typename list<HashNode*>::iterator p;
        for(p = m_hashmap[i].begin();p != m_hashmap[i].end();p++)
            cout << (*p)->k << " " << (*p)->v << endl;
    }
}
#endif



