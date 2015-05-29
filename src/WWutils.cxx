#define _USE_MATH_DEFINES

#include "WWutils.h"
#include "Mt2/Basic_Mt2_332_Calculator.h"

using namespace std;
using namespace ROOT;

typedef Math::LorentzVector< Math::PxPyPzE4D<float> > LorentzVector;

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v)
{
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

template <typename T> std::vector<TH1D*> operator>>(const std::vector<T>& data, std::vector<TH1D*> hists) 
{

  if (hists.size() != data.size()) {
    cout << "Hitograms and data have different sizes, can't fill it." << endl;
  } else {
    for (size_t i = 0; i < hists.size(); i++) {
      hists[i]->Fill(data[i]);
      //      cout << data[i] << endl;
    }
  }
  return hists;
    
}

int getEventType(vector <int> bestHyp, vector<int> lep_pdgId)
{
  int retval;
  if (abs(lep_pdgId[bestHyp[0]]) == 11 && abs(lep_pdgId[bestHyp[0]]) == 11) retval = 0; // ee
  else if (abs(lep_pdgId[bestHyp[0]]) == 13 && abs(lep_pdgId[bestHyp[0]]) == 13) retval = 1; // mm 
  else if (abs(lep_pdgId[bestHyp[0]]) == 11 && abs(lep_pdgId[bestHyp[0]]) == 13) retval = 2; // em
  else if (abs(lep_pdgId[bestHyp[0]]) == 13 && abs(lep_pdgId[bestHyp[0]]) == 11) retval = 3; // me
  else retval = -1;
  return retval;
}
  

vector<int> getBestHypothesis(vector<LorentzVector> lep_p4, vector<int> id, float ptcut) {

  int index1 = -1;
  int index2 = -1;
  int index3 = -1;
  for (size_t ilep = 0; ilep < lep_p4.size(); ilep++) {
    if (index1 > -1 && index2 > -1) {
      index3 = ilep;
      break;
    }    
    if (lep_p4[ilep].Pt() > ptcut && id[ilep] == 1) {
      if (index1 < 0) {
	index1 = ilep;
      } else if (index2 < 0) {
	index2 = ilep;
      }
    }
  }
  vector<int> retval;
  retval.push_back(index1);
  retval.push_back(index2);
  retval.push_back(index3);        
  return retval;
  
}

bool LeptonSelection(vector<int> bestHyp, vector<LorentzVector> lep_p4, vector<int> lep_charge) {

  if (bestHyp[0] < 0 || bestHyp[1] < 0) return false;
  bool retval = true;
  retval &= ((lep_charge[bestHyp[0]]*lep_charge[bestHyp[1]]) < 0);
  retval &= ((lep_p4[bestHyp[0]]+lep_p4[bestHyp[1]]).M() > 12.);
  return retval;
}

bool FlavorSelection(vector<int> bestHyp, vector<LorentzVector> lep_p4, vector<int> lep_pdgId) {

  if (bestHyp[0] < 0 || bestHyp[1] < 0) return false;
  bool retval = true;
  if (abs(lep_pdgId[bestHyp[0]]) != abs(lep_pdgId[bestHyp[1]])) {
    retval &= ((lep_p4[bestHyp[0]]+lep_p4[bestHyp[1]]).pt() > 30.);
  } else {
    retval &= ((lep_p4[bestHyp[0]]+lep_p4[bestHyp[1]]).pt() > 45.);
    retval &= (fabs((lep_p4[bestHyp[0]]+lep_p4[bestHyp[1]]).M()-91.1875) > 15.);
  }
  return retval;
}

bool EtMissSelection(vector<int> bestHyp, vector<LorentzVector> lep_p4, float met_pt, float met_phi) {
  
  if (bestHyp[0] < 0 || bestHyp[1] < 0) return false;
  bool retval = true;
  float dphi = min(fabs(fmod(met_phi-lep_p4[bestHyp[0]].phi(), 2.*M_PI)),fabs(fmod(met_phi-lep_p4[bestHyp[1]].phi(), 2.*M_PI)));
  float projmet = dphi < M_PI/2. ? met_pt*sin(dphi) : met_pt;
  retval &= (projmet > 20.);

  return retval;
}

bool ThirdLeptonVeto(vector<int> bestHyp) {
  return (bestHyp[2] == -1);
}

vector<double> calculateMtVariables(LorentzVector part1, LorentzVector part2, float met_pt, float met_phi) {

  Mt2::Basic_Mt2_332_Calculator mt2Calculator;

  //  cout << part1.Px() << " " << part1.Py() << " " << part1.M() << endl;
  //  cout << part2.Px() << " " << part2.Py() << " " << part2.M() << endl;
  //  cout << met_pt*cos(met_phi) << " " << met_pt*sin(met_phi) << endl;
  
  Mt2::LorentzTransverseVector  vis_1(Mt2::TwoVector(part1.Px(), part1.Py()), 0.1);
  Mt2::LorentzTransverseVector  vis_2(Mt2::TwoVector(part2.Px(), part2.Py()), 0.1);
  Mt2::TwoVector                pT_Miss(met_pt*cos(met_phi), met_pt*sin(met_phi));
  
  const double mt2 = mt2Calculator.mt2_332(vis_1, vis_2, pT_Miss, 10.);
  //  cout << mt2 << endl;
  
  vector<double> retval;
  retval.push_back(mt2);
  return retval;
}


vector<TH1D*> readTree(const char* fileName) {

  TH1D* leading_lepton_pt = new TH1D("leading_lepton_pt", "Leading Lepton pT", 100, 0., 200.);
  TH1D* subleading_lepton_pt = new TH1D("subleading_lepton_pt", "Subleading Lepton pT", 100, 0., 200.);
  TH1D* leading_lepton_mt = new TH1D("leading_lepton_mt", "Leading Lepton mT", 100, 0., 200.);
  TH1D* subleading_lepton_mt = new TH1D("subleading_lepton_mt", "Leading Lepton mT", 100, 0., 200.);
  TH1D* ll_pt = new TH1D("ll_pt", "Dilepton pT", 100, 0., 200.);
  TH1D* ll_m = new TH1D("ll_m", "Dilepton mass", 100, 0., 200.);
  TH1D* ll_mt2 = new TH1D("ll_mt2", "ll mT2", 100, 0., 200.);
  TH1D* ll_dphi = new TH1D("ll_dphi", "ll delta phi", 32, 0., 2*M_PI);
  TH1D* lljj_mt2 = new TH1D("lljj_mt2", "lljj mT2", 100, 0., 200.);
  TH1D* cutflow = new TH1D("cutflow", "Cut Flow", 5, 0., 5.);

  vector<TH1D*> hists;
  hists.push_back(leading_lepton_pt);
  // hists.push_back(subleading_lepton_pt);
  // hists.push_back(leading_lepton_mt);
  // hists.push_back(subleading_lepton_mt);
  // hists.push_back(ll_pt);
  // hists.push_back(ll_m);
  hists.push_back(ll_mt2);
  // hists.push_back(ll_dphi);
  // hists.push_back(lljj_mt2);
  // hists.push_back(cutflow);
  
  TFile* file = TFile::Open(fileName);

  TTreeReader reader("t", file);
  TTreeReaderValue<vector<LorentzVector> > lep_p4(reader, "lep_p4");
  TTreeReaderValue<vector<int> > lep_tightId(reader, "lep_tightId");
  TTreeReaderValue<vector<int> > lep_charge(reader, "std_vector_lepton_ch");
  TTreeReaderValue<vector<int> > lep_pdgId(reader, "std_vector_lepton_id");
  
  TTreeReaderValue<float > met_pt(reader, "met_pt");
  TTreeReaderValue<float > met_phi(reader, "met_phi");

  // To store the results
  vector<double> data;
  vector<double> mt_vec;
  
  while (reader.Next()) {
    vector<int> bestHyp      = getBestHypothesis(*lep_p4, *lep_tightId, 20.);
    bool passLeptonSelection = LeptonSelection(bestHyp, *lep_p4, *lep_charge);
    bool passFlavorSelection = FlavorSelection(bestHyp, *lep_p4, *lep_pdgId);
    bool passEtMissSelection = EtMissSelection(bestHyp, *lep_p4, *met_pt, *met_phi);
    bool passThirdLeptonVeto = ThirdLeptonVeto(bestHyp);
    bool passAll = (passLeptonSelection && passFlavorSelection && passEtMissSelection && passThirdLeptonVeto);
    // bool passAll = (passLeptonSelection);

    if (passAll) {      

      data.clear();
      mt_vec.clear();

      data.resize(2);
      data[0] = 0.;
      data[1] = 0.;
      
      mt_vec = calculateMtVariables(lep_p4->at(bestHyp[0]), lep_p4->at(bestHyp[1]), *met_pt, *met_phi);
      //      cout << "mt2 " << mt_vec[0] << endl;
           
      data[0] = (lep_p4->at(bestHyp[0]).Pt());
      data[1] = (mt_vec[0]);
      data >> hists;
      
    }
    
  }   
  file->Close();
  return hists;
  
  
}
