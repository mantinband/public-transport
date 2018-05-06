//
// Created by amichai on 06/05/18.
//

#include "mainFunctions.h"


inputOptions getOption(string input){
    if (input == "load") return load;
    if (input == "outbound") return outbound;
    if (input == "inbound") return inbound;
    if (input == "uniExpress") return uniExpress;
    if (input == "multiExpress") return multiExpress;
    if (input == "print") return print;
    return invalidInput;
}