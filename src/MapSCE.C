// C++ stuff
#include <iostream>
#include <iomanip>
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

#include "MapSCE.h"

using namespace std;

MapSCE :: MapSCE(const TString input, const TString output, const string histo, const double drift)
    : InputFile(input), OutputDirectory(output), HistoDirectory(histo), DriftField(drift)
{
}

void MapSCE::SetInputFile(const TString input)
{
    InputFile = input;
}

void MapSCE::SetOutputDirectory(const TString output)
{
    OutputDirectory = output;
}

void MapSCE::SetHistoDirectory(const string histo)
{
    HistoDirectory = histo;
}

void MapSCE::SetDriftField(const double drift)
{
    DriftField = drift;
}

///////////////////////////////////////////////////////////////////////////////////////
// Perform transformation                                                            //
// FieldToTransform can be Spatial or EField                                         //
// DimensionToTransform can be X, Y, Z                                               //
// FitPolN is polynomial of degree N: f(x) = p_0 + p_1*x + p_2*x^2 +...+ p_N*x^N     //
// For each call initialFitPolN x intermediateFitPolN TGraphs will be saved              //
///////////////////////////////////////////////////////////////////////////////////////
void MapSCE::PerformTransformation(string FieldToTransform, string DimensionToTransform, int initialFitPolN, int intermediateFitPolN)
{
    gStyle->SetOptStat(111);
    gStyle->SetStatY(0.35);
    gStyle->SetStatX(0.89);
    gStyle->SetStatW(0.11);
    gStyle->SetStatH(0.1);

    gStyle->SetOptFit(0111);

    cout << endl;
    cout << "----------------------------------------------------------------------" << endl;
    cout << "Performing transformation" << endl << endl;
    cout << "Input file is: " << InputFile << endl;
    cout << "Drift field is: " << DriftField << "V/m" << endl;
    cout << "Output file directory is: " << OutputDirectory << endl;
    cout << "Field to transform is: " << FieldToTransform << endl;
    cout << "Dimension to transform is: " << DimensionToTransform << endl;
    cout << "----------------------------------------------------------------------" << endl << endl;

    ///////////////////////////////////////////////////////////////////////////////////////////
    // Select the Tree
    ///////////////////////////////////////////////////////////////////////////////////////////
    TFile *FileInput = new TFile(InputFile);
    TTree *TreeInput = new TTree();

    double xTrue, yTrue, zTrue;
    double dX, dY, dZ;

    double conversionFactor = 1.0;//(1.0 / DriftField) * (1.0 / 100.0);
    if(FieldToTransform == "Spatial")
        {
            TreeInput = (TTree*)FileInput->Get("SpaCEtree_fwdDisp");
            TreeInput->SetBranchAddress("x_true", &xTrue);
            TreeInput->SetBranchAddress("y_true", &yTrue);
            TreeInput->SetBranchAddress("z_true", &zTrue);
            TreeInput->SetBranchAddress("Dx", &dX);
            TreeInput->SetBranchAddress("Dy", &dY);
            TreeInput->SetBranchAddress("Dz", &dZ);
        }
    else if(FieldToTransform == "EField")
        {
            TreeInput = (TTree*)FileInput->Get("SpaCEtree");
            TreeInput->SetBranchAddress("xpoint", &xTrue);
            TreeInput->SetBranchAddress("ypoint", &yTrue);
            TreeInput->SetBranchAddress("zpoint", &zTrue);
            TreeInput->SetBranchAddress("Ex", &dX);
            TreeInput->SetBranchAddress("Ey", &dY);
            TreeInput->SetBranchAddress("Ez", &dZ);
        }
    else
        {
            cout << "File to transform needs to be either \"Spatial\" or \"EFiled\"" << endl;
            cout << "Exiting!!" << endl << endl;
            exit(1);
        }

    ///////////////////////////////////////////////////////////////////////////////////////////
    // Work on the binning
    ///////////////////////////////////////////////////////////////////////////////////////////
    cout << "Total Entries in the Tree is: " << TreeInput->GetEntries() << endl;

    double xTrueMax = -9999.99, yTrueMax = -9999.99, zTrueMax = -9999.99;
    double dXMin = 9999.99, dYMin = 9999.99, dZMin = 9999.99;
    double dXMax = -9999.99, dYMax = -9999.99, dZMax = -9999.99;
    for (int i = 0; i < TreeInput->GetEntries(); i++)
        {
            TreeInput->GetEntry(i);

            if(FieldToTransform == "EField")
                {
                    dX = dX * conversionFactor;
                    dY = dY * conversionFactor;
                    dZ = dZ * conversionFactor;
                }

            if(xTrue > xTrueMax)
                {
                    xTrueMax = xTrue;
                }
            if(yTrue > yTrueMax)
                {
                    yTrueMax = yTrue;
                }
            if(zTrue > zTrueMax)
                {
                    zTrueMax = zTrue;
                }

            if(dX < dXMin)
                {
                    dXMin = dX;
                }
            if(dY < dYMin)
                {
                    dYMin = dY;
                }
            if(dZ < dZMin)
                {
                    dZMin = dZ;
                }

            if(dX > dXMax)
                {
                    dXMax = dX;
                }
            if(dY > dYMax)
                {
                    dYMax = dY;
                }
            if(dZ > dZMax)
                {
                    dZMax = dZ;
                }
        }

    double binningFactor = 5.0;

    cout << endl;
    cout << "xTrueMax = " << xTrueMax << ", yTrueMax = " << yTrueMax << ", zTrueMax = " << zTrueMax << endl;

    int xBins = xTrueMax * binningFactor, yBins = yTrueMax * binningFactor, zBins = zTrueMax * binningFactor;
    cout << "xBins = " << xBins << ", yBins = " << yBins << ", zBins = " << zBins << endl;

    double xLimit = xTrueMax / 2.0, yLimit = yTrueMax / 2.0, zLimit = zTrueMax / 2.0;
    cout << "xLimt = " << xLimit << ", yLimt = " << yLimit << ", zLimt = " << zLimit << endl;

    cout << "dXMin = " << dXMin << ", dXMax = " << dXMax << ", dYMin = " << dYMin << ", dYMax = " << dYMax << ", dZMin = " << dZMin << ", dZMax = " << dZMax << endl;
    cout << endl << endl;

    int iBins = 0, jBins = 0;
    double xAxisInitialMin = 0.0, xAxisInitialMax = 0, yAxisInitialMin = 0.0, yAxisInitialMax = 0.0;;
    TString initialWhatToDraw = "", initialConditionToDraw = "";

    double xAxisIntermediateMin = 0.0, xAxisIntermediateMax = 0;
    if(DimensionToTransform == "X")
        {
            iBins = yBins;
            jBins = zBins;

            xAxisInitialMin = -xLimit;
            xAxisInitialMax = xLimit;
            yAxisInitialMin = dXMin;
            yAxisInitialMax = dXMax;

            xAxisIntermediateMin = -yLimit;
            xAxisIntermediateMax = yLimit;

            if(FieldToTransform == "Spatial")
                {
                    initialWhatToDraw =  Form("Dx:x_true-%f", xAxisInitialMax);
                    initialConditionToDraw = "fabs(y_true-%f)<0.025 && fabs(z_true-%f)<0.025";
                }
            else
                {
                    initialWhatToDraw =  Form("Ex*%f:xpoint-%f", conversionFactor, xAxisInitialMax);
                    initialConditionToDraw = "fabs(ypoint-%f)<0.025 && fabs(zpoint-%f)<0.025";
                }
        }
    else if(DimensionToTransform == "Y")
        {
            iBins = xBins;
            jBins = zBins;

            xAxisInitialMin = -yLimit;
            xAxisInitialMax = yLimit;
            yAxisInitialMin = dYMin;
            yAxisInitialMax = dYMax;

            xAxisIntermediateMin = -xLimit;
            xAxisIntermediateMax = xLimit;

            if(FieldToTransform == "Spatial")
                {
                    initialWhatToDraw =  Form("Dy:y_true-%f", xAxisInitialMax);
                    initialConditionToDraw = "fabs(x_true-%f)<0.025 && fabs(z_true-%f)<0.025";
                }
            else
                {
                    initialWhatToDraw =  Form("Ey*%f:ypoint-%f", conversionFactor, xAxisInitialMax);
                    initialConditionToDraw = "fabs(xpoint-%f)<0.025 && fabs(zpoint-%f)<0.025";
                }
        }
    else if (DimensionToTransform == "Z")
        {
            iBins = yBins;
            jBins = zBins;

            xAxisInitialMin = -xLimit;
            xAxisInitialMax = xLimit;
            yAxisInitialMin = dZMin;
            yAxisInitialMax = dZMax;

            xAxisIntermediateMin = -yLimit;
            xAxisIntermediateMax = yLimit;

            if(FieldToTransform == "Spatial")
                {
                    initialWhatToDraw =  Form("Dz:x_true-%f", xAxisInitialMax);
                    initialConditionToDraw = "fabs(y_true-%f)<0.025 && fabs(z_true-%f)<0.025";
                }
            else
                {
                    initialWhatToDraw =  Form("Ez*%f:xpoint-%f", conversionFactor, xAxisInitialMax);
                    initialConditionToDraw = "fabs(ypoint-%f)<0.025 && fabs(zpoint-%f)<0.025";
                }
        }
    else
        {
            cout << "Dimension to transform needs to be either \"X\" or \"Y\" or \"Z\"" << endl;
            cout << "Exiting!!" << endl << endl;
            exit(1);
        }

    cout << "Working on dimension " << DimensionToTransform << endl;
    cout << "iBins, jBins: " << iBins << ", " << jBins << endl;
    cout << "xAxisInitialMin, xAxisInitialMax: " << xAxisInitialMin << ", " << xAxisInitialMax << endl;
    cout << "yAxisInitialMin, yAxisInitialMax: " << yAxisInitialMin << ", " << yAxisInitialMax << endl;
    cout << "xAxisIntermediateMin, xAxisIntermediateMax: " << xAxisIntermediateMin << ", " << xAxisIntermediateMax << endl << endl;

    double iPosition = 0.0, jPosition = 0.0;
    double initialFitParameter[initialFitPolN + 1];
    fill(initialFitParameter, initialFitParameter + (initialFitPolN + 1) / sizeof(double), 0.0);
    double initialFitParameterMin[initialFitPolN + 1];
    fill(initialFitParameterMin, initialFitParameterMin + (initialFitPolN + 1) / sizeof(double), 9999.99);
    double initialFitParameterMax[initialFitPolN + 1];
    fill(initialFitParameterMax, initialFitParameterMax + (initialFitPolN + 1) / sizeof(double), -9999.99);

    TTree *tIntermediateTree = new TTree("tIntermediateTree", "tIntermediateTree");
    tIntermediateTree->Branch("iPosition", &iPosition, "iPosition/D");
    tIntermediateTree->Branch("jPosition", &jPosition, "jPosition/D");
    for(int r = 0; r < initialFitPolN + 1; r++)
        {
            tIntermediateTree->Branch(Form("initialFitParameter_%i", r), &initialFitParameter[r], Form("initialFitParameter_%i/D", r));
        }

    int initialCount = 0;
    double initialCheckSum = 0.0;
    for(int i = 0; i <= iBins; i++)
        {
            iPosition = i / binningFactor;

            for(int j = 0; j <= jBins; j++)
                {
                    jPosition = j / binningFactor;

                    TCanvas *cInitialHistos = new TCanvas("cInitialHistos", " ", 1250, 600);
                    cInitialHistos->cd();


                    TH2D *hInitialHistos = new TH2D("hInitialHistos", "",
                                                    500, xAxisInitialMin + (0.025 * xAxisInitialMin), xAxisInitialMax + (0.025 * xAxisInitialMax),
                                                    250, yAxisInitialMin + (0.025 * yAxisInitialMin), yAxisInitialMax + (0.025 * yAxisInitialMax));
                    hInitialHistos->Sumw2();

                    TreeInput->Project("hInitialHistos", initialWhatToDraw, Form(initialConditionToDraw, iPosition, jPosition));

                    TF1 *initialFitFunction =  new TF1("initialFitFunction", Form("pol%i", initialFitPolN));
                    initialFitFunction->SetLineWidth(1);
                    hInitialHistos->Fit(initialFitFunction, "Q");
                    initialFitFunction->GetParameters(initialFitParameter);

                    double initialFitCheck = (initialFitFunction->GetChisquare()) / (initialFitFunction->GetNDF());
                    initialCheckSum += initialFitCheck;
                    initialCount++;

                    for(int f = 0; f < initialFitPolN + 1; f++)
                        {
                            if(initialFitParameter[f] > initialFitParameterMax[f])
                                {
                                    initialFitParameterMax[f] = initialFitParameter[f];
                                }
                            if(initialFitParameter[f] < initialFitParameterMin[f])
                                {
                                    initialFitParameterMin[f] = initialFitParameter[f];
                                }
                        }

                    hInitialHistos->Draw("CONT3");
                    cInitialHistos->Update();

                    cInitialHistos->SaveAs(Form("%s/hInitialHisto_%s_%s_%i_%i.pdf", HistoDirectory.c_str(), FieldToTransform.c_str(), DimensionToTransform.c_str(), i, j));

                    tIntermediateTree->Fill();
                    delete hInitialHistos;
                    delete cInitialHistos;
                }
        }

    double kPosition = 0.0;
    double intermediateFitParameter[initialFitPolN + 1][intermediateFitPolN + 1];
    for(int r = 0; r < initialFitPolN + 1; r++)
        {
            for(int i = 0; i < intermediateFitPolN + 1; i++)
                {
                    intermediateFitParameter[r][i] = 0.0;
                }
        }

    TTree *tFinalTree = new TTree("tFinalTree", "tFinalTree");
    tFinalTree->Branch("kPosition", &kPosition, "kPosition/D");
    for(int r = 0; r < initialFitPolN + 1; r++)
        {
            tFinalTree->Branch(Form("intermediateFitParameter_%i", r), intermediateFitParameter[r],
                               Form("intermediateFitParameter_%i[%i]/D", r, (intermediateFitPolN + 1)));
        }

    int intermediateCount = 0;
    double intermediateCheckSum = 0.0;
    for(int j = 0; j <= jBins; j++)
        {
            kPosition = j / binningFactor;

            for(int r = 0; r < initialFitPolN + 1; r++)
                {
                    TCanvas *cIntermediateHistos = new TCanvas("cIntermediateHistos", " ", 1250, 600);
                    cIntermediateHistos->cd();
                    TH2D *hIntermediateHistos = new TH2D("hIntermediateHistos", "",
                                                         500, xAxisIntermediateMin + (0.025 * xAxisIntermediateMin), xAxisIntermediateMax + (0.025 * xAxisIntermediateMax),
                                                         250, initialFitParameterMin[r] + (0.025 * initialFitParameterMin[r]), initialFitParameterMax[r] + (0.025 * initialFitParameterMax[r]));
                    hIntermediateHistos->Sumw2();

                    tIntermediateTree->Project("hIntermediateHistos", Form("initialFitParameter_%i:iPosition-%f", r, xAxisIntermediateMax), Form("fabs(jPosition-%f)<0.025", kPosition));

                    TF1 *intermediateFitFunction =  new TF1("intermediateFitFunction", Form("pol%i", intermediateFitPolN));
                    intermediateFitFunction->SetLineWidth(1);
                    hIntermediateHistos->Fit(intermediateFitFunction, "Q");
                    intermediateFitFunction->GetParameters(intermediateFitParameter[r]);

                    double intermediateFitCheck = (intermediateFitFunction->GetChisquare()) / (intermediateFitFunction->GetNDF());
                    intermediateCheckSum += intermediateFitCheck;
                    intermediateCount++;

                    hIntermediateHistos->Draw("CONT3");
                    cIntermediateHistos->Update();

                    cIntermediateHistos->SaveAs(Form("%s/hIntermediateHisto_%s_%s_%i_%i.pdf", HistoDirectory.c_str(), FieldToTransform.c_str(), DimensionToTransform.c_str(), r, j));
                    delete hIntermediateHistos;
                    delete cIntermediateHistos;
                }
            tFinalTree->Fill();
        }


    TGraph *gFinalGraph[initialFitPolN + 1][intermediateFitPolN + 1];
    for(int r = 0; r < initialFitPolN + 1; r++)
        {
            for(int i = 0; i < intermediateFitPolN + 1; i++)
                {
                    gFinalGraph[r][i] = new TGraph();

                }
        }

    tFinalTree->SetBranchAddress("kPosition", &kPosition);
    for(int r = 0; r < initialFitPolN + 1; r++)
        {
            tFinalTree->SetBranchAddress(Form("intermediateFitParameter_%i", r), intermediateFitParameter[r]);
        }

    for (int t = 0; t != tFinalTree->GetEntries(); t++)
        {
            tFinalTree->GetEntry(t);
            for(int r = 0; r < initialFitPolN + 1; r++)
                {
                    for(int i = 0; i < intermediateFitPolN + 1; i++)
                        {
                            gFinalGraph[r][i]->SetPoint(t, kPosition, intermediateFitParameter[r][i]);
                        }
                }
        }

    TFile *outputFile = new TFile(Form(OutputDirectory + "/Result_%s_%s.root", FieldToTransform.c_str(), DimensionToTransform.c_str()), "RECREATE");
    for(int r = 0; r < initialFitPolN + 1; r++)
        {
            for(int i = 0; i < intermediateFitPolN + 1; i++)
                {
                    gFinalGraph[r][i]->Write(Form("g%i_%i", r, i));
                }
        }
    outputFile->Write();
    outputFile->Close();

    cout << endl;
    cout << "Initial chi2/ndf average = " << initialCheckSum / initialCount << endl;
    cout << "Intermediate chi2/ndf average = " << intermediateCheckSum / intermediateCount << endl;
    cout << "Done working on dimension " << DimensionToTransform << "!!!" << endl << endl;
}

