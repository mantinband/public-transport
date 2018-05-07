//
// Created by amichai on 05/05/18.
//

#ifndef OOP_CPP_EX2_STATION_H
#define OOP_CPP_EX2_STATION_H
#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <set>
#include "Neighbors.h"
using namespace std;

class Station {
private:
    Neighbors busNeighbors;
    Neighbors tramNeighbors;
    Neighbors sprinterNeighbors;
    Neighbors railNeighbors;
    string name;
public:

    explicit Station(string name);
    Station(Station &rhs);
    Station& operator=(Station &&rhs);
    virtual ~Station() = default;
    bool hasNeighborStation(const string &stationName) const;
    void setName(const string &name);
    string getName() const;
    Neighbors & getBusNeighbors() ;
    Neighbors & getTramNeighbors() ;
    Neighbors & getSprinterNeighbors() ;
    Neighbors & getRailNeighbors() ;
    void updateConnection(const string &destinationNode, const int &duration);

    void insertNeighborsToSetRec(shared_ptr<set<string>> neighborsSet, Neighbors neighbors) const;
    string getBusRouteOptions() const;
    string getTramRouteOptions() const;
    string getSprinterRouteOptions() const;
    string getRailRouteOptions() const;

    string getString(const shared_ptr<set<string>> &stationSet) const;
};


#endif //OOP_CPP_EX2_STATION_H
