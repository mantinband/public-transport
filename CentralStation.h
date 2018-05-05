//
// Created by amichai on 05/05/18.
//

#ifndef OOP_CPP_EX2_CENTRALSTATION_H
#define OOP_CPP_EX2_CENTRALSTATION_H


#include "Station.h"

class CentralStation : public Station{
public:
    static const int DEFAULT_CHANGE_TIME;
    CentralStation(const string &name);
private:
public:
    static const int &getChangeTime();

    static void setChangeTime(int changeTime);

private:
    static int changeTime;
};


#endif //OOP_CPP_EX2_CENTRALSTATION_H
