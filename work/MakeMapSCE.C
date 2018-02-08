// Root stuff
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"

// C++ stuff
#include <iostream>
#include <chrono>
#include <random>

#include "../src/MapSCE.h"

using namespace std;

/////////////////////////////////////////////////////////////////////
//Note: CopyDir and CopyFile copied from Rene Brun
/////////////////////////////////////////////////////////////////////
void CopyDir(TDirectory *source, char* dirName)
{
    source->ls();
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

    /////////////////////////////////////////////////////////////////////
    // These directories must exist for it to work
    /////////////////////////////////////////////////////////////////////
    TString inputFile = "../InputFiles/dispOutput_protoDUNE_E500.root";
    TString outputFile = "../OutputFiles";
    string histoDir = "../HistoDirectory";

    /////////////////////////////////////////////////////////////////////
    // Set location of directories
    /////////////////////////////////////////////////////////////////////
    MapSCE *myMapSCE = new MapSCE();
    myMapSCE->SetInputFile(inputFile);
    myMapSCE->SetOutputDirectory(outputFile);
    myMapSCE->SetHistoDirectory(histoDir);

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
    intermediatePolN = 7;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    // Z dimension
    dimension = "Z";
    initialPolN = 3;
    intermediatePolN = 4;
    myMapSCE->PerformTransformation(field, dimension, initialPolN, intermediatePolN);

    /////////////////////////////////////////////////////////////////////
    // EField field
    /////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////
    //Put all outputs in central root files
    /////////////////////////////////////////////////////////////////////
    TFile *finalOutputFile = new TFile(outputFile + "/OffsetsSCE.root", "RECREATE");
    CopyFile(outputFile + "/Result_Spatial_X.root", (char*)"deltaX");
    CopyFile(outputFile + "/Result_Spatial_Y.root", (char*)"deltaY");
    CopyFile(outputFile + "/Result_Spatial_Z.root", (char*)"deltaZ");

    finalOutputFile->ls();
    delete finalOutputFile;

    cout << endl;
    cout << "All Done!!" << endl << endl;


    return 0;
}
