//
// Created by amichai on 06/05/18.
//

#include "mainFunctions.h"


bool stationExists(const PublicTransport &publicTransport, const string &stationName);

void displayUsage();

inputOptions getOption(string input){
    if (input == "load") return load;
    if (input == "outbound") return outbound;
    if (input == "inbound") return inbound;
    if (input == "uniExpress") return uniExpress;
    if (input == "multiExpress") return multiExpress;
    if (input == "print") return print;
    return invalidInput;
}

void loadDataFromArguments(PublicTransport &publicTransport, int argc, char **argv) {
    if (argc <= 2){
        cerr << "ERROR: Invalid number of arguments" << endl;
        displayUsage();
        exit(1);
    }
    try{
        int argumentType = none;
        for (int i=1; i<argc; i++){
            shared_ptr<ifstream> ifs(new ifstream());
            //check for flags
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
                    ifs->open(argv[i],fstream::in);
                    publicTransport.configure(ifs);
                    break;
                case output:
                    publicTransport.setOutputFile(argv[i]);
                    break;
                default:
                    cerr << "Invalid arguments" << endl;
                    displayUsage();
                    exit(1);
            }
        }
    } catch (exception &e){
        cerr << e.what() << endl;
        exit(2);
    } // end of load data

}

void getPublicTransportQuerys(PublicTransport publicTransport) {
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
                if (stationExists(publicTransport,sourceNode)){
                    cout << publicTransport.outboundStations(sourceNode) << endl;
                }
            } break;
            case (inbound): {
                string destinationNode;
                cin >> destinationNode;
                if (stationExists(publicTransport,destinationNode)){
                    cout << publicTransport.inboundStations(destinationNode) << endl;
                }
            } break;
            case (uniExpress): {
                string sourceNode;
                string destinationNode;

                cin >> sourceNode >> destinationNode;
                if (stationExists(publicTransport,sourceNode) && stationExists(publicTransport,destinationNode)){
                    cout << publicTransport.uniExpressOptions(sourceNode,destinationNode) << endl;
                }
            } break;
            case (multiExpress): {
                string sourceNode;
                string destinationNode;

                cin >> sourceNode >> destinationNode;
                if (stationExists(publicTransport,sourceNode) && stationExists(publicTransport,destinationNode)){
                    cout << publicTransport.multiExpressOptions(sourceNode, destinationNode) << endl;
                }
                break;
            }
            case (print):
                try {
                    publicTransport.printToFile();
                } catch (exception &e){
                    cerr << e.what() << endl;
                }
                publicTransport.printStationList(); //for debugging
                break;
            default:
                cerr << "ERROR: Invalid input" << endl;
        }
        cin >> input;
    }
}

bool stationExists(const PublicTransport &publicTransport, const string &stationName) {
    if (!publicTransport.isInStationList(stationName)) {
        cerr << stationName << " does not exist in the current network \n";
        return false;
    }
    return true;
}

void displayUsage() {
    cerr << "usage: <./program_name> -i <infile1> [ <infile2> <infile3> ... ]\n\t[ [-c] <config_file> ]\n\t[ [-o] <outfile_name> ]" << endl;
}
