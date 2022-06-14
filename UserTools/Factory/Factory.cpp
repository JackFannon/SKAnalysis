#include "Factory.h"

Tool* Factory(std::string tool){
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;
if (tool=="ApplyTMVA") ret=new ApplyTMVA;
if (tool=="ExtractFeatures") ret=new ExtractFeatures;
if (tool=="ReadHits") ret=new ReadHits;
if (tool=="ReadMCInfo") ret=new ReadMCInfo;
if (tool=="SearchCandidates") ret=new SearchCandidates;
if (tool=="SetPromptVertex") ret=new SetPromptVertex;
if (tool=="SKRead") ret=new SKRead;
if (tool=="SubtractToF") ret=new SubtractToF;
if (tool=="NTupleMatcher") ret = new NTupleMatcher;
if (tool=="AddNoise") ret = new AddNoise;
if (tool=="WriteOutput") ret=new WriteOutput;
if (tool=="TestTool") ret=new TestTool;
if (tool=="TruthNeutronCaptures") ret=new TruthNeutronCaptures;
if (tool=="TruthNeutronCaptures_v2") ret=new TruthNeutronCaptures_v2;
if (tool=="TruthNeutronCaptures_v3") ret=new TruthNeutronCaptures_v3;
if (tool=="LoadFileList") ret=new LoadFileList;
if (tool=="RootReadTest") ret=new RootReadTest;
if (tool=="PlotNeutronCaptures") ret=new PlotNeutronCaptures;
if (tool=="GracefulStop") ret=new GracefulStop;
if (tool=="LoadBetaSpectraFluka") ret=new LoadBetaSpectraFluka;
if (tool=="PlotMuonDtDlt") ret=new PlotMuonDtDlt;
if (tool=="FitLi9Lifetime") ret=new FitLi9Lifetime;
if (tool=="FitPurewaterLi9NcaptureDt") ret=new FitPurewaterLi9NcaptureDt;
if (tool=="FitSpallationDt") ret=new FitSpallationDt;
if (tool=="PurewaterSpallAbundanceCuts") ret=new PurewaterSpallAbundanceCuts;
if (tool=="lf_allfit") ret=new lf_allfit;
if (tool=="TreeReader") ret=new TreeReader;
if (tool=="TreeReaderDemo") ret=new TreeReaderDemo;
if (tool=="lf_allfit_new") ret=new lf_allfit_new;
if (tool=="evDisp") ret=new evDisp;
if (tool=="CompareRootFiles") ret=new CompareRootFiles;
if (tool=="vectgen") ret=new vectgen;
if (tool=="SK2p2MeV_ntag") ret=new SK2p2MeV_ntag;
if (tool=="ntag_BDT") ret=new ntag_BDT;
if (tool=="SimplifyTree") ret=new SimplifyTree;
if (tool=="PrintMCInfo") ret=new PrintMCInfo;
if (tool=="ReconResolution") ret=new ReconResolution;
if (tool=="CompareMCandSecondaries") ret=new CompareMCandSecondaries;
if (tool=="SpallCandidates") ret=new SpallCandidates;
if (tool=="RelicCandidates") ret=new RelicCandidates;
if (tool=="SkipTriggers") ret=new SkipTriggers;
if (tool=="WallCut") ret=new WallCut;
if (tool=="RunEnergyCut") ret=new RunEnergyCut;
if (tool=="BSGoodCut") ret=new BSGoodCut;
if (tool=="ODCut") ret=new ODCut;
if (tool=="RelicMuonMatching") ret=new RelicMuonMatching;
if (tool=="ReconstructMatchedMuons") ret=new ReconstructMatchedMuons;
  if (tool=="WriteMuonCand") ret=new WriteMuonCand;
return ret;
}

