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
    StadStation(string name, const Station &rhs);
    ~StadStation() override;

    /*  returns stad station enum value  */
    StationTypes getStationType() const override;

    /*  returns change time between transport options at station  */
    int getSwitchTransportTime() const override;

    static int getChangeTime();
    static void setChangeTime(int changeTime);

private:
    static int CHANGE_TIME;
};



#endif //OOP_CPP_EX2_STADSTATION_H
