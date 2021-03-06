#ifndef TREECONVERTER_H
#define TREECONVERTER_H

//TGRSIRunInfo *info = NULL;
TPPG *ppg          = NULL;
TChain *gChain     = NULL;
TList *outList     = NULL;

std::vector<std::string> RootFiles;
std::vector<std::string> CalFiles;
std::vector<std::string> InfoFiles;

class Notifier : public TObject {
public:
Notifier() {
}
~Notifier() {
}

void AddChain(TChain *chain)       {
	fChain = chain;
}
void AddRootFile(std::string name) {
	RootFiles.push_back(name);
}
void AddInfoFile(std::string name) {
	InfoFiles.push_back(name);
}
void AddCalFile(std::string name)  {
	CalFiles.push_back(name);
}

bool Notify() {
	printf("%s loaded.\n", fChain->GetCurrentFile()->GetName());
	ppg = (TPPG*)fChain->GetCurrentFile()->Get("TPPG");

	if (CalFiles.size() > 0) {
		TChannel::ReadCalFile(CalFiles.at(0).c_str());
	} else {
		std::cout << "No calibration file loaded." << std::endl;
	}

	return true;
}

private:
TChain *fChain;
std::vector<std::string> RootFiles;
std::vector<std::string> CalFiles;
std::vector<std::string> InfoFiles;
}; // End Notifier class

Notifier *notifier = new Notifier;

typedef struct grif_event {double charge; double energy; double time; int arrayNumber;} GrifEvent_t;

int ProcessData(TFile* out_file);
int WriteTree(TFile* out_file, TTree* tree);
void OpenRootFile(std::string fileName);
int CheckInputTree(const char *infile);
void AutoFileDetect(std::string fileName);
void PrintUsage(char* argv[]);

#endif
