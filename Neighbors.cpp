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

    if (route.first.lock() != nullptr) {
        if (route.second > duration) {
            route.second = duration;
            return true;
        }
    }
    return false;
}


void Neighbors::add(const shared_ptr<Station> &toAdd, const int &durationTime) {
    neighbors.emplace_back(toAdd,durationTime);
}

const pair<weak_ptr<Station>, int> & Neighbors::getRoute(const string &stationName) const {
    for (auto i = neighbors.begin(); i != neighbors.end(); i++){
        if (i->first.lock()->getName() == stationName){
            return *i;
        }
    }
    return *(new pair<weak_ptr<Station>,int>());
}

const vector<pair<weak_ptr<Station>, int>> &Neighbors::getNeighbors() const {
    return neighbors;
}

void Neighbors::setNeighbors(const vector<pair<weak_ptr<Station>, int>> &neighbors) {
    Neighbors::neighbors = neighbors;
}
