rm -rf MapSCE
find ../HistoDirectory/ -name "*.pdf" | xargs rm
rm -rf ../OutputFiles/Result_*
g++ -Wall -std=c++11 MakeMapSCE.C ../src/MapSCE.C `root-config --cflags --glibs` -o MapSCE;
./MapSCE

