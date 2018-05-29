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

class AdjacentStations {
public:
    AdjacentStations();    //c'tor
    AdjacentStations(const AdjacentStations &rhs);    //copy c'tor
    AdjacentStations(AdjacentStations &&rhs) noexcept; //move c'tor
    AdjacentStations& operator=(const AdjacentStations &rhs);
    AdjacentStations& operator=(AdjacentStations &&rhs) noexcept;
    virtual ~AdjacentStations();       //d'tor

    /*  used by c'tors to set station vector   */
    void setNeighbors(vector<pair<weak_ptr<Station>, int>> neighbors);

    /*  add connection. connection is defined as a pair of
     * <destination station, travel time>   */
    void add(const shared_ptr<Station> &toAdd,const int &durationTime);

    /*  returns true if vector of adjacentNeighborVector contains station */
    bool contains(const string &stationName) const;

    /*  returns station if found, empty weak_ptr otherwise  */
    weak_ptr<Station> getStation(const string &stationToFind) const;

    /*  updated connection to new given duration if shorter than current duration   */
    bool updateConnection(const string &destinationNode, const int &duration);

    /*  returns pair containing station and travel time of given station name   */
    const pair<weak_ptr<Station>, int> & getRoute(const string &stationName) const;

    /*  returns adjacent*/
    const vector<pair<weak_ptr<Station>, int>> &getNeighbors() const;

    /*  returns true if destination station is reachable    */
    bool searchStationRecAt(const string &destination, shared_ptr<set<string>> visitedStations, int i) const;

    /*  returns string containing all adjacent neighbors. string format is station name(duration)   */
    string getConnections() const;
private:
    vector<pair<weak_ptr<Station>,int>> adjacentNeighborVector;
};

#endif //OOP_CPP_EX2_ROUTE_H
