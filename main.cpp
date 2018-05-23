#include "mainFunctions.h"

int main(int argc, char **argv) {
    PublicTransport publicTransport;

    //load data from given arguments
    loadDataFromArguments(publicTransport, argc, argv);

    //get user querys
    getPublicTransportQuerys(publicTransport);



    return 0;
}





