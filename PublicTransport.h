//
// Created by amichai on 05/05/18.
//

#ifndef OOP_CPP_EX2_PUBLICTRANSPORT_H
#define OOP_CPP_EX2_PUBLICTRANSPORT_H

#include <fstream>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include "Station.h"
#include "IntercityStation.h"
#include "CentralStation.h"
#include "StadStation.h"

using namespace std;

class PublicTransport {

public:
    static const string INTERCITY_PREFIX;
    static const string CENTRAL_PREFIX;
    static const string DEFAULT_OUTPUT_FILE_NAME;

    PublicTransport();      //c'tor
    PublicTransport(const PublicTransport &rhs) = default;  //copy c'tor
    PublicTransport(PublicTransport &&rhs) noexcept;        //move c'tor
    PublicTransport& operator=(const PublicTransport& rhs);
    PublicTransport& operator=(PublicTransport&& rhs) noexcept;
    virtual ~PublicTransport();     //d'tor

    int getChangeTime(int i) const;
    const string &getOutputFileName() const;
    const vector<shared_ptr<Station>> &getStationList() const;

    /*  re-configures change time between transport options and wait
     * time at station for transport option according to given file */
    void configure(std::shared_ptr<ifstream> configureFile);

    /*  setter for output file name    */
    void setOutputFile(string outputFileName);

    /*  adds stations to station list and adds connections
     *  between them according to given file    */
    void load(shared_ptr<ifstream> inputFile, const string &fileName);

    /*  returns true in station name is in station list. false otherwise    */
    bool isInStationList(const string &stationName) const;

    /*  returns number of stations in station list  */
    int getNumberOfStations();

    /*  returns string containing names of all
     * stations reachable  from given source node */
    string outboundStations(const string &sourceNode);

    /*  returns string containing names of all
     * from witch destination node is reachable */
    string inboundStations(const string &destinationNode);

    /*  returns shortest duration from source node
     * to destination node via each transport option    */
    string uniExpressOptions(const string &source, const string &destination);

    /*  returns shortest duration from source node to destination
     * node with switching between transport options */
    string multiExpressOptions(const string &source, const string &destination);

    /*  prints all station details to file.
     * format:
     * ----------------------------------
     * <station name>:
        bus: no connections / <station name, duration> <station name, duration> ..
        tram: no connections / <station name, duration> <station name, duration> ..
        sprinter: no connections / <station name,duration> <station name,duration> ..
        rail: no connections / <station name, duration> <station name, duration> ..
        ---------------------------------- */
    void printToFile();


    /* prints station list to standard output. used for debugging */
    void printStationList();


    struct invalidConfigurationFileException : exception {
        const char* what() const throw() override {
            return "ERROR opening configuration file.";
        }
    };

    struct invalidOutputFileException : exception {
        const char* what() const throw() override {
            return "ERROR opening outpue file.";
        }
    };

    struct invalidInputFileException : exception {
        const char* what() const throw() override {
            return "ERROR opening the specified file.";
        }
    };

    void setStationList(const vector<shared_ptr<Station>> &stationList);

private:
    int changeTime[Station::NUM_OF_TRANSPORT_OPTIONS];
    string outputFileName;
    vector<shared_ptr<Station>> stationList;

    /*  adds station to station list    */
    void addStation(const string &stationToAdd);

    /*  returns given station   */
    shared_ptr<Station> getStation(const string &stationToFind);

    /*  used by multiExpress, returns exit node for given station name and transport type   */
    shared_ptr<Station> getExitStation(string stationName, shared_ptr<vector<shared_ptr<Station >>> graph, int i);

    /*  returns transport type prefix (i.e. "Bus", "Tram" etc') */
    string getPrefix(int i);

    /*  used by multiExpress, returns entry node for given station name and transport type   */
    shared_ptr<Station> getEnterStation(string stationName, shared_ptr<vector<shared_ptr<Station>>> graph, int i);

    /*  returns wait time at station for given transport time (i.e. bus: 2, tram:3) etc'    */
    const int &getWaitTime(int i);

     /* @input: graph, source node, destination node, transport type and if change time is taken into account
     *  @output: shortest travel time if found, INT32_MAX if not. */
    int getShortestRoute(shared_ptr<vector<shared_ptr<pair<weak_ptr<Station>, int>>>> stationsVector,
                         const string &source, const string &destination, int i,
                         bool useChangeTime);
};


#endif //OOP_CPP_EX2_PUBLICTRANSPORT_H
