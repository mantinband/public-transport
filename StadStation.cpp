//
// Created by amichai on 05/05/18.
//

#include "StadStation.h"

#include <utility>

const int StadStation::DEFAULT_CHANGE_TIME = 10;

StadStation::StadStation(const string &name) : Station(name) {
}
int StadStation::CHANGE_TIME;

int StadStation::getChangeTime() {
    return CHANGE_TIME;
}

void StadStation::setChangeTime(int changeTime) {
    StadStation::CHANGE_TIME = changeTime;
}

Station::StationTypes StadStation::getStationType() const {
    return stad;
}

int StadStation::getSwitchTransportTime() const {
    return DEFAULT_CHANGE_TIME;
}

StadStation::StadStation(string name, const Station &rhs) : Station(std::move(name)) {
}

StadStation::~StadStation() = default;
