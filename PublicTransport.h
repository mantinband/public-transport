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

    struct invalidConfigurationFileException : exception {
        const char* what() const throw() override {
            return "ERROR opening configuration file.";
        }
    };

    void printStationList();
    int getNumberOfStations();
    string outboundStations(const string &sourceNode);
    string inboundStations(const string &destinationNode);
private:
    shared_ptr<Station> getStation(const string &stationToFind);

    vector<shared_ptr<Station>> stationList;
    void addStation(const string &stationToAdd);
    int busChangeTime;
    int tramChangeTime;
    int sprinterChangeTime;
    int railChangeTime;
    string outputFileName;
};


#endif //OOP_CPP_EX2_PUBLICTRANSPORT_H
