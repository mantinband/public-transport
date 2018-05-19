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
    virtual ~PublicTransport();     //d'tor

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

private:

    int changeTime[Station::NUM_OF_TRANSPORT_OPTIONS];
    string outputFileName;
    vector<shared_ptr<Station>> stationList;
    void addStation(const string &stationToAdd);


    /*  returns shortest duration from source node
     * to destination node via chosen transport option    */
    int getShortestUniRoute(vector<shared_ptr<pair<weak_ptr<Station>, int>>> stationVector,
                            const string &source, const string &destination,
                            int i);

    /*  returns shortest duration from source node to destination
     * node with switching between transport options */
    int getShortestMultiRoute(vector<shared_ptr<pair<weak_ptr<Station>,pair<int,int>>>> stationVector,
                                               const string &source, const string &destination);

    /* returns the delay at current station.
     * if current transport type to next station
     * is the same as to current station, then
     * wait time for current transport type is
     * returned. (i.e. bus:2, tram:3, sprinter:4, rail:5
     *
     * if not, returned value is the time taken to
     * switch between transport types at current station
     * (i.e. central: 5, stad: 10, intercity: 15,
     * * */
    int delayTimeAtStation(shared_ptr<pair<weak_ptr<Station>, pair<int, int>>> &nextStation,
                               shared_ptr<pair<weak_ptr<Station>, pair<int, int>>> currentStation, int i);

    /*  returns given station   */
    shared_ptr<Station> getStation(const string &stationToFind);

};


#endif //OOP_CPP_EX2_PUBLICTRANSPORT_H
