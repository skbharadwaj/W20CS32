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

// inserts the given StreetSegment into the ExpandableHashMap
// finds the vector corresponding to it if it exists, and otherwise,
// makes a new vector and inserts it into the map
void StreetMapImpl::insertSegment(StreetSegment s1) {
    // finds the vector of StreetSegments in the map
    vector<StreetSegment>* curr = m_map.find(s1.start);
    // inserts it into the vector if it exists, or
    if(curr == nullptr) {
        vector<StreetSegment> newVec;
        newVec.push_back(s1);
        m_map.associate(s1.start, newVec);
    }
    else curr->push_back(s1);
}

// load function, which takes the data from mapdata.txt and
// puts it into the member hashmap of the StreetMap
bool StreetMapImpl::load(string mapFile)
{
    // creating input stream for file input and checking file validity
    ifstream infile(mapFile);
    if(!infile) return false;
    string name;
    // loop to go through the mapdata.txt file while there are still lines
    while (getline(infile, name))
    {
        // pick out number of GeoCoord pairs for each street
        string num("");
        getline(infile, num);
        if(num == "") break;
        int numSegments = stoi(num);
        // loop through the number of GeoCoords and put them in the map
        for(int i = 0;i < numSegments;i++) {
            // starting and ending coordinate variables
            string start_lat, start_lon, end_lat, end_lon;
            // pipe into variables from input file
            infile >> start_lat;
            infile >> start_lon;
            infile >> end_lat;
            infile >> end_lon;
            // create starting and ending segments for both ways
            GeoCoord g1(start_lat, start_lon), g2(end_lat, end_lon);
            StreetSegment s1(g1, g2, name), s2(g2, g1, name);
            // insert segments into map using insertSegment()
            insertSegment(s1);
            insertSegment(s2);
        }
        infile.ignore(10000, '\n');
    }
    return true;
}

// member function that looks through the member ExpandableHashMap containing
// a key-value pair of GeoCoords to vector<StreetSegment>
bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    // find the vector in the map and return true if it is there
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


