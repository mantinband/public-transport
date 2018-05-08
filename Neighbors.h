//
// Created by amichai on 07/05/18.
//

#ifndef OOP_CPP_EX2_ROUTE_H
#define OOP_CPP_EX2_ROUTE_H
#include <iostream>
#include <vector>
#include <memory>
#include <set>

class Station;

using namespace std;

class Neighbors {
public:
    void add(const shared_ptr<Station> &toAdd,const int &durationTime);
    bool contains(const string &stationName) const;
    weak_ptr<Station> getStation(const string &stationToFind) const;
    bool updateConnection(const string &destinationNode, const int &duration);
    const pair<weak_ptr<Station>, int> & getRoute(const string &stationName) const;
    const vector<pair<weak_ptr<Station>, int>> &getNeighbors() const;
    void setNeighbors(const vector<pair<weak_ptr<Station>, int>> &neighbors);
    bool searchStationRecAt(const string &destination, shared_ptr<set<string>> visitedStations, const int i) const;
    string getConnections() const;
private:
    vector<pair<weak_ptr<Station>,int>> neighbors;
};


#endif //OOP_CPP_EX2_ROUTE_H
