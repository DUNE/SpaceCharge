#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TROOT.h>
#include <TKey.h>
#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>

#include "../../SourceCode/MapSCE.h"

using namespace std;

/////////////////////////////////////////////////////////////////////
//Note: CopyDir and CopyFile copied from Rene Brun
/////////////////////////////////////////////////////////////////////
void CopyDir(TDirectory *source, char* dirName)
{
    TDirectory *savdir = gDirectory;
    TDirectory *adir = savdir->mkdir(dirName);
    adir->cd();

    TKey *key;
    TIter nextkey(source->GetListOfKeys());
    while ((key = (TKey*)nextkey()))
        {
            const char *classname = key->GetClassName();
            TClass *cl = gROOT->GetClass(classname);
            if (!cl)
                {
                    continue;
                }
            if (cl->InheritsFrom("TDirectory"))
                {
                    source->cd(key->GetName());
                    TDirectory *subdir = gDirectory;
                    adir->cd();
                    CopyDir(subdir, dirName);
                    adir->cd();
                }
            else if (cl->InheritsFrom("TTree"))
                {
                    TTree *T = (TTree*)source->Get(key->GetName());
                    adir->cd();
                    TTree *newT = T->CloneTree();
                    newT->Write();
                }
            else
                {
                    source->cd();
                    TObject *obj = key->ReadObj();
                    adir->cd();
                    TNamed *named = (TNamed*)obj;
                    named->SetName(key->GetName());
                    named->Write();
                    delete obj;
                }
        }
    adir->SaveSelf(kTRUE);
    savdir->cd();
}
void CopyFile(const char *fname, char* dirName)
{
    TDirectory *target = gDirectory;
    TFile *f = TFile::Open(fname);
    if (!f || f->IsZombie())
        {
            printf("Cannot copy file: %s\n", fname);
            target->cd();
            return;
        }
    target->cd();
    CopyDir(f, dirName);
    delete f;
    target->cd();
}

int main()
{
    cout << endl;

    TString inputFile = "../../InputFiles/dispOutput_protoDUNE_E500.root";
    string experimentName = "ProtoDUNE";
    double binningFactor = 5.0;

    //TString inputFile = "../../InputFiles/dispOutput_MicroBooNE_E500.root";
    //string experimentName = "MicroBooNE";
    //double binningFactor = 10.0;

    // These directories must exist for it to work
    TString outputFile = "../../OutputFiles";
    string histoDir = "../../HistoDirectory";
    double driftField = 500.0;

    cout << "Doing calculations for " << experimentName << " with drift field of " << driftField << "V/cm" << endl;
    /////////////////////////////////////////////////////////////////////
    // Set location of directories
    /////////////////////////////////////////////////////////////////////
    MapSCE *myMapSCE = new MapSCE();
    myMapSCE->SetInputFile(inputFile);
    myMapSCE->SetOutputDirectory(outputFile);
    myMapSCE->SetHistoDirectory(histoDir);
    myMapSCE->SetDriftField(driftField);
    myMapSCE->SetBinningFactor(binningFactor);

    /////////////////////////////////////////////////////////////////////
    // Spatial field
    /////////////////////////////////////////////////////////////////////
    string field = "Spatial";

    string dimension = "K";
    int initialPolN = 0;
    int intermediatePolN = 0;

    // X dimension
    dimension = "X";
    initialPolN = 4;
    intermediatePolN = 6;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    // Y dimension
    dimension = "Y";
    initialPolN = 5;
    intermediatePolN = 5;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    // Z dimension
    dimension = "Z";
    initialPolN = 3;
    intermediatePolN = 4;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    /////////////////////////////////////////////////////////////////////
    // EField field
    /////////////////////////////////////////////////////////////////////
    field = "EField";

    // X dimension
    dimension = "X";
    initialPolN = 4;
    intermediatePolN = 6;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    // Y dimension
    dimension = "Y";
    initialPolN = 5;
    intermediatePolN = 5;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    // Z dimension
    dimension = "Z";
    initialPolN = 3;
    intermediatePolN = 4;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    /////////////////////////////////////////////////////////////////////
    //Put all outputs in central root files
    /////////////////////////////////////////////////////////////////////
    TFile *finalOutputFile = new TFile(outputFile + "/SCEoffsets_" + experimentName + "_E" + to_string(int(driftField)) + ".root", "RECREATE");

    CopyFile(outputFile + "/Result_Spatial_X.root", (char*)"deltaX");
    CopyFile(outputFile + "/Result_Spatial_Y.root", (char*)"deltaY");
    CopyFile(outputFile + "/Result_Spatial_Z.root", (char*)"deltaZ");

    CopyFile(outputFile + "/Result_EField_X.root", (char*)"deltaExOverE");
    CopyFile(outputFile + "/Result_EField_Y.root", (char*)"deltaEyOverE");
    CopyFile(outputFile + "/Result_EField_Z.root", (char*)"deltaEzOverE");

    delete finalOutputFile;

    cout << endl;
    cout << "All Done!!" << endl << endl;

    return 0;
}
