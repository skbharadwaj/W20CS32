#include "CarMap.h"
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;
	CarMap::CarMap() {}
	bool CarMap::addCar(string license) {
		return m_map.insert(license, 0);
	}
	double CarMap::miles(string license) const {
		double value;
		if(!m_map.contains(license)) return -1;
		m_map.get(license, value);
		return value;
	}
	bool CarMap::drive(string license, double distance) {
		if(!m_map.contains(license) || miles(license) < 0) return false;
		m_map.update(license, miles(license) + distance);
		return true;
	}
	int CarMap::fleetSize() const {
		return m_map.size();
	}
	void CarMap::print() const {
		for(int i = 0;i < m_map.size();i++) {
			string k = "";
			double v = 0.0;
			m_map.get(i,k,v);
			cout << k << " " << v << endl;
		}
	}









