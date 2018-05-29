//
// Created by amichai on 07/05/18.
//

#include "AdjacentStations.h"
#include "Station.h"

bool AdjacentStations::contains(const string &stationName) const {
    return getRoute(stationName).first.lock() != nullptr;
}

weak_ptr<Station> AdjacentStations::getStation(const string &stationToFind) const {
    pair<weak_ptr<Station>,int> route = getRoute(stationToFind);

    return route.first.lock() != nullptr  ? route.first : weak_ptr<Station>();
}

bool AdjacentStations::updateConnection(const string &destinationNode, const int &duration) {
    pair<weak_ptr<Station>,int> route = getRoute(destinationNode);

    for (auto &neighbor : adjacentNeighborVector) {
        if (neighbor.first.lock()->getName() == destinationNode){
            if (neighbor.second > duration){
                neighbor.second = duration;
            }
        }
    }
    return false;
}


void AdjacentStations::add(const shared_ptr<Station> &toAdd, const int &durationTime) {
    adjacentNeighborVector.emplace_back(toAdd,durationTime);
}

const pair<weak_ptr<Station>, int> & AdjacentStations::getRoute(const string &stationName) const {
    for (const auto &neighbor : adjacentNeighborVector) {
        if (neighbor.first.lock()->getName() == stationName){
            return neighbor;
        }
    }
    return *(new pair<weak_ptr<Station>,int>());
}

const vector<pair<weak_ptr<Station>, int>> &AdjacentStations::getNeighbors() const {
    return adjacentNeighborVector;
}

bool AdjacentStations::searchStationRecAt(const string &destination, shared_ptr<set<string>> visitedStations, const int i) const {
    for (auto station : adjacentNeighborVector){
        if (station.first.lock()->getName() == destination){
            return true;
        }
        if (visitedStations->find(station.first.lock()->getName()) != visitedStations->end()){
            return false;
        }
        visitedStations->insert(station.first.lock()->getName());
        return station.first.lock()->getNeighborsAt(i).searchStationRecAt(destination,visitedStations,i);
    }
    return false;
}

string AdjacentStations::getConnections() const {
    string res;

    for (auto connection : adjacentNeighborVector){
        res += connection.first.lock()->getName() + "(" + to_string(connection.second) + ") ";
    }

    return res;
}

AdjacentStations &AdjacentStations::operator=(const AdjacentStations &rhs) {
    adjacentNeighborVector = rhs.getNeighbors();
    return *this;
}

AdjacentStations::AdjacentStations(AdjacentStations &&rhs) noexcept {
    adjacentNeighborVector = rhs.getNeighbors();
    rhs.setNeighbors(vector<pair<weak_ptr<Station>,int>>());
}

void AdjacentStations::setNeighbors(const vector<pair<weak_ptr<Station>, int>> neighbors) {
    AdjacentStations::adjacentNeighborVector = neighbors;
}

AdjacentStations &AdjacentStations::operator=(AdjacentStations &&rhs) noexcept {
    adjacentNeighborVector = rhs.getNeighbors();
    rhs.setNeighbors(vector<pair<weak_ptr<Station>,int>>());
    return *this;
}

AdjacentStations::AdjacentStations() = default;

AdjacentStations::~AdjacentStations() = default;

AdjacentStations::AdjacentStations(const AdjacentStations &rhs) = default;

