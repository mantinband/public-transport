//
// Created by amichai on 05/05/18.
//

#include "CentralStation.h"

const int CentralStation::DEFAULT_CHANGE_TIME = 5;
CentralStation::CentralStation(const string &name) : Station(name) {
}
int CentralStation::CHANGE_TIME;

const int &CentralStation::getChangeTime() {
    return CHANGE_TIME;
}

void CentralStation::setChangeTime(int changeTime) {
    CentralStation::CHANGE_TIME = changeTime;
}

Station::StationTypes CentralStation::getStationType() const {
    return central;
}

int CentralStation::getSwitchTransportTime() const {
    return DEFAULT_CHANGE_TIME;
}

