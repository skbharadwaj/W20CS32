#include "CarMap.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

int main() {    
    CarMap CM;
    CM.addCar("525XHGJ");
    CM.addCar("6DFHJSG");
    assert(CM.miles("6DFHJSG") == 0);
    assert(CM.fleetSize() == 2);
    CM.addCar("6XHF724");
    CM.drive("525XHGJ", 35);
    CM.drive("525XHGJ", 20);
    assert(CM.miles("525XHGJ") == 55);
    CM.addCar("7FHS421");
    assert(CM.fleetSize() == 4);
    CM.addCar("3HGJ295");
    CM.addCar("223X531");
    CarMap newCM(CM);
    newCM.drive("3HGJ295", 400);

    CM.print();
    cout << endl;
    newCM.print();
    newCM = CM;
    cout << endl;
    newCM.print();
    cout << "All tests passed." << endl;
    return 0;
}