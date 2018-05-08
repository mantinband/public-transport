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
public:
    const Neighbors *getNeighbors() const;
    static const int NUM_OF_TRANSPORT_OPTIONS = 4;
private:
    Neighbors neighbors[NUM_OF_TRANSPORT_OPTIONS];

    string name;


public:
    explicit Station(string name);
    Station(Station &rhs);
    Station& operator=(Station &&rhs);
    virtual ~Station() = default;
    bool hasNeighborStation(const string &stationName) const;
    void setName(const string &name);
    string getName() const;
    void updateConnection(string destinationNode, int duration, int i);
    string getString(const shared_ptr<set<string>> &stationSet) const;
    Neighbors &getNeighborsAt(const int &i) const;
    string getRouteOptions(int i) const;
    void insertNeighborsToSetRec(const shared_ptr<set<string>> &neighborsSet, Neighbors neighbors, int i) const;
    static void addTransportPrefix(string &s, int i);


    enum transportOptions{
        bus,
        tram,
        sprinter,
        rail
    };

    static int getIndexOfTransportForm(const string &s);

};


#endif //OOP_CPP_EX2_STATION_H
