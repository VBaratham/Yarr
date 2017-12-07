// #################################
// # Author: Vyassa Baratham
// # Email: vbaratham at berkeley.edu
// # Project: Yarr
// # Description: Read raw data for telescope
// # Comment: Convert multiple raw output files into event-based ROOT tree
// ################################

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "Fei4EventData.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TF1.h"

// FEI4B geometry parameters
#define H                     4.0  // interplanar distance, cm
#define W                     1.9  // sensor side length
#define COL_PX                 80  // pixels per column
#define COL_PX_TO_CM   (W/COL_PX)  // conversion factor for FEI4 pixels --> cm
#define ROW_PX                336  // pixels per row
#define ROW_PX_TO_CM   (W/ROW_PX)  // conversion factor for FEI4 pixels --> cm

int main(int argc, char* argv[]){
  if (argc < 1){
    std::cout << "Usage: ./telescope_read <yarr raw datafile (bottom FE)> [...] <yarr raw datafile (top FE)> (but I only support 2 FEs right now)" << std::endl;
    return -1;
  }

  std::vector<std::fstream*> files;

  // Open files
  for(int i = 1; i < argc; ++i)
    files.push_back(new std::fstream(argv[i]));

  // tree variables
  uint16_t bcid;
  std::vector<int> iden, col, row, tot, lv1;
  Double_t theta, phi;

  // Keep the current event from all FEs to calculate per-event stuff involving all FEs
  std::vector<Fei4Event*> all_FE_events;

  // tree and branches
  TFile* rootfile = new TFile("rootfile.root","RECREATE");
  TTree* tree = new TTree("tree", "tree");
  // tree->Branch("nhits", &nhits, "nhits/I");
  // tree->Branch("tag", &l1id, "tag/I");
  tree->Branch("bcid", &bcid, "bcid/I");
  tree->Branch("theta", &theta);
  tree->Branch("phi", &phi);
  tree->Branch("iden", &iden);
  tree->Branch("col", &col);
  tree->Branch("row", &row);
  tree->Branch("tot", &tot);
  tree->Branch("lv1", &lv1);

  // histograms
  TH2F *cluster_len = new TH2F("cluster_len", "cluster_len", 10, 0, 10, 10, 0, 10);
  TH2F *cluster_wid = new TH2F("cluster_wid", "cluster_wid", 10, 0, 10, 10, 0, 10);

  // Report file sizes
  for(std::fstream *file : files) {
      file->seekg(0, std::ios_base::end);
      int size = file->tellg();
      file->seekg(0, std::ios_base::beg);
      std::cout << "Size: " << size/1024.0/1024.0 << " MB" << std::endl;
  }

  // // Check nevents in each file:
  // for(unsigned i = 0; i < files.size(); ++i) {
  //   int nevents = 0;
  //   while(files[i]) {
  //     if (!*files[i])
  // 	break;
  //     Fei4Event event;
  //     event.fromFileBinary(*files[i]);
  //     nevents++;
  //   }
  //   std::cout << nevents << std::endl;
  // }

  int event_i = 0;
  while(true) {
    // TODO: implement checks that all files have same nevents (see commented code above)
    iden.clear();
    lv1.clear();
    col.clear();
    row.clear();
    tot.clear();

    if (!*files[0]) // done w/ all files if files[0] is done (hopefully)
      break;

    for (unsigned i = 0; i < files.size(); i++) {
      Fei4Event *event = new Fei4Event();
      all_FE_events.push_back(event);
      event->fromFileBinary(*files[i]);

      if (!*files[i])
	break;

      event->doClustering();
      bcid = event->bcid;

      for (int j = 0; j < event->nHits; ++j){
	iden.push_back(i);
	lv1.push_back(event->l1id);
	col.push_back(event->hits[j].col);
	row.push_back(event->hits[j].row);
	tot.push_back(event->hits[j].tot);
      }

      // std::cout << event->bcid << ", "
      //           << i << ", "
      //           << event->l1id << ", "
      //           << event->nHits << std::endl;
                
    }

    // Calculate incomimg angle
    std::cout << all_FE_events[0]->clusters.size() << " " << all_FE_events[1]->clusters.size() << std::endl;
    if (all_FE_events[0]->clusters.size() == 1 && all_FE_events[1]->clusters.size() == 1){
      // TODO: handle events with 2 clusters
      double x1 = all_FE_events[0]->clusters[0].getMidRow() * ROW_PX_TO_CM;
      double x2 = all_FE_events[1]->clusters[0].getMidRow() * ROW_PX_TO_CM;
      double y1 = all_FE_events[0]->clusters[0].getMidCol() * COL_PX_TO_CM;
      double y2 = all_FE_events[1]->clusters[0].getMidCol() * COL_PX_TO_CM;
      if (abs(x1-x2) > W || abs(y1-y2) > W){
	std::cout << "x or y distance greater than sensor size" << std::endl;
      	theta = -999;
	phi = -999;
      } else {
	theta = atan( sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) ) / H );
	phi = atan2(y2-y1, x2-x1);
	std::cout << theta << " " << phi << " " << x2 - x1 << " " << y2 - y1 << std::endl;
      }

      // Plot cluster length/width correlations
      cluster_len->Fill(all_FE_events[0]->clusters[0].getColLength(),
			all_FE_events[1]->clusters[0].getColLength());
      cluster_wid->Fill(all_FE_events[0]->clusters[0].getRowWidth(),
			all_FE_events[1]->clusters[0].getRowWidth());
    } else {
      // std::cout << "Skipped calculating theta for event with "
      // 	        << all_FE_events[0]->clusters.size()
      // 	        << " hits on FE 0, and "
      // 	        << all_FE_events[1]->clusters.size()
      // 	        << " hits on FE 1" << std::endl;
	
      theta = -999;
      phi = -999;
    }

    
    all_FE_events.clear();

    tree->Fill();

    event_i += 1;
  }

  // Fit cos^2 to angular distribution
  TF1 *f = new TF1("cos2", "[0]*(cos(x - [1]))^2 - [2]", 0, .5);
  f->SetParameter(0, 30);
  f->SetParameter(1, 0.2);
  f->SetParameter(1, 20);
  tree->Draw("theta>>theta(15, 0, .5)", "theta != -999");
  TH1F *histo = (TH1F*) gDirectory->Get("theta");
  histo->Fit(f);
  f->Write();
  histo->Write();
  
  tree->Write();
  cluster_len->Write();
  cluster_wid->Write();
  rootfile->Close();

}
