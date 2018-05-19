//
// Created by amichai on 05/05/18.
//

#ifndef OOP_CPP_EX2_CENTRALSTATION_H
#define OOP_CPP_EX2_CENTRALSTATION_H


#include "Station.h"

class CentralStation : public Station{
public:
    static const int DEFAULT_CHANGE_TIME;

    explicit CentralStation(const string &name); //c'tor
    explicit CentralStation(string name, Station &rhs);  //copy c'tor
    ~CentralStation() override;

    /*  returns central station enum value  */
    StationTypes getStationType() const override;
    /*  returns change time between transport options at station  */
    int getSwitchTransportTime() const override;

    static const int &getChangeTime();
    static void setChangeTime(int changeTime);
private:
    static int CHANGE_TIME;
};


#endif //OOP_CPP_EX2_CENTRALSTATION_H
