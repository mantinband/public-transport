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
/*  returns input option (i.e. load, outbound, inbound etc) */
inputOptions getOption(string input);

/*  extracts data from given arguments. if invalid files or
 * arguments are given program aborts with error message   */
void loadDataFromArguments(PublicTransport &publicTransport, int argc, char **argv);

/*  loop requesting query as long as QUIT was not chosen    */
void getPublicTransportQuerys(PublicTransport publicTransport);

/*  returns true if station exists in public transport
 * station list. false and error message otherwise */
bool stationExists(const PublicTransport &publicTransport, const string &stationName);

/*  prints instructions for expected arguments  */
void displayUsage();

#endif //OOP_CPP_EX2_MAINFUNCTIONS_H
