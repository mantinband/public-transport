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
           getline(*inputFile, duration)) {
        if (!isInStationList(sourceNode)) {
            addStation(sourceNode);
        }
        if (!isInStationList(destinationNode)) {
            addStation(destinationNode);
        }

        int i = Station::getIndexOfTransportForm(fileName);

        if (getStation(sourceNode)->getNeighborsAt(i).contains(destinationNode)) {
            getStation(sourceNode)->updateConnection(destinationNode, stoi(duration), i);
        } else {
            getStation(sourceNode)->getNeighborsAt(i).add(getStation(destinationNode), stoi(duration));
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

    for (const auto &station : stationList){
        cout << station->getName() << ":" <<  endl;
        for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
            string transportPrefix;
            Station::addTransportPrefix(transportPrefix,i);
            string connections = station->getNeighborsAt(i).getConnections();
            cout << transportPrefix;
            if (!connections.empty()){
                cout << connections << endl;
            } else {
                cout << "no connections" << endl;
            }
        }
        cout << "----------------------------------" << endl;
    }
}

int PublicTransport::getNumberOfStations() {
    return static_cast<int>(stationList.size());
}

string PublicTransport::outboundStations(const string &sourceNode) {
    string res;
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        if (i != Station::NUM_OF_TRANSPORT_OPTIONS-1) {
            res += getStation(sourceNode)->getRouteOptions(i) + "\n";
        } else {
            res += getStation(sourceNode)->getRouteOptions(i);
        }
    }
    return res;
}

string PublicTransport::inboundStations(const string &destinationNode) {
    string transportOptions[Station::NUM_OF_TRANSPORT_OPTIONS];

    for (const auto &station: stationList){
        if (station->getName() != destinationNode){
            for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
                if (station->getNeighborsAt(i).searchStationRecAt(destinationNode, make_shared<set<string>>(),i)){
                    transportOptions[i] += station->getName() + "\t";
                }
            }
        }
    }

    for (auto &routeOption : transportOptions){
        if (routeOption.empty()){
            routeOption = "no outbound travel";
        }
    }

    string res;

    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        Station::addTransportPrefix(transportOptions[i],i);
        if (i != Station::NUM_OF_TRANSPORT_OPTIONS-1){
            res +=transportOptions[i] + "\n";
        } else {
            res += transportOptions[i];
        }
    }

    return res;
}

