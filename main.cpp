#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include "mainFunctions.h"
#include "PublicTransport.h"
using namespace std;

int main(int argc, char **argv) {
    PublicTransport publicTransport;

    //load data from given arguments
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
    } // end of load data


    string input;
    cin >> input;


    while (input != "QUIT"){
        switch(getOption(input)){
            case (load): {
                string fileName;
                cin >> fileName;
                shared_ptr<ifstream> ifs(new ifstream(fileName));
                try {
                    publicTransport.load(ifs, fileName);
                } catch (PublicTransport::invalidInputFileException &e) {
                    e.what();
                }
            }   break;
            case (outbound): {
                string sourceNode;
                cin >> sourceNode;
                if (!publicTransport.isInStationList(sourceNode)){
                    cerr << sourceNode << "does not exist in the current network \n" ;
                } else {
                    cout << publicTransport.outboundStations(sourceNode) << endl;
                }
            } break;
            case (inbound): {
                string destinationNode;
                cin >> destinationNode;
                if (!publicTransport.isInStationList(destinationNode)){
                    cerr << destinationNode << " does not exist in the current network \n";
                } else {
                    cout << publicTransport.inboundStations(destinationNode) << endl;
                }
            } break;
            case (uniExpress):
                break;
            case (multiExpress):
                break;
            case (print):
                publicTransport.printStationList();
                break;
            default:
                cerr << "ERROR: Invalid input" << endl;

        }
        cin >> input;
    }


    publicTransport.printStationList();
    return 0;
}



