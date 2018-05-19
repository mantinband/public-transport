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
    Station& operator=(Station &&rhs);
    virtual ~Station() = default;       //d'tor

    virtual StationTypes getStationType() const = 0;    //returns central, intercity or stad
    virtual int getSwitchTransportTime() const = 0;     //returns time to switch between transport options

    string getName() const;

    /*  returns string containing all stations the can be reached from current station  */
    string getRouteOptions(int i) const;

    /*  returns string containing all strings in given set  */
    string getString(const shared_ptr<set<string>> &stationSet) const;

    /*  returns reference to neighbor list with given transport option
     * (i.e. all stations adjacent to station by chosen transport option    */
    Neighbors &getNeighborsAt(const int &i) const;

    /*  if given duration from current station to destination
     * station with given transport option is shorter than current
     * duration to destination, travel time is updated to be new duration   */
    void updateConnection(string destinationNode, int duration, int i);

    /*  used by getRouteOptions(). inserts adjacent neighbors to set,
     *  then repeats action for adjacent neighbors */
    void insertNeighborsToSetRec(const shared_ptr<set<string>> &neighborsSet, Neighbors neighbors, int i) const;

    /*  adds transport option to given string (for example
     *  : addTransportPrefix("15",0), returns "bus: 15" */
    static void addTransportPrefix(string &s, int i);

    /*  returns index of given transport string name. returns -1 if no match found   */
    static int getIndexOfTransportForm(const string &s);

private:
    /*  each array spot contains list of stations
     *  that can be reached by specific transport option
     *  neighbors[0] -> list of adjacent nodes by bus
     *  neighbors[1] -> list of adjacent nodes by tram
     *  neighbors[2] -> list of adjacent nodes by sprinter
     *  neighbors[3] -> list of adjacent nodes by rail
     *  each list contains pairs of station and travel time */
    Neighbors neighbors[NUM_OF_TRANSPORT_OPTIONS];

    string name;    //station name
};


#endif //OOP_CPP_EX2_STATION_H
