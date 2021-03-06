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
#include "AdjacentStations.h"

using namespace std;

class Station {
public:
    static const int NUM_OF_TRANSPORT_OPTIONS = 4;

    enum transportOptions{
        bus,
        tram,
        sprinter,
        rail
    };

    typedef enum stationTypes{
        central,
        intercity,
        stad
    } StationTypes;


    explicit Station(string name);      //c'tor
    Station(Station &rhs);              //copy c'tor
    Station(Station &&rhs) noexcept ;   //move c'tor
    Station& operator=(const Station &rhs);
    Station& operator=(Station &&rhs) noexcept;
    virtual ~Station() = default;       //d'tor

    /*  returns central, intercity or stad  */
    virtual StationTypes getStationType() const = 0;
    virtual int getSwitchTransportTime() const = 0;     //returns time to switch between transport options

    string getName() const;

    void setNeighborsAt(int i, AdjacentStations n);

    /*  returns string containing all stations the can be reached from current station  */
    string getRouteOptions(int i) const;

    /*  returns string containing all strings in given set  */
    string getString(const shared_ptr<set<string>> &stationSet) const;

    /*  returns reference to neighbor list with given transport option
     * (i.e. all stations adjacent to station by chosen transport option    */
    AdjacentStations &getNeighborsAt(const int &i) const;

    /*  if given duration from current station to destination
     * station with given transport option is shorter than current
     * duration to destination, travel time is updated to be new duration   */
    void updateConnection(string destinationNode, int duration, int i);

    /*  used by getRouteOptions(). inserts adjacent adjacentNeighborVector to set,
     *  then repeats action for adjacent adjacentNeighborVector */
    void insertNeighborsToSetRec(const shared_ptr<set<string>> &neighborsSet, AdjacentStations neighbors, int i) const;

    /*  adds transport option to given string (for example
     *  : addTransportPrefix("15",0), returns "bus: 15" */
    static void addTransportPrefix(string &s, int i);

    /*  returns index of given transport string name. returns -1 if no match found   */
    static int getIndexOfTransportForm(const string &s);

private:
    /*  each array spot contains list of stations
     *  that can be reached by specific transport option
     *  adjacentNeighborVector[0] -> list of adjacent nodes by bus
     *  adjacentNeighborVector[1] -> list of adjacent nodes by tram
     *  adjacentNeighborVector[2] -> list of adjacent nodes by sprinter
     *  adjacentNeighborVector[3] -> list of adjacent nodes by rail
     *  each list contains pairs of station and travel time */
    AdjacentStations neighbors[NUM_OF_TRANSPORT_OPTIONS];

    string name;    //station name
};


#endif //OOP_CPP_EX2_STATION_H
