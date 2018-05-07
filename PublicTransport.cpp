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
            getStation(sourceNode)->getBusNeighbors().add(getStation(destinationNode), stoi(duration));
            cout << "added bus connectino from " << sourceNode << " to " << destinationNode << endl;
        } else if (fileName.substr(0,4) == "tram"){
            getStation(sourceNode)->getTramNeighbors().add(getStation(destinationNode),stoi(duration));
            cout << "added tram connectino from " << sourceNode << " to " << destinationNode << endl;
        } else if (fileName.substr(0,8) == "sprinter"){
            getStation(sourceNode)->getSprinterNeighbors().add(getStation(destinationNode),stoi(duration));
            cout << "added sprinter connectino from " << sourceNode << " to " << destinationNode << endl;
        } else if (fileName.substr(0,4) == "rail"){
            getStation(sourceNode)->getRailNeighbors().add(getStation(destinationNode),stoi(duration));
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

string PublicTransport::outboundStations(const string &sourceNode) {
    return "bus: " + getStation(sourceNode)->getBusRouteOptions()
            + "\ntram: " + getStation(sourceNode)->getTramRouteOptions()
            + "\nsprinter: " + getStation(sourceNode)->getSprinterRouteOptions()
            + "\nrail: " + getStation(sourceNode)->getRailRouteOptions();

}

string PublicTransport::inboundStations(const string &destinationNode) {
    string busTransportStations;
    string tramTransportStations;
    string sprinterTransportStations;
    string railTransportStations;

    for (const auto &station : stationList){
        if (station->getName() != destinationNode){
            if (station->getBusNeighbors().searchBusStationRec(destinationNode, make_shared<set<string>>())) {
                busTransportStations += station->getName() + " ";
            }
            if (station->getTramNeighbors().searchTramStationRec(destinationNode,make_shared<set<string>>())) {
                tramTransportStations += station->getName() + " ";
            }
            if (station->getSprinterNeighbors().searchSprinterStationRec(destinationNode,make_shared<set<string>>())) {
                sprinterTransportStations += station->getName() + " ";
            }
            if (station->getRailNeighbors().searchRailStationRec(destinationNode,make_shared<set<string>>())) {
                railTransportStations += station->getName() + " ";
            }
        }
    }

    if (busTransportStations.empty()){
        busTransportStations = "no outbound travel";
    }
    if (sprinterTransportStations.empty()){
        sprinterTransportStations = "no outbound travel";
    }
    if (tramTransportStations.empty()){
        tramTransportStations = "no outbound travel";
    }
    if (railTransportStations.empty()){
        railTransportStations = "no outbound travel";
    }
    return "bus: " + busTransportStations + "\ntram: " + tramTransportStations + "\nsprinter: " + sprinterTransportStations + "\nrail: " + railTransportStations;
}

