#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include "PublicTransport.h"
using namespace std;

int main(int argc, char **argv) {
    PublicTransport publicTransport;

    try{
        for (int i=1; i<argc; i++){
            shared_ptr<ifstream> ifs(new ifstream());
            if (strcmp(argv[i],"-c") == 0){
                ifs->open(argv[++i],fstream::in);
                publicTransport.configure(ifs);
            } else if (strcmp(argv[i],"-o") == 0){
                publicTransport.setOutputFile(argv[++i]);
            } else {
                try {
                    ifs->open(argv[i], fstream::in);
                    publicTransport.load(ifs, argv[i]);
                } catch (exception &e){
                    if (i == argc-1 && publicTransport.getNumberOfStations() == 0){
                        throw PublicTransport::invalidInputFileException();
                    }
                }
            }
        }
    } catch (exception &e){
        cerr << e.what() << endl;
    }

    publicTransport.printStationList();
    return 0;
}