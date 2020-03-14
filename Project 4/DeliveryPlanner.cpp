#include "provided.h"
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class DeliveryPlannerImpl
{
public:
    // constructor and destructor
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    // generates the delivery plan using the vector of delivieries
    // and puts them into the vector of commands, as well as noting
    // the total distance travelled
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    // member streetmap and helper functions to get directions
    const StreetMap* streetmap;
    string getDirectionBetweenTwoCoordinates(GeoCoord g1, GeoCoord g2) const;
    string getTurnDirection(StreetSegment s1, StreetSegment s2) const;
};

// helper function to get the direction between two Segments, to know which
// way to turn. makes use of the anglebetween2lines method given
string DeliveryPlannerImpl::getTurnDirection(StreetSegment s1, StreetSegment s2) const {
    // gets the angle between the segments
    double result = angleBetween2Lines(s1, s2);
    // maps this to turn directions and returns the appropriate directions
    if(result < 1 || result > 359) return "neither";
    else if(result >= 1 && result < 180) return "left";
    else return "right";
}

// helper methods that find the direction between coordinates
string DeliveryPlannerImpl::getDirectionBetweenTwoCoordinates(GeoCoord g1, GeoCoord g2)
const {
    // gets the angle using a helper function provided
    double angle = angleOfLine(StreetSegment(g1, g2, "doesn't matter"));
    // maps the angle to the appropriate direction and returns the
    // string containing the appropriate text
    if(angle >= 0 && angle < 22.5) return "east";
    else if(angle >= 22.5 && angle < 67.5) return "northeast";
    else if(angle >= 67.5 && angle < 112.5) return "north";
    else if(angle >= 112.5 && angle < 157.5) return "northwest";
    else if(angle >= 157.5 && angle < 202.5) return "west";
    else if(angle >= 202.5 && angle < 247.5) return "southwest";
    else if(angle >= 247.5 && angle < 292.5) return "south";
    else if(angle >= 292.5 && angle < 337.5) return "southeast";
    else return "east";
}

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    streetmap = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

// implementation of the generateDeliveryPlan() function. The function
// takes the coordinate of the depot and a vector of DeliveryRequests,
// and produces the vector of delivery commands to get to each of the
// location. Uses the DeliveryOptimizer class to generate a more efficient
// route
DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    // return if there is nothing to deliver
    if(deliveries.size() == 0) return DELIVERY_SUCCESS;
    // temporary vector to use the optimizer class, because it is declared const
    vector<DeliveryRequest> deliveriesTemp;
    for(int i = 0;i < deliveries.size();i++) deliveriesTemp.push_back(deliveries[i]);
    DeliveryOptimizer delivoptimizer(streetmap);
    double before = 0, after = 0;
    delivoptimizer.optimizeDeliveryOrder(depot, deliveriesTemp, before,
                                         after);
    // create the current coordinate a the depot and a PointToPointRouter instance
    GeoCoord curr = depot;
    PointToPointRouter ppr(streetmap);
    // add the depot as the last delivery location
    deliveriesTemp.push_back(DeliveryRequest("END", depot));
    // for each item in the deliveries list
    for(int i = 0;i < deliveriesTemp.size();i++) {
        // gets both attributes of the item
        string dr_item = deliveriesTemp[i].item;
        GeoCoord dr_gc = deliveriesTemp[i].location;
        // gets the route from the current point to the next point and
        // stores it in the temporary list
        list<StreetSegment> route;
        double tempDistanceTravelled = 0;
        DeliveryResult delivResult1 = ppr.generatePointToPointRoute(curr, dr_gc, route,
                                                                    tempDistanceTravelled);
        // adds the distance on this route to the total distance
        totalDistanceTravelled += tempDistanceTravelled;
        // check if the locations are valid and return if they aren't
        if(delivResult1 != DELIVERY_SUCCESS) return delivResult1;
        // initialize the first command of the loop as a proceed command
        // as noted by the spec (after a proceed or turn or delivery) and
        // add it to the vector of commands
        list<StreetSegment>::iterator p = route.begin();
        StreetSegment firstSegment = *p;
        DeliveryCommand d1;
        d1.initAsProceedCommand
        (getDirectionBetweenTwoCoordinates(curr, firstSegment.end),
                                firstSegment.name,
                                distanceEarthMiles(curr, firstSegment.end));
        commands.push_back(d1);
        p++;
        // the current segment is the first segment
        StreetSegment prevSegment = firstSegment;
        // for each segment in the route:
        for(;p != route.end();p++) {
            // if the name of this segment is the same as the previous,
            // simply increase the distance in miles of the previous command
            StreetSegment currSegment = *p;
            if(prevSegment.name == currSegment.name) {
                commands[commands.size() - 1]
                .increaseDistance(distanceEarthMiles(currSegment.start, currSegment.end));
            }
            // otherwise, you are changing streets:
            else {
                // get the turn direction between this street and the next
                string turnDir;
                turnDir = getTurnDirection(prevSegment, currSegment);
                DeliveryCommand toTurn;
                // create a delivery command based on the angle of turning
                // and add it to the vector of commands
                if(turnDir == "left") {
                    toTurn.initAsTurnCommand("left", currSegment.name);
                    commands.push_back(toTurn);
                }
                else if(turnDir == "right") {
                    toTurn.initAsTurnCommand("right", currSegment.name);
                    commands.push_back(toTurn);
                }
                // proceed in the appropriate direction after turning
                toTurn.initAsProceedCommand(
                getDirectionBetweenTwoCoordinates(prevSegment.end, currSegment.end),
                currSegment.name, distanceEarthMiles(currSegment.start, currSegment.end));
                commands.push_back(toTurn);
            }
            // move up the previous segment to the current segment
            // to iterate forward
            prevSegment = currSegment;
        }

        // if the end is reached, return
        if(deliveriesTemp[i].item == "END") return DELIVERY_SUCCESS;
        // otherwise, deliver the item 
        DeliveryCommand finalDelivery;
        finalDelivery.initAsDeliverCommand(deliveriesTemp[i].item);
        commands.push_back(finalDelivery);
        curr = dr_gc;
    }
    cout << "no route";
    return NO_ROUTE;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
