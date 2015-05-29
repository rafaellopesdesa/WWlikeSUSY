#ifndef MAKEPLOT_H
#define MAKEPLOT_H

// 
#include "TH1D.h"
#include "TStyle.h"
#include "TCanvas.h"

#include <vector>
#include <string>

struct PlotInfo {
  TH1D* Plot;
  std::string Title;
  Color_t Color;
};


TCanvas* dataMCplotMaker(TH1D* Data, std::vector <TH1D*> Backgrounds, std::vector <std::string> Titles, std::string titleIn = "", std::string title2In = "", std::string options_string = "", std::vector <TH1D*> Signals = std::vector<TH1D*>(), std::vector <std::string> SignalTitles = std::vector<std::string>(), std::vector <Color_t> color_input = std::vector<Color_t>());



#endif
