//
// Created by amichai on 05/05/18.
//

#include "Station.h"

#include <utility>


Station::Station(string name)
:name(std::move(name))
{}


string Station::getName() const {
    return name;
}


Station::Station(Station &rhs) {
    for (int i=0; i<NUM_OF_TRANSPORT_OPTIONS; i++){
        neighbors[i] = rhs.getNeighborsAt(i);
    }
    name = rhs.getName();
}

Station & Station::operator=(Station &&rhs) {
    for (int i=0; i<NUM_OF_TRANSPORT_OPTIONS; i++){
        neighbors[i] = rhs.getNeighborsAt(i);
    }
    name = rhs.getName();

    return *this;
}

void Station::updateConnection(string destinationNode, int duration, int i) {
    neighbors[i].updateConnection(destinationNode,duration);
}

string Station::getRouteOptions(const int i) const {
    shared_ptr<set<string>> neighborSet(new set<string>());

    auto neighborVector = neighbors[i].getNeighbors();

    for (auto station : neighborVector){
        if (station.first.lock()) {
            station.first.lock()->insertNeighborsToSetRec(neighborSet,station.first.lock()->getNeighborsAt(i),i);
        }
    }
    neighborSet->erase(name);

    string res = getString(neighborSet);
    addTransportPrefix(res,i);
    return res;
}


void Station::insertNeighborsToSetRec(const shared_ptr<set<string>> &neighborsSet, Neighbors neighbors, const int i) const {
    if (neighborsSet->find(name) != neighborsSet->end()){
        return;
    }
    auto neighborVector = neighbors.getNeighbors();

    neighborsSet->insert(name);
    for (auto station : neighborVector){
        if (station.first.lock()) {
            station.first.lock()->insertNeighborsToSetRec(neighborsSet,station.first.lock()->getNeighborsAt(i),i);
        }
    }
}

string Station::getString(const shared_ptr<set<string>> &stationSet) const    {
    string stringResult;
    for (const auto &station : *stationSet) {
        stringResult += station + "\t";
    }
    if (stringResult.empty()){
        stringResult = "no outbound travel";
    }
    return stringResult;
}


Neighbors &Station::getNeighborsAt(const int &i) const {
    return (Neighbors&)neighbors[i];
}

void Station::addTransportPrefix(string &s, int i) {
    switch (i) {
        case bus:
            s = "bus: " + s; break;
        case tram:
            s = "tram: " + s; break;
        case sprinter:
            s = "sprinter: " + s; break;
        case rail:
            s = "rail: " + s; break;
        default:;
    }
}

int Station::getIndexOfTransportForm(const string &s) {
    if (s.find("bus") == 0) return bus;
    if (s.find("tram") == 0) return tram;
    if (s.find("sprinter") == 0) return sprinter;
    if (s.find("rail") == 0) return rail;

    return -1;
}


