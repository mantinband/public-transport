//
// Created by amichai on 05/05/18.
//

#include "CentralStation.h"

const int CentralStation::DEFAULT_CHANGE_TIME = 5;
CentralStation::CentralStation(const string &name) : Station(name) {
}
int CentralStation::changeTime;

const int &CentralStation::getChangeTime() {
    return changeTime;
}

void CentralStation::setChangeTime(int changeTime) {
    CentralStation::changeTime = changeTime;
}
