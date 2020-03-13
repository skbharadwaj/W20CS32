#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* streetmap;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    streetmap = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
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
        for(int i = 0;i < deliveries.size() - 1;i++) {
            newCrowDistance += distanceEarthMiles(deliveries[i].location
                                                  , deliveries[i+1].location);
        }
        newCrowDistance += distanceEarthMiles(deliveries[deliveries.size() - 1].location
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
