//
// Created by amichai on 05/05/18.
//

#include "IntercityStation.h"

const int IntercityStation::DEFAULT_CHANGE_TIME = 15;
IntercityStation::IntercityStation(const string &name) : Station(name){
}

int IntercityStation::CHANGE_TIME;

int IntercityStation::getChangeTime() {
    return CHANGE_TIME;
}

void IntercityStation::setChangeTime(int changeTime) {
    IntercityStation::CHANGE_TIME = changeTime;
}

Station::StationTypes IntercityStation::getStationType() const {
    return intercity;
}

int IntercityStation::getSwitchTransportTime() const {
    return DEFAULT_CHANGE_TIME;
}

