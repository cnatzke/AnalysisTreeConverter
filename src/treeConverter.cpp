//g++ treeConvert.cxx -std=c++11 `grsi-config --cflags --all-libs --GRSIData-libs --root` -I$GRSISYS/GRSIData/include -o TreeConverter

#include "TFile.h"
#include "TChain.h"
#include "TPPG.h"
#include "TGRSIUtilities.h"
#include "TParserLibrary.h" // needed for GetRunNumber
//#include "TGRSIRunInfo.h"
#include "progress_bar.h"
#include "treeConverter.h"

#include <iostream>
#include <iomanip> // setiosflags
#include <unistd.h>

#ifndef __CINT__
#include "TGriffin.h"
#endif

Notifier *notifier = new Notifier;

int main(int argc, char **argv) {

    if (argc == 1){ // no inputs given
        PrintUsage(argv);
        return 0;
    }

   for (auto i = 1; i < argc; i++) AutoFileDetect(argv[i]);

   if (!gChain) std::cout << "No gChain found" << std::endl;
   if (!gChain->GetEntries()) std::cout << "Found gChain, but no entries retrieved" << std::endl;

   if (!gChain || !gChain->GetEntries()){
      std::cerr << "Failed to find anything. Exiting" << std::endl;
      return 1;
   }

   std::string fName = gChain->GetCurrentFile()->GetName();
   int run_number = GetRunNumber(fName.c_str());

   std::cout << "Starting run " << run_number << " with " << gChain->GetNtrees() << " files" << std::endl;

   // Defining RootFile branch structure
   typedef struct {double energy; double time; int arrayNumber;} GRIFFIN;
   GRIFFIN fGrif;

   // Output (converted) ROOT file
   TFile *out_file = new TFile("./convertedFile.root", "RECREATE");

   // Branches
   TTree *griffin_tree = new TTree("griffin", "GRIFFIN Hits");
   griffin_tree->Branch("fGrif", &fGrif, "energy/D:time/D:arrayNumber/I");

   long analysis_entries = gChain->GetEntries();

   TGriffin *griffin_data = NULL;
   if (gChain->FindBranch("TGriffin")){
      gChain-> SetBranchAddress("TGriffin", &griffin_data);
      std::cout << "Succesfully found TGriffin Branch" << std::endl;
   }

   std::cout << "Starting fill loop" << std::endl;

   /* Creates a progress bar that has a width of 70,
    * shows '=' to indicate completion, and blank
    * space for incomplete
    */
   ProgressBar progress_bar(analysis_entries, 70, '=', ' ');
   for (auto i = 0; i < analysis_entries; i++){
      // retrieve entries from trees
      gChain->GetEntry(i);

      for (auto g = 0; g < griffin_data->GetMultiplicity(); g++){
         TGriffinHit *grifHit = griffin_data->GetGriffinHit(g);

         // get leaves
         fGrif.energy = grifHit->GetEnergy();
         fGrif.time = grifHit->GetTime();
         fGrif.arrayNumber = grifHit->GetArrayNumber() - 1;

         // fill tree
         griffin_tree->Fill();
      } // end griffin hits

      if (i % 10000 == 0){
         progress_bar.display();
      }
      ++progress_bar; // iterates progress_bar
   } // end fill loop

   progress_bar.done();

   out_file->cd();
   griffin_tree->Write("", TObject::kOverwrite); // Overwrites old versions of the file
   //out_file->Write();

   out_file->Purge(); // removes lower namecycle copies of t_file->Close();

   std::cout << "Done!" << std::endl;

   return 0;
} // End main

void OpenRootFile(std::string fileName){
   TFile f(fileName.c_str());
   if (f.Get("AnalysisTree")){
      if (!gChain) {
         gChain = new TChain("AnalysisTree");
         notifier->AddChain(gChain);
         gChain->SetNotify(notifier);
      }
      gChain->Add(fileName.c_str());
      std::cout << "Added: " << fileName << std::endl;
   }
} // end OpenRootFile

void AutoFileDetect(std::string fileName){
   size_t dot_pos = fileName.find_last_of('.');
   std::string ext = fileName.substr(dot_pos + 1);

   if (ext == "root"){
      OpenRootFile(fileName);
   }
   else if (ext == "cal"){
      notifier->AddCalFile(fileName);
   } else {
      std::cerr << "Discarding unknown file: " << fileName.c_str() << std::endl;
   }
} // End AutoFileDetect

void PrintUsage(char* argv[]){
    std::cerr << "usage: " << argv[0] << " analysis_tree calibration_file\n"
    << " analysis_tree:    analysis tree to convert (must end with .root)\n"
    << " calibration_file: calibration file (must end with .cal)"
    << std::endl;
} // end PrintUsage
