#ifndef evPlot_H
#define evPlot_H

#include <iostream>
#include <string>

#include <TCanvas.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>

#include "MTreeReader.h"
#include "Tool.h"

#include "skroot.h"

/**
 * \class evPlot
 *
 * This is a balnk template for a Tool used by the script to generate a new
 * custom tool. Please fill out the descripton and author information.
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

class evPlot : public Tool {

public:
  evPlot(); ///< Simple constructor
  bool Initialise(std::string configfile, DataModel &data);
  bool Execute();
  bool Finalise();
  std::string toolName;

  // Tool config
  std::string treeReaderName;
  std::string outputDir;
  std::string outputName;
  int dataSrc;
  int maskTriggers;
  int Tmin;
  int Tmax;
  int Qmin;
  int Qmax;

  // Tree reader
  MTreeReader *myTreeReader = nullptr;

  // Canvas and pads
  TCanvas *plotCanvas = nullptr;
  TVirtualPad *plotPad = nullptr;

  // DEVELOPER STUFF TODO REMOVE
  float maxT = -99999999.;
  float minT = 99999999.;

  // Histograms and graph
  TH1D *hitTimes = nullptr;
  TH1D *hitCharges = nullptr;
  TH2D *hitTimesAndCharges = nullptr;

  // Branches
  const Header *myHeader = nullptr;
  const TQReal *myTQReal = nullptr;

  // Plotting variables
  int totalPMTsHit = -1;
  int cableNumber;
  float charge;
  float time;

  // Verbosity
  int verbosity = 1;
  int v_error = 0;

private:
};

#endif
