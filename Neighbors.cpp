//
// Created by amichai on 07/05/18.
//

#include "Neighbors.h"
#include "Station.h"

bool Neighbors::contains(const string &stationName) const {
    return getRoute(stationName).first.lock() != nullptr;
}

weak_ptr<Station> Neighbors::getStation(const string &stationToFind) const {
    pair<weak_ptr<Station>,int> route = getRoute(stationToFind);

    return route.first.lock() != nullptr  ? route.first : weak_ptr<Station>();
}

bool Neighbors::updateConnection(const string &destinationNode, const int &duration) {
    pair<weak_ptr<Station>,int> route = getRoute(destinationNode);

    for (auto &neighbor : neighbors) {
        if (neighbor.first.lock()->getName() == destinationNode){
            if (neighbor.second > duration){
                neighbor.second = duration;
            }
        }
    }
    return false;
}


void Neighbors::add(const shared_ptr<Station> &toAdd, const int &durationTime) {
    neighbors.emplace_back(toAdd,durationTime);
}

const pair<weak_ptr<Station>, int> & Neighbors::getRoute(const string &stationName) const {
    for (const auto &neighbor : neighbors) {
        if (neighbor.first.lock()->getName() == stationName){
            return neighbor;
        }
    }
    return *(new pair<weak_ptr<Station>,int>());
}

const vector<pair<weak_ptr<Station>, int>> &Neighbors::getNeighbors() const {
    return neighbors;
}

bool Neighbors::searchStationRecAt(const string &destination, shared_ptr<set<string>> visitedStations, const int i) const {
    for (auto station : neighbors){
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

string Neighbors::getConnections() const {
    string res;

    for (auto connection : neighbors){
        res += connection.first.lock()->getName() + "(" + to_string(connection.second) + ") ";
    }

    return res;
}

Neighbors &Neighbors::operator=(const Neighbors &rhs) {
    neighbors = rhs.getNeighbors();
    return *this;
}

Neighbors::Neighbors(Neighbors &&rhs) noexcept {
    neighbors = rhs.getNeighbors();
    rhs.setNeighbors(vector<pair<weak_ptr<Station>,int>>());
}

void Neighbors::setNeighbors(const vector<pair<weak_ptr<Station>, int>> neighbors) {
    Neighbors::neighbors = neighbors;
}

Neighbors &Neighbors::operator=(Neighbors &&rhs) noexcept {
    neighbors = rhs.getNeighbors();
    rhs.setNeighbors(vector<pair<weak_ptr<Station>,int>>());
    return *this;
}

Neighbors::Neighbors() = default;

Neighbors::~Neighbors() = default;

Neighbors::Neighbors(const Neighbors &rhs) = default;

