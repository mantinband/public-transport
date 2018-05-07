//
// Created by amichai on 05/05/18.
//

#include "Station.h"

#include <utility>

Station::Station(string name)
:name(std::move(name))
{}

bool Station::hasNeighborStation(const string &stationName) const {
    return busNeighbors.contains(stationName) || tramNeighbors.contains(stationName) || sprinterNeighbors.contains(stationName) || railNeighbors.contains(stationName);
}

string Station::getName() const {
    return name;
}


Station::Station(Station &rhs) {
    busNeighbors = rhs.getBusNeighbors();
    tramNeighbors= rhs.getTramNeighbors();
    sprinterNeighbors = rhs.getSprinterNeighbors();
    railNeighbors = rhs.getRailNeighbors();
    name = rhs.getName();
}


void Station::setName(const string &name) {
    Station::name = name;
}

Station & Station::operator=(Station &&rhs) {
    busNeighbors = rhs.getBusNeighbors();
    tramNeighbors = rhs.getTramNeighbors();
    sprinterNeighbors = rhs.getSprinterNeighbors();
    railNeighbors = rhs.getRailNeighbors();
    name = rhs.getName();

    return *this;
}

void Station::updateConnection(const string &destinationNode, const int &duration) {
    if (busNeighbors.updateConnection(destinationNode,duration)){
        return;
    }
    if (tramNeighbors.updateConnection(destinationNode,duration)){
        return;
    }
    if (sprinterNeighbors.updateConnection(destinationNode,duration)){
        return;
    }
    if (railNeighbors.updateConnection(destinationNode,duration)){
        return;
    }
}

string Station::getBusRouteOptions() const {
    shared_ptr<set<string>> neighborSet(new set<string>());

    auto neighborVector = busNeighbors.getNeighbors();

    for (auto station : neighborVector){
        if (station.first.lock()) {
            station.first.lock()->insertNeighborsToSetRec(neighborSet,busNeighbors);
        }
    }
    return getString(neighborSet);
}

string Station::getTramRouteOptions() const {
    shared_ptr<set<string>> neighborSet(new set<string>());

    auto neighborVector = tramNeighbors.getNeighbors();

    for (auto station : neighborVector){
        if (station.first.lock()) {
            station.first.lock()->insertNeighborsToSetRec(neighborSet,tramNeighbors);;
        }
    }
    return getString(neighborSet);
}


string Station::getSprinterRouteOptions() const {
    shared_ptr<set<string>> neighborSet(new set<string>());

    auto neighborVector = sprinterNeighbors.getNeighbors();

    for (auto station : neighborVector){
        if (station.first.lock()) {
            station.first.lock()->insertNeighborsToSetRec(neighborSet,sprinterNeighbors);;
        }
    }
    return getString(neighborSet);
}


string Station::getRailRouteOptions() const {
    shared_ptr<set<string>> neighborSet(new set<string>());

    auto neighborVector = railNeighbors.getNeighbors();

    for (auto station : neighborVector){
        if (station.first.lock()) {
            station.first.lock()->insertNeighborsToSetRec(neighborSet,railNeighbors);;
        }
    }
    return getString(neighborSet);
}


void Station::insertNeighborsToSetRec(shared_ptr<set<string>> neighborsSet, Neighbors neighbors) const {
    if (neighborsSet->find(name) != neighborsSet->end()){
        return;
    }
    auto neighborVector = neighbors.getNeighbors();

    neighborsSet->insert(name);
    for (auto station : neighborVector){
        if (station.first.lock()) {
            station.first.lock()->insertNeighborsToSetRec(neighborsSet, neighbors);
        }
    }
}

Neighbors &Station::getBusNeighbors() {
    return busNeighbors;
}

Neighbors &Station::getTramNeighbors()  {
    return tramNeighbors;
}

Neighbors &Station::getSprinterNeighbors() {
    return sprinterNeighbors;
}

Neighbors &Station::getRailNeighbors()  {
    return railNeighbors;
}

string Station::getString(const shared_ptr<set<string>> &stationSet) const    {
    string stringResult;
    for (const auto &station : *stationSet) {
        stringResult += station + " ";
    }
    if (stringResult.empty()){
        stringResult = "no outbound travel";
    }
    return stringResult;
}