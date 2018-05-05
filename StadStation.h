//
// Created by amichai on 05/05/18.
//

#ifndef OOP_CPP_EX2_STADSTATION_H
#define OOP_CPP_EX2_STADSTATION_H


#include "Station.h"

class StadStation : public Station{
public:
    static const int DEFAULT_CHANGE_TIME;
    explicit StadStation(const string &name);

    static int getChangeTime();

    static void setChangeTime(int changeTime);

private:
    static int changeTime;
};



#endif //OOP_CPP_EX2_STADSTATION_H
