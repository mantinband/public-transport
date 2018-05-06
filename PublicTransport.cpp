//
// Created by amichai on 05/05/18.
//

#include "PublicTransport.h"


void PublicTransport::load(shared_ptr<ifstream> inputFile,const string &fileName) {
    if (!inputFile->is_open()){
        throw invalidInputFileException();
    }

    string sourceNode;
    string destinationNode;
    string duration;
    char delimiter = '\t';
    while (getline(*inputFile, sourceNode, delimiter) &&
           getline(*inputFile, destinationNode, delimiter) &&
           getline(*inputFile, duration)){
        if (!isInStationList(sourceNode)){
            addStation(sourceNode);
        }
        if (!isInStationList(destinationNode)){
            addStation(destinationNode);
        }
        if (getStation(sourceNode)->hasNeighborStation(destinationNode)){
            getStation(sourceNode)->updateConnection(destinationNode,stoi(duration));
        }else if (fileName.substr(0,3) == "bus"){
            getStation(sourceNode)->addBusConnection(getStation(destinationNode), stoi(duration));
            cout << "added bus connectino from " << sourceNode << " to " << destinationNode << endl;
        } else if (fileName.substr(0,4) == "tram"){
            getStation(sourceNode)->addTramConnection(getStation(destinationNode),stoi(duration));
            cout << "added tram connectino from " << sourceNode << " to " << destinationNode << endl;
        } else if (fileName.substr(0,8) == "sprinter"){
            getStation(sourceNode)->addSprinterConnection(getStation(destinationNode),stoi(duration));
            cout << "added sprinter connectino from " << sourceNode << " to " << destinationNode << endl;
        } else if (fileName.substr(0,4) == "rail"){
            getStation(sourceNode)->addRailConnection(getStation(destinationNode),stoi(duration));
            cout << "added rail connectino from " << sourceNode << " to " << destinationNode << endl;
        } else {
            throw invalidInputFileException();
        }
    }
}


void PublicTransport::configure(shared_ptr<ifstream> configureFile) {
    if (!configureFile->is_open()){
        throw invalidConfigurationFileException();
    }
    string field;
    string value;
    char delimiter = '\t';

    while (getline(*configureFile,field,delimiter) &&
            getline(*configureFile,value)){
        if (field == "intercity"){
            IntercityStation::setChangeTime(stoi(value));
        } else if (field == "stad"){
            StadStation::setChangeTime(stoi(value));
        } else if (field == "central"){
            CentralStation::setChangeTime(stoi(value));
        } else if (field == "bus"){
            busChangeTime = stoi(value);
        } else if (field == "tram"){
            tramChangeTime = stoi(value);
        } else if (field == "sprinter"){
            sprinterChangeTime = stoi(value);
        } else if (field == "rail"){
            railChangeTime = stoi(value);
        } else {
            throw invalidConfigurationFileException();
        }
    }
}

void PublicTransport::setOutputFile(string outputFileName) {
    PublicTransport::outputFileName = std::move(outputFileName);
}

bool PublicTransport::isInStationList(const string &stationName) const {
    for(const auto &station : stationList){
        if (station->getName() == stationName){
            return true;
        }
    }

    return false;
}

void PublicTransport::addStation(const string &stationToAdd) {
    if (stationToAdd.substr(0,2) == INTERCITY_PREFIX){
        stationList.push_back(shared_ptr<Station>(new IntercityStation(stationToAdd)));
    } else if (stationToAdd.substr(0,2) == CENTRAL_PREFIX){
        stationList.push_back(shared_ptr<Station> (new CentralStation(stationToAdd)));
    } else{
        stationList.push_back(shared_ptr<Station>(new StadStation(stationToAdd)));
    }

}

shared_ptr<Station> PublicTransport::getStation(const string &stationToFind) {
    for (auto station : stationList){
        if (station->getName() == stationToFind){
            return station;
        }
    }
    return nullptr;
}

const string PublicTransport::INTERCITY_PREFIX = "IC";
const string PublicTransport::CENTRAL_PREFIX = "CS";
PublicTransport::PublicTransport()
:busChangeTime(2), tramChangeTime(3),sprinterChangeTime(4),railChangeTime(5)
{}

void PublicTransport::printStationList() {
    for (const auto &station: stationList){
        cout << station->getName() << endl;
    }
}

int PublicTransport::getNumberOfStations() {
    return static_cast<int>(stationList.size());
}

void PublicTransport::outboundStations(const string &sourceNode) {
    cout << "bus: " << *(getStation(sourceNode)->getBusNeighbors()) << endl;
    cout << "tram: " << *(getStation(sourceNode)->getTramNeighbors()) << endl;
    cout << "sprinter: " << *(getStation(sourceNode)->getSprinterNeighbors()) << endl;
    cout << "rail: " << *(getStation(sourceNode)->getRailNeighbors()) << endl;
}

void PublicTransport::inboundStations(const string &destinationNode) {

}

