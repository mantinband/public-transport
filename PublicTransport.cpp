//
// Created by amichai on 05/05/18.
//

#include <algorithm>
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
            changeTime[0] = stoi(value);
        } else if (field == "tram"){
            changeTime[1] = stoi(value);
        } else if (field == "sprinter"){
            changeTime[2] = stoi(value);
        } else if (field == "rail"){
            changeTime[3] = stoi(value);
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
PublicTransport::PublicTransport() {
    changeTime[0] = 2;
    changeTime[1] = 3;
    changeTime[2] = 4;
    changeTime[3] = 5;
}

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

const vector<shared_ptr<Station>> &PublicTransport::getStationList() const {
    return stationList;
}

string PublicTransport::uniExpressOptions(const string &source,const string &destination) {
    string transportOptions[Station::NUM_OF_TRANSPORT_OPTIONS];
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        vector<shared_ptr<pair<weak_ptr<Station>,int>>> stationVector;

        for (auto station : stationList){
            stationVector.push_back(std::make_shared<pair<weak_ptr<Station>,int>>(station,INT32_MAX));
        }

        int shortestRoute = getShortestRoute(stationVector, source, destination, i);
        transportOptions[i] += shortestRoute == -1 ? "route unavailable" : to_string(shortestRoute);
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

int PublicTransport::getShortestRoute(vector<shared_ptr<pair<weak_ptr<Station>, int>>> stationVector, const string &source, const string &destination,
                                      int i) {
    auto sourceIterator = find_if(stationVector.begin(),stationVector.end(),
                                  [source](shared_ptr<std::pair<weak_ptr<Station>, int>> pair) {
                                      return source == pair->first.lock()->getName();
                                  });
    auto curPair = *sourceIterator;
    stationVector.erase(sourceIterator);
    curPair->second = 0;
    while (!stationVector.empty()) {
        for (auto route : curPair->first.lock()->getNeighborsAt(i).getNeighbors()) {
            auto adjacentPair = *(find_if(stationVector.begin(), stationVector.end(),
                                          [route](shared_ptr<std::pair<weak_ptr<Station>, int>> pair) {
                                              return route.first.lock()->getName() == pair->first.lock()->getName();
                                          }));
            if (adjacentPair->second > curPair->second + route.second + changeTime[i]) {
                adjacentPair->second = curPair->second + route.second + changeTime[i];
            }
        }

        sort(stationVector.begin(),stationVector.end(),[](shared_ptr<std::pair<weak_ptr<Station>, int>> p1,shared_ptr<std::pair<weak_ptr<Station>, int>> p2){
            return p1->second < p2->second;
        });

        curPair = stationVector.at(0);
        if (i == 0) {
            cout << "vector size is" << stationVector.size() << endl;
            cout << "after sort" << endl;
            for (const auto &pair : stationVector) {
                cout << pair->first.lock()->getName() << " " << pair->second << endl;
            }
        }
        stationVector.erase(stationVector.begin());
        if (curPair->first.lock()->getName() == destination){
            if (curPair->second < INT32_MAX){
                return curPair->second-changeTime[i];
            }
            return -1;
        }
    }
    return -1;
}

