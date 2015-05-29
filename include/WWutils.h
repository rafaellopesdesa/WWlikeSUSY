#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TTreeCache.h"
#include "TDirectory.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TH1D.h"

#include "Math/VectorUtil.h"

#include <cmath>
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>

// Auxiliary
template <typename T> std::ostream& operator<< (std::ostream& out, const std::vector<T>& v);
template <typename T> std::vector<TH1D*> operator>>(const std::vector<T>& data, std::vector<TH1D*> hists);
template <typename T> std::vector<TH1D*> operator>>(const std::vector<std::pair<T,T> >& data, std::vector<TH1D*> hists);


// Classification
int getEventType(std::vector <int> bestHyp, std::vector<int> lep_pdgId);
std::vector<int> getBestHypothesis(std::vector<ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > > lep_p4, std::vector<int> id, float ptcut);

// Selections
bool LeptonSelection(std::vector<int> bestHyp, std::vector<ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > > lep_p4, std::vector<int> lep_charge);
bool FlavorSelection(std::vector<int> bestHyp, std::vector<ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > > lep_p4, std::vector<int> lep_pdgId);
bool EtMissSelection(std::vector<int> bestHyp, std::vector<ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > > lep_p4, float met_pt, float met_phi);
bool ThirdLeptonVeto(std::vector<int> bestHyp);

// Calculation stuff
std::vector<double> calculateMtVariables(ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > part1, ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > part2, float met_pt, float met_phi);

// The main analysis code
std::vector<TH1D*> readTree(const char* fileName);
