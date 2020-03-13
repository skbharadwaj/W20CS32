#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <queue>
#include <tuple>
#include <unordered_map>

using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* streetmap;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    streetmap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{

    if(start == end) {
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }

    vector<StreetSegment> starts, ends;
    if(!streetmap->getSegmentsThatStartWith(start, starts) ||
       !streetmap->getSegmentsThatStartWith(start, ends)) {
        return BAD_COORD;
    }

    queue<GeoCoord> coords;
    map<GeoCoord, GeoCoord> path;
    coords.push(start);
    GeoCoord dummy(to_string(1), to_string(1));
    path.insert(pair<GeoCoord, GeoCoord>(start, dummy));
    while(!coords.empty()) {
        GeoCoord curr = coords.front();
        coords.pop();
        vector<StreetSegment> streetSegs;
        vector<GeoCoord> successors;
        streetmap->getSegmentsThatStartWith(curr, streetSegs);
        if(curr.latitudeText == end.latitudeText && curr.longitudeText == end.longitudeText) {
            GeoCoord current = end;
            GeoCoord next = path[current];
            while(current.latitudeText != start.latitudeText &&
            current.longitudeText != start.longitudeText) {
                vector<StreetSegment> seg;
                streetmap->getSegmentsThatStartWith(current, seg);
                for(int i = 0;i < seg.size();i++) {
                    StreetSegment s = seg[i];
                    if(s.start.latitudeText == current.latitudeText &&
                    s.start.longitudeText == current.longitudeText &&
                       s.end.latitudeText == next.latitudeText &&
                    s.end.longitudeText == next.longitudeText) {
                        route.push_front(s);
                    }
                }
                totalDistanceTravelled += distanceEarthMiles(current, next);
                current = next;
                next = path[next];
            }
            for(list<StreetSegment>::iterator p = route.begin();p != route.end();
                p++) {
                GeoCoord g1 = (*p).start;
                (*p).start = (*p).end;
                (*p).end = g1;
            }
            return DELIVERY_SUCCESS;
        }
        for(int i = 0;i < streetSegs.size();i++) {
            GeoCoord toAdd = (streetSegs[i].end.latitudeText == curr.latitudeText &&
            streetSegs[i].end.longitudeText == curr.longitudeText) ? streetSegs[i].start
            : streetSegs[i].end;
            if (find(successors.begin(), successors.end(), toAdd) == successors.end()) {
                successors.push_back(toAdd);
            }
        }
        for(int i = 0;i < successors.size();i++) {
            if(path.find(successors[i]) == path.end()) {
                coords.push(successors[i]);
                path.insert(pair<GeoCoord, GeoCoord>(successors[i], curr));
            }
        }
    }
     
    return DELIVERY_SUCCESS;

}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
