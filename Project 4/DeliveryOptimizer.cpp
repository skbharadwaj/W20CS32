#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    // constructor and destructor
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    // optimization function delcaration
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    // private member streetmap declaration
    const StreetMap* streetmap;
};

// constructor and destructor implementation 
DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    streetmap = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

// optimization functino for the delivery order. This function
// essentially sorts through the array of orders and puts them in an
// order with the shortest distance from the depot appearing first
// and each successive order being the one the shortest distance from that one.
// Works in time O(N^2).
void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    // calculates the old crow distance by addding up the distances
    // from each location to the next
    double oldCrowDist = 0;
    if(deliveries.size() > 0) {
        oldCrowDist += distanceEarthMiles(depot, deliveries[0].location);
        for(int i = 0;i < deliveries.size() - 1;i++) {
            oldCrowDist += distanceEarthMiles(deliveries[i].location
                                                  , deliveries[i+1].location);

        }
        oldCrowDistance = oldCrowDist;
        vector<DeliveryRequest> temp;
        GeoCoord prev = depot;
        // rearranges the locations in a selection sort style algorithm,
        // that successively finds the shortest path from the previous
        // location to one present in the array and swaps it with that one
        for(int i = 0;i < deliveries.size() - 1;i++) {
            double dist = distanceEarthMiles(prev,
                                             deliveries[i].location);
            int whereToSwap = i;
            for(int j = i + 1;j < deliveries.size();j++) {
                double currDist = distanceEarthMiles(prev
                                                     ,deliveries[j].location);
                if(currDist < dist)
                    whereToSwap = j;
            }
            swap(deliveries[i], deliveries[whereToSwap]);
            prev = deliveries[i].location;
        }
        // computes new crow distance by adding up the distances along
        // the new path
        for(int i = 0;i < deliveries.size() - 1;i++) {
            newCrowDistance += distanceEarthMiles(deliveries[i].location
                                                  , deliveries[i+1].location);
        }
        newCrowDistance += 2*distanceEarthMiles(deliveries[deliveries.size() - 1].location
                                              , depot);
    }
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
