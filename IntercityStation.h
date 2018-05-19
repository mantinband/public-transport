//
// Created by amichai on 05/05/18.
//

#ifndef OOP_CPP_EX2_INTERCITYSTATION_H
#define OOP_CPP_EX2_INTERCITYSTATION_H


#include "Station.h"

class IntercityStation : public Station{
public:
    static const int DEFAULT_CHANGE_TIME;

    explicit IntercityStation(const string &name);  //c'tor
    explicit IntercityStation(string name, Station &rhs);        //copy c'tor
    ~IntercityStation() override;                   //d'tor

    /*  returns intercity station enum value  */
    StationTypes getStationType() const override;
    /*  returns change time between transport options at station  */
    int getSwitchTransportTime() const override;

    static int getChangeTime();
    static void setChangeTime(int changeTime);

private:
    static int CHANGE_TIME;
};


#endif //OOP_CPP_EX2_INTERCITYSTATION_H
