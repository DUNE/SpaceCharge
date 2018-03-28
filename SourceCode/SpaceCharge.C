////////////////////////////////////////////////////////////////////////////////////
// SpaceCharge.C                                                            //
// Brief implementation of class for storing/accessing space charge distortions  //
////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <TFile.h>
#include <TCanvas.h>

#include "SpaceCharge.h"

SpaceCharge::SpaceCharge(std::string filename,
                         const int initialSpatialPolN[3], const int intermediateSpatialPolN[3], const int initialEFieldPolN[3], const int intermediateEFieldPolN[3],
                         const std::string eName)
    : ExperimentName(eName)
{
    for(int i = 0; i < 3; i++)
        {
            initialSpatialFitPolN[i] = initialSpatialPolN[i];
            intermediateSpatialFitPolN[i] = intermediateSpatialPolN[i];

            initialEFieldFitPolN[i] = initialEFieldPolN[i];
            intermediateEFieldFitPolN[i] = intermediateEFieldPolN[i];
        }
    Configure(filename);
}

SpaceCharge::~SpaceCharge()
{
}

bool SpaceCharge::Configure(std::string filename)
{
    if(ExperimentName != "MicroBooNE" && ExperimentName != "ProtoDUNE")
        {
            std::cout << "Experiment " << ExperimentName << " not found! Exiting!" << std::endl;
            exit(1);
        }

    TFile inputfile(filename.c_str(), "READ");

    for(int i = 0; i < initialSpatialFitPolN[0] + 1; i++)
        {
            for(int j = 0; j < intermediateSpatialFitPolN[0] + 1; j++)
                {
                    gSpatialGraphX[i][j] = (TGraph*)inputfile.Get(Form("deltaX/g%i_%i", i, j));

                }
            intermediateSpatialFitFunctionX[i] = new TF1(Form("intermediateSpatialFitFunctionX_%i", i), Form("pol%i", intermediateSpatialFitPolN[0]));
        }
    for(int i = 0; i < initialSpatialFitPolN[1] + 1; i++)
        {
            for(int j = 0; j < intermediateSpatialFitPolN[1] + 1; j++)
                {
                    gSpatialGraphY[i][j] = (TGraph*)inputfile.Get(Form("deltaY/g%i_%i", i, j));

                }
            intermediateSpatialFitFunctionY[i] = new TF1(Form("intermediateSpatialFitFunctionY_%i", i), Form("pol%i", intermediateSpatialFitPolN[1]));
        }
    for(int i = 0; i < initialSpatialFitPolN[2] + 1; i++)
        {
            for(int j = 0; j < intermediateSpatialFitPolN[2] + 1; j++)
                {
                    gSpatialGraphZ[i][j] = (TGraph*)inputfile.Get(Form("deltaZ/g%i_%i", i, j));

                }
            intermediateSpatialFitFunctionZ[i] = new TF1(Form("intermediateSpatialFitFunctionZ_%i", i), Form("pol%i", intermediateSpatialFitPolN[2]));
        }

    initialSpatialFitFunctionX =  new TF1("initialSpatialFitFunctionX", Form("pol%i", initialSpatialFitPolN[0]));
    initialSpatialFitFunctionY =  new TF1("initialSpatialFitFunctionY", Form("pol%i", initialSpatialFitPolN[1]));
    initialSpatialFitFunctionZ =  new TF1("initialSpatialFitFunctionZ", Form("pol%i", initialSpatialFitPolN[2]));

    for(int i = 0; i < initialEFieldFitPolN[0] + 1; i++)
        {
            for(int j = 0; j < intermediateEFieldFitPolN[0] + 1; j++)
                {
                    gEFieldGraphX[i][j] = (TGraph*)inputfile.Get(Form("deltaExOverE/g%i_%i", i, j));

                }
            intermediateEFieldFitFunctionX[i] = new TF1(Form("intermediateEFieldFitFunctionX_%i", i), Form("pol%i", intermediateEFieldFitPolN[0]));
        }
    for(int i = 0; i < initialEFieldFitPolN[1] + 1; i++)
        {
            for(int j = 0; j < intermediateEFieldFitPolN[1] + 1; j++)
                {
                    gEFieldGraphY[i][j] = (TGraph*)inputfile.Get(Form("deltaEyOverE/g%i_%i", i, j));

                }
            intermediateEFieldFitFunctionY[i] = new TF1(Form("intermediateEFieldFitFunctionY_%i", i), Form("pol%i", intermediateEFieldFitPolN[1]));
        }
    for(int i = 0; i < initialEFieldFitPolN[2] + 1; i++)
        {
            for(int j = 0; j < intermediateEFieldFitPolN[2] + 1; j++)
                {
                    gEFieldGraphZ[i][j] = (TGraph*)inputfile.Get(Form("deltaEzOverE/g%i_%i", i, j));

                }
            intermediateEFieldFitFunctionZ[i] = new TF1(Form("intermediateEFieldFitFunctionZ_%i", i), Form("pol%i", intermediateEFieldFitPolN[2]));
        }

    initialEFieldFitFunctionX =  new TF1("initialEFieldFitFunctionX", Form("pol%i", initialEFieldFitPolN[0]));
    initialEFieldFitFunctionY =  new TF1("initialEFieldFitFunctionY", Form("pol%i", initialEFieldFitPolN[1]));
    initialEFieldFitFunctionZ =  new TF1("initialEFieldFitFunctionZ", Form("pol%i", initialEFieldFitPolN[2]));

    inputfile.Close();

    return true;
}

// Primary working method of service that provides position offsets to be
// used in ionization electron drift
std::vector<double> SpaceCharge::GetPosOffsets(double xVal, double yVal, double zVal) const
{
    std::vector<double> thePosOffsets;

    if(IsInsideBoundaries(xVal, yVal, zVal) == false)
        {
            thePosOffsets.resize(3, 0.0);
        }
    else
        {
            thePosOffsets = GetPosOffsetsParametric(xVal, yVal, zVal);
        }

    return thePosOffsets;
}

// Provides position offsets using a parametric representation
std::vector<double> SpaceCharge::GetPosOffsetsParametric(double xVal, double yVal, double zVal) const
{
    std::vector<double> thePosOffsetsParametric;

    double xValNew = TransformX(xVal);
    double yValNew = TransformY(yVal);
    double zValNew = TransformZ(zVal);

    thePosOffsetsParametric.push_back(GetOnePosOffsetParametric(xValNew, yValNew, zValNew, "X"));
    thePosOffsetsParametric.push_back(GetOnePosOffsetParametric(xValNew, yValNew, zValNew, "Y"));
    thePosOffsetsParametric.push_back(GetOnePosOffsetParametric(xValNew, yValNew, zValNew, "Z"));

    return thePosOffsetsParametric;
}


// Provides one position offset using a parametric representation, for a given axis
double SpaceCharge::GetOnePosOffsetParametric(double xValNew, double yValNew, double zValNew, std::string axis) const
{
    double parA[99][99];
    double parB[99];

    for(int i = 0; i < 99; i++)
        {
            for(int j = 0; j < 99; j++)
                {
                    parA[i][j] = 0.0;
                }
            parB[i] = 0.0;
        }
    if(axis == "X")
        {
            for(int i = 0; i < initialSpatialFitPolN[0] + 1; i++)
                {
                    for(int j = 0; j < intermediateSpatialFitPolN[0] + 1; j++)
                        {
                            parA[i][j] = gSpatialGraphX[i][j]->Eval(zValNew);
                        }
                    intermediateSpatialFitFunctionX[i]->SetParameters(parA[i]);
                }
        }
    else if(axis == "Y")
        {
            for(int i = 0; i < initialSpatialFitPolN[1] + 1; i++)
                {
                    for(int j = 0; j < intermediateSpatialFitPolN[1] + 1; j++)
                        {
                            parA[i][j] = gSpatialGraphY[i][j]->Eval(zValNew);
                        }
                    intermediateSpatialFitFunctionY[i]->SetParameters(parA[i]);
                }
        }
    else if(axis == "Z")
        {
            for(int i = 0; i < initialSpatialFitPolN[2] + 1; i++)
                {
                    for(int j = 0; j < intermediateSpatialFitPolN[2] + 1; j++)
                        {
                            parA[i][j] = gSpatialGraphZ[i][j]->Eval(zValNew);
                        }
                    intermediateSpatialFitFunctionZ[i]->SetParameters(parA[i]);
                }
        }

    double aValNew;
    double bValNew;

    if(axis == "Y")
        {
            aValNew = xValNew;
            bValNew = yValNew;
        }
    else
        {
            aValNew = yValNew;
            bValNew = xValNew;
        }
    double offsetValNew = 0.0;
    if(axis == "X")
        {
            for(int i = 0; i < initialSpatialFitPolN[0] + 1; i++)
                {
                    parB[i] = intermediateSpatialFitFunctionX[i]->Eval(aValNew);
                }
            initialSpatialFitFunctionX->SetParameters(parB);
            offsetValNew = 100.0 * initialSpatialFitFunctionX->Eval(bValNew);
        }
    else if(axis == "Y")
        {
            for(int i = 0; i < initialSpatialFitPolN[1] + 1; i++)
                {
                    parB[i] = intermediateSpatialFitFunctionY[i]->Eval(aValNew);
                }
            initialSpatialFitFunctionY->SetParameters(parB);
            offsetValNew = 100.0 * initialSpatialFitFunctionY->Eval(bValNew);
        }
    else if(axis == "Z")
        {
            for(int i = 0; i < initialSpatialFitPolN[2] + 1; i++)
                {
                    parB[i] = intermediateSpatialFitFunctionZ[i]->Eval(aValNew);
                }
            initialSpatialFitFunctionZ->SetParameters(parB);
            offsetValNew = 100.0 * initialSpatialFitFunctionZ->Eval(bValNew);
        }

    return offsetValNew;
}

// Primary working method of service that provides E field offsets to be
// used in charge/light yield calculation (e.g.)
std::vector<double> SpaceCharge::GetEfieldOffsets(double xVal, double yVal, double zVal) const
{
    std::vector<double> theEfieldOffsets;

    if(IsInsideBoundaries(xVal, yVal, zVal) == false)
        {
            theEfieldOffsets.resize(3, 0.0);
        }
    else
        {
            theEfieldOffsets = GetEfieldOffsetsParametric(xVal, yVal, zVal);
        }

    //theEfieldOffsets.at(0) = -1.0 * theEfieldOffsets.at(0);
    //theEfieldOffsets.at(1) = -1.0 * theEfieldOffsets.at(1);
    //theEfieldOffsets.at(2) = -1.0 * theEfieldOffsets.at(2);

    return theEfieldOffsets;
}

// Provides E field offsets using a parametric representation
std::vector<double> SpaceCharge::GetEfieldOffsetsParametric(double xVal, double yVal, double zVal) const
{
    std::vector<double> theEfieldOffsetsParametric;

    double xValNew = TransformX(xVal);
    double yValNew = TransformY(yVal);
    double zValNew = TransformZ(zVal);

    theEfieldOffsetsParametric.push_back(GetOneEfieldOffsetParametric(xValNew, yValNew, zValNew, "X"));
    theEfieldOffsetsParametric.push_back(GetOneEfieldOffsetParametric(xValNew, yValNew, zValNew, "Y"));
    theEfieldOffsetsParametric.push_back(GetOneEfieldOffsetParametric(xValNew, yValNew, zValNew, "Z"));

    return theEfieldOffsetsParametric;
}

// Provides one E field offset using a parametric representation, for a given axis
double SpaceCharge::GetOneEfieldOffsetParametric(double xValNew, double yValNew, double zValNew, std::string axis) const
{
    double parA[99][99];
    double parB[99];

    for(int i = 0; i < 99; i++)
        {
            for(int j = 0; j < 99; j++)
                {
                    parA[i][j] = 0.0;
                }
            parB[i] = 0.0;
        }
    if(axis == "X")
        {
            for(int i = 0; i < initialEFieldFitPolN[0] + 1; i++)
                {
                    for(int j = 0; j < intermediateEFieldFitPolN[0] + 1; j++)
                        {
                            parA[i][j] = gEFieldGraphX[i][j]->Eval(zValNew);
                        }
                    intermediateEFieldFitFunctionX[i]->SetParameters(parA[i]);
                }
        }
    else if(axis == "Y")
        {
            for(int i = 0; i < initialEFieldFitPolN[1] + 1; i++)
                {
                    for(int j = 0; j < intermediateEFieldFitPolN[1] + 1; j++)
                        {
                            parA[i][j] = gEFieldGraphY[i][j]->Eval(zValNew);
                        }
                    intermediateEFieldFitFunctionY[i]->SetParameters(parA[i]);
                }
        }
    else if(axis == "Z")
        {
            for(int i = 0; i < initialEFieldFitPolN[2] + 1; i++)
                {
                    for(int j = 0; j < intermediateEFieldFitPolN[2] + 1; j++)
                        {
                            parA[i][j] = gEFieldGraphZ[i][j]->Eval(zValNew);
                        }
                    intermediateEFieldFitFunctionZ[i]->SetParameters(parA[i]);
                }
        }

    double aValNew;
    double bValNew;

    if(axis == "Y")
        {
            aValNew = xValNew;
            bValNew = yValNew;
        }
    else
        {
            aValNew = yValNew;
            bValNew = xValNew;
        }

    double offsetValNew = 0.0;
    if(axis == "X")
        {
            for(int i = 0; i < initialEFieldFitPolN[0] + 1; i++)
                {
                    parB[i] = intermediateEFieldFitFunctionX[i]->Eval(aValNew);
                }
            initialEFieldFitFunctionX->SetParameters(parB);
            offsetValNew = initialEFieldFitFunctionX->Eval(bValNew);
        }
    else if(axis == "Y")
        {
            for(int i = 0; i < initialEFieldFitPolN[1] + 1; i++)
                {
                    parB[i] = intermediateEFieldFitFunctionY[i]->Eval(aValNew);
                }
            initialEFieldFitFunctionY->SetParameters(parB);
            offsetValNew = initialEFieldFitFunctionY->Eval(bValNew);
        }
    else if(axis == "Z")
        {
            for(int i = 0; i < initialEFieldFitPolN[2] + 1; i++)
                {
                    parB[i] = intermediateEFieldFitFunctionZ[i]->Eval(aValNew);
                }
            initialEFieldFitFunctionZ->SetParameters(parB);
            offsetValNew = initialEFieldFitFunctionZ->Eval(bValNew);
        }

    return offsetValNew;
}

// Transform X to SCE X coordinate
double SpaceCharge::TransformX(double xVal) const
{
    double xValNew = 0.0;

    if(ExperimentName == "MicroBooNE")
        {
            xValNew = 2.50 - (2.50 / 2.56) * (xVal / 100.0);
            xValNew -= 1.25;
        }
    else if(ExperimentName == "ProtoDUNE")
        {
            xValNew = (fabs(xVal) / 100.0);
            xValNew -= 1.8;
        }

    return xValNew;
}

// Transform Y to SCE Y coordinate
double SpaceCharge::TransformY(double yVal) const
{
    double yValNew = 0.0;
    if(ExperimentName == "MicroBooNE")
        {
            yValNew = (2.50 / 2.33) * ((yVal / 100.0) + 1.165);
            yValNew -= 1.25;
        }
    else if(ExperimentName == "ProtoDUNE")
        {
            yValNew = (6.00 / 6.08) * ((yVal + 0.2) / 100.0);
            yValNew -= 3.0;
        }
    return yValNew;
}

// Transform Z to SCE Z coordinate
double SpaceCharge::TransformZ(double zVal) const
{
    double zValNew = 0.0;
    if(ExperimentName == "MicroBooNE")
        {
            zValNew = (10.0 / 10.37) * (zVal / 100.0);
        }
    else if(ExperimentName == "ProtoDUNE")
        {
            zValNew = (7.20 / 6.97) * ((zVal + 0.8) / 100.0);
        }
    return zValNew;
}

// Check to see if point is inside boundaries of map (allow to go slightly out of range)
bool SpaceCharge::IsInsideBoundaries(double xVal, double yVal, double zVal) const
{
    bool isInside = true;
    if(ExperimentName == "MicroBooNE")
        {
            if((xVal < 0.0) || (xVal > 260.0) || (yVal < -120.0) || (yVal > 120.0) || (zVal < 0.0) || (zVal > 1050.0))
                {
                    isInside = false;
                }
        }
    else if(ExperimentName == "ProtoDUNE")
        {
            if((xVal < -360.0) || (xVal > 360.0) || (yVal < -5.0) || (yVal > 615.0) || (zVal < -5.0) || (zVal > 705.0))
                {
                    isInside = false;
                }
        }

    return isInside;
}
