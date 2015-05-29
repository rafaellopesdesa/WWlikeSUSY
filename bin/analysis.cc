#include "WWutils.h"
#include "dataMCplotMaker.h"

#include <string>

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

  // TT baby
  vector<TH1D*> TThists = readTree("babies/baby_TTJets.root");  
  output->mkdir("TT");
  output->cd("TT");
  for (auto&& hists : TThists)
    hists->Write();

  // T2tt baby
  vector<TH1D*> T2tthists = readTree("babies/baby_T2tt_2J_mStop-425_mLSP-325.root");  
  output->cd();
  output->mkdir("T2tt");
  output->cd("T2tt");
  for (auto&& hists : T2tthists)
    hists->Write();

  // Some plots
  output->cd();
  output->mkdir("canvases");
  output->cd("canvases");

  string dists[] = {"ptl", "mt2"};
  TH1D* null = new TH1D("","",1,0,1);
  for (size_t iplot=0; iplot < T2tthists.size(); iplot++) {
    vector<TH1D*> bkgds;
    bkgds.push_back(WWhists[iplot]);
    bkgds.push_back(TThists[iplot]);
    vector<string> titles;
    titles.push_back("WW");
    titles.push_back("Top quark");
    string title(dists[iplot]);
    string title2("");
    string option("");
    vector<TH1D*> signals;
    signals.push_back(T2tthists[iplot]);
    vector<string> signals_titles;
    signals_titles.push_back("T2tt");
    TCanvas* c0 = dataMCplotMaker(null, bkgds, titles, title, title2, option, signals, signals_titles);
    c0->Write(dists[iplot].c_str());
  }
    
  output->Close();

  return 0;
}


