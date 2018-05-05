//
// Created by amichai on 05/05/18.
//

#include "IntercityStation.h"

const int IntercityStation::DEFAULT_CHANGE_TIME = 15;
IntercityStation::IntercityStation(const string &name) : Station(name){
}

int IntercityStation::changeTime;

int IntercityStation::getChangeTime() {
    return changeTime;
}

void IntercityStation::setChangeTime(int changeTime) {
    IntercityStation::changeTime = changeTime;
}

