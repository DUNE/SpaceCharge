// C++ stuff
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

// Root stuff
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TRatioPlot.h>

#include "../../SourceCode/SpaceCharge.h"
#include "../../OldFiles/ProtoDUNE/SpaceChargeProtoDUNE.h"
#include "../../OldFiles/MicroBooNE/SpaceChargeMicroBooNE.h"

using namespace std;

int main()
{
    int initialSpatialN[3] = {4, 5, 3};
    int intermediateSpatialN[3] = {6, 5, 4};
    int initialEFieldN[3] = {4, 5, 3};
    int intermediateEFieldN[3] = {6, 5, 4};

    SpaceCharge *myExperiment = new SpaceCharge("../../OutputFiles/SCEoffsets_ProtoDUNE_E500.root",
						initialSpatialN, intermediateSpatialN, initialEFieldN, intermediateEFieldN, "ProtoDUNE");

    SpaceChargeProtoDUNE *oldExperiment = new SpaceChargeProtoDUNE("../../OldFiles/ProtoDUNE/Mike_SCEoffsets_ProtoDUNE_E500.root");
    TFile *fileInput = new TFile("../../InputFiles/dispOutput_protoDUNE_E500.root");
    TFile *histoFile = new TFile("../../OutputFiles/OffsetsComparision_ProtoDUNE.root", "RECREATE");
    int xMin = -360;
    int xMax = 360;
    int yMin = -5;
    int yMax = 615;
    int zMin = -5;
    int zMax = 705;

    /**
       SpaceCharge *myExperiment = new SpaceCharge("../../OutputFiles/SCEoffsets_MicroBooNE_E500.root",
       initialSpatialN, intermediateSpatialN, initialEFieldN, intermediateEFieldN, "MicroBooNE");
       SpaceChargeMicroBooNE *oldExperiment = new SpaceChargeMicroBooNE("../../OldFiles/MicroBooNE/Mike_SCEoffsets_MicroBooNE_E500.root");
       TFile *fileInput = new TFile("../../InputFiles/dispOutput_MicroBooNE_E500.root");
       TFile *histoFile = new TFile("../../OutputFiles/OffsetsComparision_MicroBooNE.root", "RECREATE");
       int xMin = 0;
       int xMax = 260;
       int yMin = -120;
       int yMax = 120;
       int zMin = 0;
       int zMax = 1050;
    **/

    const int dBins = 100;
    const double minD = -0.1;
    const double maxD = 0.1;
    const int eBins = 100;
    const double minE = -7000;
    const double maxE = 7000;

    TH1D *iDx = new TH1D("iDx", "", dBins, minD, maxD);
    TH1D *iDy = new TH1D("iDy", "", dBins, minD, maxD);
    TH1D *iDz = new TH1D("iDz", "", dBins, minD, maxD);
    TH1D *iEx = new TH1D("iEx", "", eBins, minE, maxE);
    TH1D *iEy = new TH1D("iEy", "", eBins, minE, maxE);
    TH1D *iEz = new TH1D("iEz", "", eBins, minE, maxE);

    TH1D *mDx = new TH1D("mDx", "", dBins, minD, maxD);
    TH1D *mDy = new TH1D("mDy", "", dBins, minD, maxD);
    TH1D *mDz = new TH1D("mDz", "", dBins, minD, maxD);
    TH1D *mEx = new TH1D("mEx", "", eBins, minE, maxE);
    TH1D *mEy = new TH1D("mEy", "", eBins, minE, maxE);
    TH1D *mEz = new TH1D("mEz", "", eBins, minE, maxE);

    TH1D *oDx = new TH1D("oDx", "", dBins, minD, maxD);
    TH1D *oDy = new TH1D("oDy", "", dBins, minD, maxD);
    TH1D *oDz = new TH1D("oDz", "", dBins, minD, maxD);
    TH1D *oEx = new TH1D("oEx", "", eBins, minE, maxE);
    TH1D *oEy = new TH1D("oEy", "", eBins, minE, maxE);
    TH1D *oEz = new TH1D("oEz", "", eBins, minE, maxE);

    TTree *TreeD = (TTree*)fileInput->Get("SpaCEtree_fwdDisp");
    TreeD->Draw("Dx>>iDx");
    TreeD->Draw("Dy>>iDy");
    TreeD->Draw("Dz>>iDz");
    TTree *TreeE = (TTree*)fileInput->Get("SpaCEtree");
    TreeE->Draw("Ex>>iEx");
    TreeE->Draw("Ey>>iEy");
    TreeE->Draw("Ez>>iEz");

    for(int iX = xMin; iX <= xMax; iX++)
        {
            iX = iX + 10;
            for(int iY = yMin; iY <= yMax; iY++)
                {
                    iY = iY + 10;
                    for(int iZ = zMin; iZ <= zMax; iZ++)
                        {
                            iZ = iZ + 10;
                            cout << iX << ", " << iY << ", " << iZ << endl;

                            vector<double> mySpatialOffsets = myExperiment->GetPosOffsets(iX, iY, iZ);
                            vector<double> myEfieldOffsets = myExperiment->GetEfieldOffsets(iX, iY, iZ);
                            mDx->Fill(mySpatialOffsets.at(0));
                            mDy->Fill(mySpatialOffsets.at(1));
                            mDz->Fill(mySpatialOffsets.at(2));
                            mEx->Fill(myEfieldOffsets.at(0));
                            mEy->Fill(myEfieldOffsets.at(1));
                            mEz->Fill(myEfieldOffsets.at(2));

                            vector<double> oldSpatialOffsets = oldExperiment->GetPosOffsets(iX, iY, iZ);
                            vector<double> oldEfieldOffsets = oldExperiment->GetEfieldOffsets(iX, iY, iZ);
                            oDx->Fill(oldSpatialOffsets.at(0));
                            oDy->Fill(oldSpatialOffsets.at(1));
                            oDz->Fill(oldSpatialOffsets.at(2));
                            oEx->Fill(oldEfieldOffsets.at(0));
                            oEy->Fill(oldEfieldOffsets.at(1));
                            oEz->Fill(oldEfieldOffsets.at(2));
                        }
                }
        }
    histoFile->Write();
    histoFile->Close();

    return 0;
}
