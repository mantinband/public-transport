//
// Created by amichai on 06/05/18.
//

#ifndef OOP_CPP_EX2_MAINFUNCTIONS_H
#define OOP_CPP_EX2_MAINFUNCTIONS_H
#include <iostream>
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

inputOptions getOption(string input);

#endif //OOP_CPP_EX2_MAINFUNCTIONS_H
