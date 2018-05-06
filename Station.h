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

using namespace std;

class Station {
private:
    vector<pair<weak_ptr<Station>,int>> busNeighborStations;
    vector<pair<weak_ptr<Station>,int>> tramNeighborStations;
    vector<pair<weak_ptr<Station>,int>> sprinterNeighborStations;
    vector<pair<weak_ptr<Station>,int>> railNeighborStations;
    string name;

public:
    explicit Station(string name);
    Station(Station &&rhs);
    Station(const Station &rhs);
    Station& operator=(Station &&rhs);
    virtual ~Station() = default;

    bool hasNeighborStation(const string &stationName) const;

    void addBusConnection(const shared_ptr<Station> &toAdd,const int &durationTime);
    void addTramConnection(const shared_ptr<Station> &toAdd,const int &durationTime);
    void addSprinterConnection(const shared_ptr<Station> &toAdd,const int &durationTime);
    void addRailConnection(const shared_ptr<Station> &toAdd,const int &durationTime);

    weak_ptr<Station> getBusStation(const string &stationToFind) const;

    weak_ptr<Station> getTramStation(const string &stationToFind) const;

    weak_ptr<Station> getSprinterStation(const string &stationToFind) const;

    weak_ptr<Station> getRailStation(const string &stationToFind) const;

    const vector<pair<weak_ptr<Station>, int>> &getBusNeighborStations() const;

    const vector<pair<weak_ptr<Station>, int>> &getRailNeighborStations() const;

    const vector<pair<weak_ptr<Station>, int>> &getTramNeighborStations() const;

    const vector<pair<weak_ptr<Station>, int>> &getSprinterNeighborStations() const;

    void setBusNeighborStations(const vector<pair<weak_ptr<Station>, int>> &busNeighborStations);

    void setTramNeighborStations(const vector<pair<weak_ptr<Station>, int>> &tramNeighborStations);

    void setSprinterNeighborStations(const vector<pair<weak_ptr<Station>, int>> &sprinterNeighborStations);

    void setRailNeighborStations(const vector<pair<weak_ptr<Station>, int>> &railNeighborStations);

    void setName(const string &name);

    string getName() const;

    void updateConnection(const string &destinationNode, const int &duration);

    void insertBusNeighborsToSetRec(shared_ptr<set<string>> neighborsSet) const;
    void insertTramNeighborsToSetRec(shared_ptr<set<string>> neighborsSet) const;
    void insertSprinterNeighborsToSetRec(shared_ptr<set<string>> neighborsSet) const;
    void insertRailNeighborsToSetRec(shared_ptr<set<string>> neighborsSet) const;

    shared_ptr<string> getBusNeighbors() const ;
    shared_ptr<string> getTramNeighbors() const ;
    shared_ptr<string> getSprinterNeighbors() const ;
    shared_ptr<string> getRailNeighbors() const ;

    shared_ptr<string> getString(shared_ptr<set<string>> set) const;
};


#endif //OOP_CPP_EX2_STATION_H
