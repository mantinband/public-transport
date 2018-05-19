//
// Created by amichai on 06/05/18.
//

#ifndef OOP_CPP_EX2_MAINFUNCTIONS_H
#define OOP_CPP_EX2_MAINFUNCTIONS_H
#include <iostream>
#include <cstring>
#include "PublicTransport.h"

using namespace std;

enum inputOptions{
    load,
    outbound,
    inbound,
    uniExpress,
    multiExpress,
    print,
    invalidInput
};

enum argumentTypes{
    none,
    input,
    configure,
    output
};

inputOptions getOption(string input);
void loadDataFromArguments(PublicTransport &publicTransport, int argc, char **argv);
void getPublicTransportQuerys(PublicTransport publicTransport);
#endif //OOP_CPP_EX2_MAINFUNCTIONS_H
