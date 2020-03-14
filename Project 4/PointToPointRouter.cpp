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
    // constructor that takes a StreetMap, and destructor
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    // generates a list of streetsegments that forms the route
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* streetmap;
};

// constructor
PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    streetmap = sm;
}

// destructor
PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

// generatePointToPointRoute() takes a starting and ending coordinate and
// computes a list of StreetSegments between them using a queue based breadth
// first search.
DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{

    // if the start and the end coordinate are equal, just return
    // because you are already at the destination
    if(start == end) {
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }

    // vector of segments to determine whether the given coordinates are valid
    vector<StreetSegment> starts, ends;
    if(!streetmap->getSegmentsThatStartWith(start, starts) ||
       !streetmap->getSegmentsThatStartWith(start, ends)) {
        return BAD_COORD;
    }

    // the following two are the data structures used for this algorithm, the
    // queue of GeoCoords which forms the basis for the breadth first search
    // and the map that maps each coordinate to its parent; that is,
    // the coordinate from which it came
    queue<GeoCoord> coords;
    map<GeoCoord, GeoCoord> path;

    // puts starting coordinate onto the queue and inserts it into the map
    coords.push(start);
    GeoCoord dummy(to_string(1), to_string(1));
    path.insert(pair<GeoCoord, GeoCoord>(start, dummy));
    // loop that goes through the queue while it isn't empty
    while(!coords.empty()) {
        // exploring the current coordinate
        GeoCoord curr = coords.front();
        coords.pop();
        // gets the segments associated with the current coordinate
        vector<StreetSegment> streetSegs;
        vector<GeoCoord> successors;
        streetmap->getSegmentsThatStartWith(curr, streetSegs);
        // checks if the end is reached
        if(curr.latitudeText == end.latitudeText && curr.longitudeText == end.longitudeText) {
            // this block of code is intended to use the map to go through
            // the list of directions and add them into the StreetSegment
            // vector
            GeoCoord current = end;
            GeoCoord next = path[current];
            // loop that follows the parent of each coordinate back until
            // the starting node is reached
            while(current.latitudeText != start.latitudeText &&
            current.longitudeText != start.longitudeText) {
                // gets the segments associated with that node
                // and finds the appropriate streetSegment, and adds it to the
                // route list. Also updates the distance travelled associated
                // with this streetsegment
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
                // increments distance travelled and moves the current node
                // to its parent 
                totalDistanceTravelled += distanceEarthMiles(current, next);
                current = next;
                next = path[next];
            }
            // switches the coordinates streetsegments because they are added in reverse
            // order
            for(list<StreetSegment>::iterator p = route.begin();p != route.end();
                p++) {
                GeoCoord g1 = (*p).start;
                (*p).start = (*p).end;
                (*p).end = g1;
            }
            return DELIVERY_SUCCESS;
        }
        // looks through the vector of StreetSegments and adds neighboring
        // coordinates to the vector of successors (that is, coordinates
        // that come after the current coordinate)
        for(int i = 0;i < streetSegs.size();i++) {
            GeoCoord toAdd = (streetSegs[i].end.latitudeText == curr.latitudeText &&
            streetSegs[i].end.longitudeText == curr.longitudeText) ? streetSegs[i].start
            : streetSegs[i].end;
            if (find(successors.begin(), successors.end(), toAdd) == successors.end()) {
                successors.push_back(toAdd);
            }
        }
        // looks through the map for them and adds them into the queue
        // if they aren't there
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
