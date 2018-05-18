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
    PublicTransport();
    static const string INTERCITY_PREFIX;
    static const string CENTRAL_PREFIX;
    void configure(std::shared_ptr<ifstream> configureFile);
    void setOutputFile(string outputFileName);
    void load(shared_ptr<ifstream> inputFile, const string &fileName);
    bool isInStationList(const string &stationName) const;
    struct invalidInputFileException : exception {
        const char* what() const throw() override {
            return "ERROR opening the specified file.";
        }
    };

    const vector<shared_ptr<Station>> &getStationList() const;

    struct invalidConfigurationFileException : exception {
        const char* what() const throw() override {
            return "ERROR opening configuration file.";
        }
    };

    void printStationList();
    int getNumberOfStations();
    string outboundStations(const string &sourceNode);
    string inboundStations(const string &destinationNode);


    string uniExpressOptions(const string &source, const string &destination);
    int multiExpressOptions(const string &source, const string &destination);
private:

    shared_ptr<Station> getStation(const string &stationToFind);
    vector<shared_ptr<Station>> stationList;
    void addStation(const string &stationToAdd);
    int changeTime[Station::NUM_OF_TRANSPORT_OPTIONS];
    string outputFileName;

    int getShortestUniRoute(vector<shared_ptr<pair<weak_ptr<Station>, int>>> source,
                            const string &destination, const string &i,
                            int i1);

    int getShortestMultiRoute(vector<shared_ptr<pair<weak_ptr<Station>,pair<int,int>>>> stationVector,
                                               const string &source, const string &destination);

    int delayTimeAtStation(shared_ptr<pair<weak_ptr<Station>, pair<int, int>>> &nextStation,
                               shared_ptr<pair<weak_ptr<Station>, pair<int, int>>> currentStation, int i);

    enum stationTypes{
        central,
        intercity,
        stad
    };
};


#endif //OOP_CPP_EX2_PUBLICTRANSPORT_H
