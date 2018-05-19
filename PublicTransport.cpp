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
    configureFile->close();
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
const string PublicTransport::DEFAULT_OUTPUT_FILE_NAME = "output.dat";

PublicTransport::PublicTransport() {
    outputFileName = DEFAULT_OUTPUT_FILE_NAME;
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

int PublicTransport::getShortestUniRoute(vector<shared_ptr<pair<weak_ptr<Station>, int>>> stationVector,
                                         const string &source, const string &destination,
                                         int i) {
    //get iterator to source node
    auto sourceIterator = find_if(stationVector.begin(),stationVector.end(),
                                  [source](shared_ptr<std::pair<weak_ptr<Station>, int>> pair) {
                                      return source == pair->first.lock()->getName();
                                  });
    auto curPair = *sourceIterator;
    //remove source node and set distance from source to 0
    stationVector.erase(sourceIterator);
    curPair->second = 0;


    //while vector still has unvisited nodes
    while (!stationVector.empty()) {
        //check all nodes adjacent to current node and update distance if a shorter path is found
        for (auto route : curPair->first.lock()->getNeighborsAt(i).getNeighbors()) {
            auto adjacentPair = find_if(stationVector.begin(), stationVector.end(),
                                          [route](shared_ptr<std::pair<weak_ptr<Station>, int>> pair) {
                                              return route.first.lock()->getName() == pair->first.lock()->getName();
                                          });

            //when calculating distance, change time at station is taken into account
            if (adjacentPair != stationVector.end()) {
                if ((*adjacentPair)->second > curPair->second + route.second + changeTime[i]) {
                    (*adjacentPair)->second = curPair->second + route.second + changeTime[i];
                }
            }
        }

        //reorganize vector by shortest distance first
        sort(stationVector.begin(),stationVector.end(),[](shared_ptr<std::pair<weak_ptr<Station>, int>> p1,shared_ptr<std::pair<weak_ptr<Station>, int>> p2){
            return p1->second < p2->second;
        });

        //current pair is updated to be the closest to the source node
        curPair = stationVector.at(0);

        //if closest station hasn't been visited yet, it isn't reachable
        if (curPair->second == INT32_MAX){
            return -1;
        }

        //closest node will not be visited again, hence- remove current node from vector
        stationVector.erase(stationVector.begin());

        //if we have reached the destination, and it has been marked
        //as visited (meaning its value is smaller than MAX_INT it's distance is returned
        if (curPair->first.lock()->getName() == destination){
            if (curPair->second < INT32_MAX){
                return curPair->second-changeTime[i];
            }
            return -1;
        }
    }
    return -1;
}

string PublicTransport::uniExpressOptions(const string &source,const string &destination) {
    string transportOptions[Station::NUM_OF_TRANSPORT_OPTIONS];
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        vector<shared_ptr<pair<weak_ptr<Station>,int>>> stationVector;

        for (auto station : stationList){
            //add node to vector and set as unvisited (i.e. distance from source node = MAX_INT)
            stationVector.push_back(std::make_shared<pair<weak_ptr<Station>,int>>(station,INT32_MAX));
        }

        int shortestRoute = getShortestUniRoute(stationVector, source, destination, i);
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

string PublicTransport::multiExpressOptions(const string &source, const string &destination) {
    string transportOptions[Station::NUM_OF_TRANSPORT_OPTIONS];
    vector<shared_ptr<pair<weak_ptr<Station>,pair<int,int>>>> stationVector;

    //a node is defined as following: station, distance from source, incoming transport type
    for (auto station : stationList){
        //add node to vector and set as unvisited (i.e. distance from source node = MAX_INT) and
        //incoming transport type to none (i.e. -1)
        stationVector.push_back(std::make_shared<pair<weak_ptr<Station>,pair<int,int>>>(station,pair<int,int>(INT32_MAX,-1)));
    }
    int shortestRouteTime = getShortestMultiRoute(stationVector,source,destination);
    return shortestRouteTime == -1 ? "route unavailable" : to_string(shortestRouteTime);
}

int PublicTransport::getShortestMultiRoute(vector<shared_ptr<pair<weak_ptr<Station>,pair<int,int>>>> stationVector,
                                           const string &source, const string &destination) {
    //get iterator to source node
    auto sourceIterator = find_if(stationVector.begin(),stationVector.end(),
                                  [source](shared_ptr<pair<weak_ptr<Station>,pair<int,int>>> pair) {
                                      return source == pair->first.lock()->getName();
                                  });
    auto curPair = *sourceIterator;

    //remove source node and set distance from source to 0
    stationVector.erase(sourceIterator);
    curPair->second.first = 0;

    //while vector still has unvisited nodes
    while (!stationVector.empty()) {
        //check all nodes adjacent to current node and update distance if a shorter path is found
        //route is a pair that contains <adjacent station, travel time>
        for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++) {
            for (auto route : curPair->first.lock()->getNeighborsAt(i).getNeighbors()) {
                //exctract adjacent node from station vector
                auto adjacentPair = find_if(stationVector.begin(), stationVector.end(),
                                            [route](shared_ptr<std::pair<weak_ptr<Station>,std::pair<int,int>>> pair) {
                                                return route.first.lock()->getName() == pair->first.lock()->getName();
                                            });

                //if pair was found in vector
                if (adjacentPair != stationVector.end()) {
                    //delay time depends on whether or not transport type to next node is the same as to current node
                    int delayTime = delayTimeAtStation(*adjacentPair, curPair, i);
                    if ((*adjacentPair)->second.first > curPair->second.first + route.second + delayTime) {
                        (*adjacentPair)->second.first = curPair->second.first + route.second + delayTime;
                    }
                }
            }
        }
        //reorganize vector by shortest distance first
        sort(stationVector.begin(),stationVector.end(),[](shared_ptr<std::pair<weak_ptr<Station>, std::pair<int,int>>> p1,shared_ptr<std::pair<weak_ptr<Station>, std::pair<int,int>>> p2){
            return p1->second.first < p2->second.first;
        });
        //current pair is updated to be the closest to the source node
        curPair = stationVector.at(0);
        //if closest station hasn't been visited yet, it isn't reachable
        if (curPair->second.first == INT32_MAX){
            return -1;
        }

        //closest node will not be updated again, hence- remove current node from vector
        stationVector.erase(stationVector.begin());

        //if we have reached the destination, and it has been marked
        //as visited (meaning its value is smaller than MAX_INT it's distance is returned
        if (curPair->first.lock()->getName() == destination){
            if (curPair->second.first < INT32_MAX){
                return curPair->second.first-changeTime[curPair->second.second];
            }
            return -1;
        }
    }
    return -1;
}
/*
 * @input: current station & next station
 * @output: delay time at station
 *
 * if current transport type to next station
 * is the same as to current station, then
 * wait time for current transport type is
 * returned. (i.e. bus:2, tram:3, sprinter:4, rail:5
 *
 * if not, returned value is the time taken to
 * switch between transport types at current station
 * (i.e. central: 5, stad: 10, intercity: 15,
 * * */
int PublicTransport::delayTimeAtStation(shared_ptr<pair<weak_ptr<Station>, pair<int, int>>> &nextStation,
                                        shared_ptr<pair<weak_ptr<Station>, pair<int, int>>> currentStation, int i) {

    //if current node is source node
    if (currentStation->second.second == -1){
        nextStation->second.second = i;
        return changeTime[i];
    }
    //if next node hasn't been visited yet
    if (nextStation->second.second == -1){
        nextStation->second.second = i; //set next stations incoming transport type to current transport type
        if (currentStation->second.second == -1){ //if transport type to current destination is same as current transport type or current node is source node
            return changeTime[i];
        }
    }
    //if transport to current node is different than to next node => add switch time
    if (currentStation->second.second != i){
        nextStation->second.second = i;
        return currentStation->first.lock()->getSwitchTransportTime();
    }
    //else => return current transport wait time
    return changeTime[i];
}

void PublicTransport::printToFile() {
    ofstream outputFile;
    outputFile.open(outputFileName);
    if (!outputFile.is_open()){
        throw invalidOutputFileException();
    }

    for (const auto &station : stationList){
        outputFile << station->getName() << ":" <<  endl;
        for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
            string transportPrefix;
            Station::addTransportPrefix(transportPrefix,i);
            string connections = station->getNeighborsAt(i).getConnections();
            outputFile << transportPrefix;
            if (!connections.empty()){
                outputFile << connections << endl;
            } else {
                outputFile << "no connections" << endl;
            }
        }
        outputFile << "----------------------------------" << endl;
    }
    outputFile.close();
}

PublicTransport::~PublicTransport() = default;
