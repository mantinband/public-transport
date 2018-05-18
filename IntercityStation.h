//
// Created by amichai on 05/05/18.
//

#ifndef OOP_CPP_EX2_INTERCITYSTATION_H
#define OOP_CPP_EX2_INTERCITYSTATION_H


#include "Station.h"

class IntercityStation : public Station{
public:
    static const int DEFAULT_CHANGE_TIME;
    explicit IntercityStation(const string &name);
    virtual StationTypes getStationType() const;
    virtual int getSwitchTransportTime() const;
private:
    static int CHANGE_TIME;
public:
    static int getChangeTime();
    static void setChangeTime(int changeTime);
};


#endif //OOP_CPP_EX2_INTERCITYSTATION_H
