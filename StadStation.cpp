//
// Created by amichai on 05/05/18.
//

#include "StadStation.h"

const int StadStation::DEFAULT_CHANGE_TIME = 10;

StadStation::StadStation(const string &name) : Station(name) {
}
int StadStation::changeTime;

int StadStation::getChangeTime() {
    return changeTime;
}

void StadStation::setChangeTime(int changeTime) {
    StadStation::changeTime = changeTime;
}