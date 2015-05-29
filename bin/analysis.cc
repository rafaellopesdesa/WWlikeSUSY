#include "WWutils.h"

using namespace std;
using namespace ROOT;


int main(int argc, char** argv)
{

  TFile* output = TFile::Open("result.root", "RECREATE");

  // WW baby
  vector<TH1D*> WWhists = readTree("babies/baby_WWTo2L2Nu.root");  
  output->mkdir("WW");
  output->cd("WW");
  for (auto&& hists : WWhists)
    hists->Write();

  // T2tt baby
  vector<TH1D*> T2tthists = readTree("babies/baby_T2tt_2J_mStop-425_mLSP-325.root");  
  output->cd();
  output->mkdir("T2tt");
  output->cd("T2tt");
  for (auto&& hists : T2tthists)
    hists->Write();

  output->Close();

  return 0;
}


