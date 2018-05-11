//
// Created by amichai on 08/05/18.
//

#ifndef OOP_CPP_EX2_EXPRESS_H
#define OOP_CPP_EX2_EXPRESS_H
#include "PublicTransport.h"
#include <queue>

using namespace std;


class Express {

public:
    explicit Express(PublicTransport &publicTransport);
    void printQueue() const;
    string uniSearch() const;
private:
    vector<pair<weak_ptr<Station>,int >> v;
};


#endif //OOP_CPP_EX2_EXPRESS_H
