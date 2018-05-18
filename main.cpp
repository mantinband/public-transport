#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include "mainFunctions.h"
#include "PublicTransport.h"

using namespace std;

int main(int argc, char **argv) {
    PublicTransport publicTransport;

    if (argc <= 1){
        cerr << "ERROR: Invalid number of arguments" << endl;
        exit(1);
    }

    //load data from given arguments
    try{
        int argumentType = none;
        for (int i=1; i<argc; i++){
            shared_ptr<ifstream> ifs(new ifstream());
            if (strcmp(argv[i],"-i") == 0){
                argumentType = input;
                i++;
            } else if (strcmp(argv[i],"-c") == 0){
                argumentType = configure;
                i++;
            } else if (strcmp(argv[i],"-o") == 0){
                argumentType = output;
                i++;
            }
            switch (argumentType){
                case input:
                    try {
                        ifs->open(argv[i], fstream::in);
                        publicTransport.load(ifs, argv[i]);
                    } catch (exception &e){
                        if (i == argc-1 && publicTransport.getNumberOfStations() == 0){
                            throw PublicTransport::invalidInputFileException();
                        }
                    }
                    break;
                case configure:
                    ifs->open(argv[++i],fstream::in);
                    publicTransport.configure(ifs);
                    break;
                case output:
                    publicTransport.setOutputFile(argv[++i]);
                    break;
                default:
                    cerr << "ERROR: Invalid input.\nArgument must follow "
                            "one of the following: [-i] input, [-c] configure [-o] output file" << endl;
                    exit(1);
                    break;
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
                    cerr << e.what() << endl;
                }
            }   break;
            case (outbound): {
                string sourceNode;
                cin >> sourceNode;
                if (!publicTransport.isInStationList(sourceNode)){
                    cerr << sourceNode << " does not exist in the current network \n" ;
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
            case (uniExpress): {
                string sourceNode;
                string destinationNode;

                cin >> sourceNode >> destinationNode;
                if (!publicTransport.isInStationList(sourceNode)){
                    cerr << sourceNode << " does not exist in the current network \n";
                } else if (!publicTransport.isInStationList(destinationNode)){
                    cerr << destinationNode << " does not exist in the current network \n";
                } else {
                    cout << publicTransport.uniExpressOptions(sourceNode,destinationNode) << endl;
                }
            } break;
            case (multiExpress): {
                string sourceNode;
                string destinationNode;

                cin >> sourceNode >> destinationNode;
                if (!publicTransport.isInStationList(sourceNode)) {
                    cerr << sourceNode << " does not exist in the current network \n";
                } else if (!publicTransport.isInStationList(destinationNode)) {
                    cerr << destinationNode << " does not exist in the current network \n";
                } else {
                    cout << publicTransport.multiExpressOptions(sourceNode, destinationNode) << endl;
                }
                break;
            }
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



