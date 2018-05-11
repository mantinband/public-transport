//
// Created by amichai on 08/05/18.
//

#include "Express.h"

string Express::uniSearch() const {
    return std::__cxx11::string();
}

Express::Express(PublicTransport &publicTransport){
    auto stations = publicTransport.getStationList();

    for (const auto &station : stations){
        v.push_back(pair(station,INT32_MAX));
    }
}

void Express::printQueue() const{
    for (auto pair : v){
        cout << pair.first.lock()->getName() << pair.second << endl;
    }

}



