# public transport

```
usage: <./program name> -i <input file> [<input file> <input file> ..] [ [-c] <configuration file> [-o] <output file name> ]
```

## program manages public transportation and supports quaries as defined below.

#### the public transportation is made of stations which can be one of:  
* central station 
* intercity station
* stad station.  
#### transportation type are one of the following:
* bus
* tram
* sprinter
* rail.


information regarding stations and travel duration between
two station is inserted via files.
input file must start with "bus" "tram" "sprinter" or "rail"
to indicate what transport method is given.

### supported queries are:
* ```inbound <station name>``` *(from which station current station is reachable)*
* ```outbound <station name>``` *(which stations are reachable from current station)*
* ```load <filename>``` *(load new information to system)*
* ```uniExpress <source node> <destination node>``` *(shortest travel time between two stations. output is given for each transport option)*
* ```multiExpress <source node> <destination node>``` *(shortest travel time between two stations. query allows switching between transportation form)*
* ```print``` *(prints stations to <outputfile>)*
* ```QUIT``` *(exit program)*
