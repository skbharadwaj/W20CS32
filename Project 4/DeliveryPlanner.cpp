#include "provided.h"
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* streetmap;
    string getDirectionBetweenTwoCoordinates(GeoCoord g1, GeoCoord g2) const;
    string getTurnDirection(StreetSegment s1, StreetSegment s2) const;
};

string DeliveryPlannerImpl::getTurnDirection(StreetSegment s1, StreetSegment s2) const {
    double result = angleBetween2Lines(s1, s2);
    if(result < 1 || result > 359) return "neither";
    else if(result >= 1 && result < 180) return "left";
    else return "right";
}

string DeliveryPlannerImpl::getDirectionBetweenTwoCoordinates(GeoCoord g1, GeoCoord g2)
const {
    double angle = angleOfLine(StreetSegment(g1, g2, "doesn't matter"));
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


DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    if(deliveries.size() == 0) return DELIVERY_SUCCESS;
    vector<DeliveryRequest> deliveriesTemp;
    for(int i = 0;i < deliveries.size();i++) deliveriesTemp.push_back(deliveries[i]);
    DeliveryOptimizer delivoptimizer(streetmap);
    double before = 0, after = 0;
    delivoptimizer.optimizeDeliveryOrder(depot, deliveriesTemp, before,
                                         after);

    GeoCoord curr = depot;
    PointToPointRouter ppr(streetmap);
    deliveriesTemp.push_back(DeliveryRequest("END", depot));
    for(int i = 0;i < deliveriesTemp.size();i++) {
        string dr_item = deliveriesTemp[i].item;
        GeoCoord dr_gc = deliveriesTemp[i].location;
        list<StreetSegment> route;
        double tempDistanceTravelled = 0;
        DeliveryResult delivResult1 = ppr.generatePointToPointRoute(curr, dr_gc, route,
                                                                    tempDistanceTravelled);
        totalDistanceTravelled += tempDistanceTravelled;
        if(delivResult1 != DELIVERY_SUCCESS) return delivResult1;
        list<StreetSegment>::iterator p = route.begin();
        StreetSegment firstSegment = *p;
        DeliveryCommand d1;
        d1.initAsProceedCommand
        (getDirectionBetweenTwoCoordinates(curr, firstSegment.end),
                                firstSegment.name,
                                distanceEarthMiles(curr, firstSegment.end));
        commands.push_back(d1);
        p++;
        StreetSegment prevSegment = firstSegment;
        for(;p != route.end();p++) {
            StreetSegment currSegment = *p;
            if(prevSegment.name == currSegment.name) {
                commands[commands.size() - 1]
                .increaseDistance(distanceEarthMiles(currSegment.start, currSegment.end));
            }
            else {
                string turnDir;
                turnDir = getTurnDirection(prevSegment, currSegment);
                DeliveryCommand toTurn;
                if(turnDir == "left") {
                    toTurn.initAsTurnCommand("left", currSegment.name);
                    commands.push_back(toTurn);
                }
                else if(turnDir == "right") {
                    toTurn.initAsTurnCommand("right", currSegment.name);
                    commands.push_back(toTurn);
                }
                toTurn.initAsProceedCommand(
                getDirectionBetweenTwoCoordinates(prevSegment.end, currSegment.end),
                currSegment.name, distanceEarthMiles(currSegment.start, currSegment.end));
                commands.push_back(toTurn);
            }
            prevSegment = currSegment;
        }

        if(deliveriesTemp[i].item == "END") return DELIVERY_SUCCESS;
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
