#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "stdio.h"

#include "../src/SpaceCharge.h"
//#include "../TestFiles/SpaceChargeMicroBooNE.h"

using namespace std;

int main()
{
    //const double Edrift = 0.500; // in kV/cm
    //const double dEdx = 2.1; // in MeV/cm
    //const double ModBoxA = 0.930; // recomb constant A used in LArG4
    //const double ModBoxB = 0.212; // recomb constant B used in LArG4

    int initialSpatialN[3] = {4, 5, 3};
    int intermediateSpatialN[3] = {6, 5, 4};
    int initialEFieldN[3] = {4, 5, 3};
    int intermediateEFieldN[3] = {6, 5, 4};
    SpaceCharge *myMicroBooNE = new SpaceCharge("../OutputFiles/SCEoffsets_ProtoDUNE_E500.root", initialSpatialN, intermediateSpatialN, initialEFieldN, intermediateEFieldN, "MicroBooNE");
    //SpaceChargeMicroBooNE *oldMicroBooNE = new SpaceChargeMicroBooNE("../TestFiles/Mike_SCEoffsets_ProtoDUNE_E500.root");

    //TFile *FileInput = new TFile("../InputFiles/dispOutput_MicroBooNE_E500.root");

    /**
    double position[3] = {0.0, 0.0, 0.0};
    vector<double> mySpatialOffsets = myMicroBooNE->GetPosOffsets(position[0], position[1], position[2]);
    vector<double> oldSpatialOffsets = oldMicroBooNE->GetPosOffsets(position[0], position[1], position[2]);
    vector<double> myEFieldOffsets = myMicroBooNE->GetEfieldOffsets(position[0], position[1], position[2]);
    vector<double> oldEFieldOffsets = oldMicroBooNE->GetEfieldOffsets(position[0], position[1], position[2]);

    cout << "Spatial Offsets:  "<<endl;
    cout<<"Mine: "<< -1.0 * mySpatialOffsets.at(0) << ", " << mySpatialOffsets.at(1) << ", " << mySpatialOffsets.at(2) << endl;
    cout<<"Old : "<< -1.0 * oldSpatialOffsets.at(0) << ", " << oldSpatialOffsets.at(1) << ", " << oldSpatialOffsets.at(2) << endl<<endl;

    cout << "EField Offsets:  "<<endl;
    cout<<"Mine: "<< myEFieldOffsets.at(0) << ", " << myEFieldOffsets.at(1) << ", " << myEFieldOffsets.at(2) << endl;
    cout<<"Old : "<< oldEFieldOffsets.at(0) << ", " << oldEFieldOffsets.at(1) << ", " << oldEFieldOffsets.at(2) << endl<<endl;
    **/
    return 0;
}
