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
						initialSpatialN, intermediateSpatialN, initialEFieldN, intermediateEFieldN,
						500.0, "ProtoDUNE");

    SpaceChargeProtoDUNE *oldExperiment = new SpaceChargeProtoDUNE("../../OldFiles/ProtoDUNE/Mike_SCEoffsets_ProtoDUNE_E500.root");
    TFile *fileInput = new TFile("../../InputFiles/dispOutput_protoDUNE_E500.root");
    TFile *histoFile = new TFile("../../OutputFiles/OffsetsComparision_ProtoDUNE.root", "RECREATE");
    int xMin = -360;
    int xMax = 360;
    int yMin = -5;
    int yMax = 615;
    int zMin = -5;
    int zMax = 705;
    int sFactor = 1;

    /**
       SpaceCharge *myExperiment = new SpaceCharge("../../OutputFiles/SCEoffsets_MicroBooNE_E500.root",
       initialSpatialN, intermediateSpatialN, initialEFieldN, intermediateEFieldN,
       500.0, "MicroBooNE");
       SpaceChargeMicroBooNE *oldExperiment = new SpaceChargeMicroBooNE("../../OldFiles/MicroBooNE/Mike_SCEoffsets_MicroBooNE_E500.root");
       TFile *fileInput = new TFile("../../InputFiles/dispOutput_MicroBooNE_E500.root");
       TFile *histoFile = new TFile("../../OutputFiles/OffsetsComparision_MicroBooNE.root", "RECREATE");
       int xMin = 0;
       int xMax = 260;
       int yMin = -120;
       int yMax = 120;
       int zMin = 0;
       int zMax = 1050;
       int sFactor = 2;
    **/
    const int dBins = 264 / sFactor;
    const double minD = -22.0 / sFactor;
    const double maxD = 22.0 / sFactor;
    const int eBins = 180 / sFactor;
    const double minE = -0.15 / sFactor;
    const double maxE = 0.15 / sFactor;

    TH1D *iDx = new TH1D("iDx", "", dBins / 2, -0.22 / 2.0, 0.22 / 2.0);
    TH1D *iDy = new TH1D("iDy", "", dBins, -0.22, 0.22);
    TH1D *iDz = new TH1D("iDz", "", dBins, -0.22, 0.22);
    TH1D *iEx = new TH1D("iEx", "", eBins, -7000.0, 7000.0);
    TH1D *iEy = new TH1D("iEy", "", eBins, -7000.0, 7000.0);
    TH1D *iEz = new TH1D("iEz", "", eBins, -7000.0, 7000.0);

    TH1D *mDx = new TH1D("mDx", "", dBins / 2, minD / 2.0, maxD / 2.0);
    TH1D *mDy = new TH1D("mDy", "", dBins, minD, maxD);
    TH1D *mDz = new TH1D("mDz", "", dBins, minD, maxD);
    TH1D *mEx = new TH1D("mEx", "", eBins, minE, maxE);
    TH1D *mEy = new TH1D("mEy", "", eBins, minE, maxE);
    TH1D *mEz = new TH1D("mEz", "", eBins, minE, maxE);

    TH1D *oDx = new TH1D("oDx", "", dBins / 2, minD / 2.0, maxD / 2.0);
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

    int nSkip = 10;
    for(int iX = xMin; iX <= xMax; iX++)
        {
            iX = iX + nSkip;
            for(int iY = yMin; iY <= yMax; iY++)
                {
                    iY = iY + nSkip;
                    for(int iZ = zMin; iZ <= zMax; iZ++)
                        {
                            iZ = iZ + nSkip;
                            cout << iX << ", " << iY << ", " << iZ << endl;

                            vector<double> mySpatialOffsets = myExperiment->GetPosOffsets(iX, iY, iZ);
                            if(!((mySpatialOffsets.at(0) == mySpatialOffsets.at(1)) &&
				 (mySpatialOffsets.at(1) == mySpatialOffsets.at(2)) &&
				 (mySpatialOffsets.at(2) == 0.0)))
                                {
                                    mDx->Fill(mySpatialOffsets.at(0));
                                    mDy->Fill(mySpatialOffsets.at(1));
                                    mDz->Fill(mySpatialOffsets.at(2));
                                }

                            vector<double> myEfieldOffsets = myExperiment->GetEfieldOffsets(iX, iY, iZ);
                            if(!((myEfieldOffsets.at(0) == myEfieldOffsets.at(1)) &&
				 (myEfieldOffsets.at(1) == myEfieldOffsets.at(2)) &&
				 (myEfieldOffsets.at(2) == 0.0)))
                                {
                                    mEx->Fill(myEfieldOffsets.at(0));
                                    mEy->Fill(myEfieldOffsets.at(1));
                                    mEz->Fill(myEfieldOffsets.at(2));
                                }

                            vector<double> oldSpatialOffsets = oldExperiment->GetPosOffsets(iX, iY, iZ);
                            if(!((oldSpatialOffsets.at(0) == oldSpatialOffsets.at(1)) &&
				 (oldSpatialOffsets.at(1) == oldSpatialOffsets.at(2)) &&
				 (oldSpatialOffsets.at(2) == 0.0)))
                                {
                                    oDx->Fill(oldSpatialOffsets.at(0));
                                    oDy->Fill(oldSpatialOffsets.at(1));
                                    oDz->Fill(oldSpatialOffsets.at(2));
                                }

                            vector<double> oldEfieldOffsets = oldExperiment->GetEfieldOffsets(iX, iY, iZ);
                            if(!((oldEfieldOffsets.at(0) == oldEfieldOffsets.at(1)) &&
				 (oldEfieldOffsets.at(1) == oldEfieldOffsets.at(2)) &&
				 (oldEfieldOffsets.at(2) == 0.0)))
                                {
                                    oEx->Fill(oldEfieldOffsets.at(0));
                                    oEy->Fill(oldEfieldOffsets.at(1));
                                    oEz->Fill(oldEfieldOffsets.at(2));
                                }
                        }
                }
        }
    histoFile->Write();
    histoFile->Close();

    return 0;
}
