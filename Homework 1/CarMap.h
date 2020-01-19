#ifndef CARMAP_H
#define CARMAP_H
#include "Map.h"
using namespace std;

class CarMap {
	public:
		CarMap();
		CarMap(CarMap& other);
		bool addCar(string license);
		double miles(string license) const;
		bool drive(string license, double distance);
		int fleetSize() const;
		void print() const;
		void operator=(const CarMap& other);
	private:
		Map m_map;
};
#endif
