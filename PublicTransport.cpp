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


string PublicTransport::uniExpressOptions(const string &source,const string &destination) {
    string transportOptions[Station::NUM_OF_TRANSPORT_OPTIONS];
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        shared_ptr<vector<shared_ptr<pair<weak_ptr<Station>,int>>>> stationVector = std::make_shared<vector<shared_ptr<pair<weak_ptr<Station>,int>>>>();

        for (auto station : stationList){
            //add node to vector and set as unvisited (i.e. distance from source node = MAX_INT)
            stationVector->push_back(std::make_shared<pair<weak_ptr<Station>,int>>(station,INT32_MAX));
        }

        int shortestRoute = getShortestRoute(stationVector,
                                             source, destination, i, true);
        transportOptions[i] += shortestRoute == INT32_MAX ? "route unavailable" : to_string(shortestRoute);
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
    shared_ptr<vector<shared_ptr<Station>>> newGraph = std::make_shared<vector<shared_ptr<Station>>>();

    /*  create new graph in which each node is devided to 8 new nodes.
     *  ew graph adds new connection to transport vector at adjacentNeighborVector[0] only
     *  4 entry nodes and 4 exit nodes. while building the graph
     *  connections are added between the entry node to the exit
     *  node with the wait time of transport at station (i.e. bus:2 tram:3 etc'*/
    for (const auto &station : stationList) {
        for (int i=0; i <Station::NUM_OF_TRANSPORT_OPTIONS; i++){
            shared_ptr<Station> enter;
            shared_ptr<Station> exit;

            enter = shared_ptr<Station>(new IntercityStation(station->getName()+"Enter"+getPrefix(i)));
            exit = shared_ptr<Station>(new IntercityStation(station->getName()+"Exit" + getPrefix(i)));

            enter->getNeighborsAt(0).add(exit,getWaitTime(i));
            newGraph->push_back(enter);
            newGraph->push_back(exit);
        }
    }
    /*  connection are added to graph as following :
     *  if current station's name is "a" and i equals 1 then
     *  the station named "aExitStad" is fetched.
     *  if "a" is connected to "b" with a weight of 5, then
     *  "aExitStad" is connected to bEnterStad with a weight of 5
     *  "bEnterStad" is then connected to "bExitBus", "bExitTram",
     *  and "bExitSprinter" with a weight of 15.
     *  ("bEnterStad" is connected to "bExitStad" with a weight of
     *  5 from previous loop)*/
/*
 *
 *              a
 *
 *
 * b0        b1       b2       b3
 * |30
 * *
 * b0        b1       b2       b3
 * */
    for (const auto &station : stationList){
        for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++) {
            for (auto connection : station->getNeighborsAt(i).getNeighbors()){

                shared_ptr<Station> exitOrigin = getExitStation(station->getName(),newGraph,i);
                shared_ptr<Station> enterDestination = getEnterStation(connection.first.lock()->getName(),newGraph,i);
                exitOrigin->getNeighborsAt(0).add(enterDestination,connection.second);
                for (int j=0; j<Station::NUM_OF_TRANSPORT_OPTIONS; j++){
                    if (i != j) {
                        shared_ptr<Station> exitDestination = getExitStation(connection.first.lock()->getName(),
                                                                             newGraph, j);
                        enterDestination->getNeighborsAt(0).add(exitDestination, station->getSwitchTransportTime());
                    }
                }
            }
        }
    }

    shared_ptr<vector<shared_ptr<pair<weak_ptr<Station>,int>>>> newStationsVector =
            std::make_shared<vector<shared_ptr<pair<weak_ptr<Station>,int>>>>();
    /*  create new vector containing all new stations, with initialized distance from source of INT32_MAX   */
    for (auto station : *newGraph){
        newStationsVector->push_back(std::make_shared<pair<weak_ptr<Station>,int>>(station,INT32_MAX));
    }

    int shortestMultiRoute = INT32_MAX;
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        int currentMultiRouteWeight =
                getShortestRoute(newStationsVector,
                                 source + "Exit" + getPrefix(i), destination, 0, false);
        if (currentMultiRouteWeight < shortestMultiRoute){
            shortestMultiRoute = currentMultiRouteWeight;
        }
    }

    return shortestMultiRoute == INT32_MAX ? "route unavailable" : to_string(shortestMultiRoute);
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

PublicTransport &PublicTransport::operator=(const PublicTransport &rhs) {
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        changeTime[i] = rhs.getChangeTime(i);
    }
    outputFileName = rhs.getOutputFileName();
    stationList = rhs.getStationList();
    return *this;
}

int PublicTransport::getChangeTime(int i) const {
    return changeTime[i];
}

const string &PublicTransport::getOutputFileName() const {
    return outputFileName;
}

const vector<shared_ptr<Station>> &PublicTransport::getStationList() const {
    return stationList;
}

PublicTransport::PublicTransport(PublicTransport &&rhs) noexcept{
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        changeTime[i] = rhs.getChangeTime(i);
    }
    stationList = rhs.getStationList();
    rhs.setStationList(vector<shared_ptr<Station>>());
}

void PublicTransport::setStationList(const vector<shared_ptr<Station>> &stationList) {
    PublicTransport::stationList = stationList;
}

PublicTransport &PublicTransport::operator=(PublicTransport &&rhs) noexcept {
    for (int i=0; i<Station::NUM_OF_TRANSPORT_OPTIONS; i++){
        changeTime[i] = rhs.getChangeTime(i);
    }
    outputFileName = rhs.getOutputFileName();
    stationList = rhs.getStationList();
    rhs.setStationList(vector<shared_ptr<Station>>());
    return *this;
}

shared_ptr<Station> PublicTransport::getExitStation(string stationName,
                                                    shared_ptr<vector<shared_ptr<Station >>> graph, int i) {
    for (auto &station : *graph){
        if (station->getName() == stationName + "Exit" + getPrefix(i)){
            return station;
        }
    }

    return shared_ptr<Station>();
}

string PublicTransport::getPrefix(int i){
    if (i == 0) return "Bus";
    if (i == 1) return "Tram";
    if (i == 2) return "Sprinter";
    if (i == 3) return "Rail";
    return "";
}

shared_ptr<Station>
PublicTransport::getEnterStation(string stationName, shared_ptr<vector<shared_ptr<Station>>> graph, int i) {
    for (auto &station : *graph){
        if (station->getName() == stationName + "Enter" + getPrefix(i)){
            return station;
        }
    }
    return shared_ptr<Station>();
}

const int &PublicTransport::getWaitTime(int i) {
    return changeTime[i];
}

int PublicTransport::getShortestRoute(shared_ptr<vector<shared_ptr<pair<weak_ptr<Station>, int>>>> stationVector,
                                      const string &source, const string &destination, int i,
                                      bool useChangeTime) {
    //get iterator to source node
    auto sourceIterator = find_if(stationVector->begin(),stationVector->end(),
                                  [source](shared_ptr<std::pair<weak_ptr<Station>, int>> pair) {
                                      return source == pair->first.lock()->getName();
                                  });
    auto curPair = *sourceIterator;
    //remove source node and set distance from source to 0
    stationVector->erase(sourceIterator);
    curPair->second = 0;


    //while vector still has unvisited nodes
    while (!stationVector->empty()) {
        //check all nodes adjacent to current node and update distance if a shorter path is found
        for (auto route : curPair->first.lock()->getNeighborsAt(i).getNeighbors()) {
            auto adjacentPair = find_if(stationVector->begin(), stationVector->end(),
                                        [route](shared_ptr<std::pair<weak_ptr<Station>, int>> pair) {
                                            return route.first.lock()->getName() == pair->first.lock()->getName();
                                        });


            int distanceWeight = curPair->second + route.second;
            if (useChangeTime) {    //when calculating uniExpress, change time at station is taken into account
                distanceWeight += changeTime[i];
            }
            if (adjacentPair != stationVector->end()) {
                if ((*adjacentPair)->second > distanceWeight){
                    (*adjacentPair)->second  = distanceWeight;
                }
            }
        }

        //reorganize vector by shortest distance first
        sort(stationVector->begin(),stationVector->end(),
             [](shared_ptr<std::pair<weak_ptr<Station>, int>> p1,shared_ptr<std::pair<weak_ptr<Station>, int>> p2){
                 return p1->second < p2->second;
        });

        //current pair is updated to be the closest to the source node
        curPair = stationVector->at(0);

        //if closest station hasn't been visited yet, it isn't reachable
        if (curPair->second == INT32_MAX){
            return INT32_MAX;
        }

        //closest node will not be visited again, hence- remove current node from vector
        stationVector->erase(stationVector->begin());

        //if we have reached the destination, and it has been marked
        //as visited (meaning its value is smaller than MAX_INT it's distance is returned
        if (curPair->first.lock()->getName().find(destination) != string::npos){
            return useChangeTime ? curPair->second-changeTime[i] : curPair->second;
        }
    }
    return INT32_MAX;
}


PublicTransport::~PublicTransport() = default;
