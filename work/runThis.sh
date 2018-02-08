rm -rf MapSCE
g++ -Wall -std=c++11 MakeMapSCE.C ../src/MapSCE.C `root-config --cflags --glibs` -o MapSCE;
./MapSCE

