//
// Created by amichai on 05/05/18.
//

#include "Station.h"

#include <utility>

Station::Station(string name)
:name(std::move(name))
{}

bool Station::hasNeighborStation(const string &stationName) const {
    return getBusStation(stationName).lock() || getTramStation(stationName).lock() || getSprinterStation(stationName).lock() || getRailStation(stationName).lock();
}

string Station::getName() const {
    return name;
}

weak_ptr<Station> Station::getBusStation(const string &stationToFind) const {
    for(auto neighbor : busNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == stationToFind){
                return neighbor.first;
            }
        }
    }
    return weak_ptr<Station>();
}
weak_ptr<Station> Station::getTramStation(const string &stationToFind) const {
    for(auto neighbor : tramNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == stationToFind){
                return neighbor.first;
            }
        }
    }
    return weak_ptr<Station>();
}

weak_ptr<Station> Station::getSprinterStation(const string &stationToFind) const {
    for(auto neighbor : sprinterNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == stationToFind){
                return neighbor.first;
            }
        }
    }
    return weak_ptr<Station>();
}

weak_ptr<Station> Station::getRailStation(const string &stationToFind) const {
    for(auto neighbor : railNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == stationToFind){
                return neighbor.first;
            }
        }
    }
    return weak_ptr<Station>();
}

Station::Station(const Station &rhs) {
    busNeighborStations = rhs.getBusNeighborStations();
    tramNeighborStations = rhs.getTramNeighborStations();
    sprinterNeighborStations = rhs.getSprinterNeighborStations();
    railNeighborStations = rhs.getRailNeighborStations();
    name = rhs.getName();
}


const vector<pair<weak_ptr<Station>, int>> &Station::getBusNeighborStations() const {
    return busNeighborStations;
}

void Station::setBusNeighborStations(const vector<pair<weak_ptr<Station>, int>> &busNeighborStations) {
    Station::busNeighborStations = busNeighborStations;
}

const vector<pair<weak_ptr<Station>, int>> &Station::getTramNeighborStations() const {
    return tramNeighborStations;
}

void Station::setTramNeighborStations(const vector<pair<weak_ptr<Station>, int>> &tramNeighborStations) {
    Station::tramNeighborStations = tramNeighborStations;
}

const vector<pair<weak_ptr<Station>, int>> &Station::getSprinterNeighborStations() const {
    return sprinterNeighborStations;
}

void Station::setSprinterNeighborStations(const vector<pair<weak_ptr<Station>, int>> &sprinterNeighborStations) {
    Station::sprinterNeighborStations = sprinterNeighborStations;
}

const vector<pair<weak_ptr<Station>, int>> &Station::getRailNeighborStations() const {
    return railNeighborStations;
}

void Station::setRailNeighborStations(const vector<pair<weak_ptr<Station>, int>> &railNeighborStations) {
    Station::railNeighborStations = railNeighborStations;
}

void Station::setName(const string &name) {
    Station::name = name;
}

Station::Station(Station &&rhs) {
    busNeighborStations = rhs.getBusNeighborStations();
    tramNeighborStations = rhs.getTramNeighborStations();
    sprinterNeighborStations = rhs.getSprinterNeighborStations();
    railNeighborStations = rhs.getRailNeighborStations();
    name = rhs.getName();

    rhs.setBusNeighborStations(vector<pair<weak_ptr<Station>, int>>());
    rhs.setTramNeighborStations(vector<pair<weak_ptr<Station>, int>>());
    rhs.setSprinterNeighborStations(vector<pair<weak_ptr<Station>, int>>());
    rhs.setRailNeighborStations(vector<pair<weak_ptr<Station>, int>>());
    rhs.setName(nullptr);
}

Station & Station::operator=(Station &&rhs) {
    busNeighborStations = rhs.getBusNeighborStations();
    tramNeighborStations = rhs.getTramNeighborStations();
    sprinterNeighborStations = rhs.getSprinterNeighborStations();
    railNeighborStations = rhs.getRailNeighborStations();
    name = rhs.getName();

    return *this;
}

void Station::addBusConnection(const shared_ptr<Station> &toAdd, const int &durationTime) {
    busNeighborStations.emplace_back(toAdd,durationTime);
}

void Station::addTramConnection(const shared_ptr<Station> &toAdd, const int &durationTime) {
    tramNeighborStations.emplace_back(toAdd,durationTime);
}

void Station::addSprinterConnection(const shared_ptr<Station> &toAdd, const int &durationTime) {
    sprinterNeighborStations.emplace_back(toAdd,durationTime);
}

void Station::addRailConnection(const shared_ptr<Station> &toAdd, const int &durationTime) {
    railNeighborStations.emplace_back(toAdd,durationTime);
}

void Station::updateConnection(const string &destinationNode, const int &duration) {
    for(auto neighbor : busNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == destinationNode){
                neighbor.second = duration;
                return;
            }
        }
    }
    for(auto neighbor : tramNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == destinationNode){
                neighbor.second = duration;
                return;
            }
        }
    }
    for(auto neighbor : sprinterNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == destinationNode){
                neighbor.second = duration;
                return;
            }
        }
    }
    for(auto neighbor : railNeighborStations){
        if (auto station = neighbor.first.lock()){
            if (station->getName() == destinationNode){
                neighbor.second = duration;
                return;
            }
        }
    }
}

