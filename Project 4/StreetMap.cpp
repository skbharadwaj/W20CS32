#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>

using namespace std;


unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

unsigned int hasher(const string& g)
{
    return std::hash<string>()(g);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
    void insertSegment(StreetSegment s1);
};

StreetMapImpl::StreetMapImpl()
{

}

StreetMapImpl::~StreetMapImpl()
{
}

void StreetMapImpl::insertSegment(StreetSegment s1) {
    vector<StreetSegment>* curr = m_map.find(s1.start);
    if(curr == nullptr) {
        vector<StreetSegment> newVec;
        newVec.push_back(s1);
        m_map.associate(s1.start, newVec);
    }
    else curr->push_back(s1);
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if(!infile) return false;
    string name;
    while (getline(infile, name))
    {
        string num("");
        getline(infile, num);
        if(num == "") break;
        int numSegments = stoi(num);
        for(int i = 0;i < numSegments;i++) {
            string start_lat, start_lon, end_lat, end_lon;
            infile >> start_lat;
            infile >> start_lon;
            infile >> end_lat;
            infile >> end_lon;
            GeoCoord g1(start_lat, start_lon), g2(end_lat, end_lon);
            StreetSegment s1(g1, g2, name), s2(g2, g1, name);
            insertSegment(s1);
            insertSegment(s2);
        }

        infile.ignore(10000, '\n');
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    const vector<StreetSegment>* temp = m_map.find(gc);
    if(temp) { segs = *temp; return true;}
    return false;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
